/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>
#include "extmem8.h"

//#define INTERNAL_TEST

// While 20 MHz (Teensy actually uses 16 MHz in most cases) and even 24 MHz
// have worked well in testing at room temperature with 3.3V power, to fully
// meet all the worst case timing specs, the SPI clock low time would need
// to be 40ns (12.5 MHz clock) for the single chip case and 51ns (9.8 MHz
// clock) for the 6-chip memoryboard with 74LCX126 buffers.
//
// Timing analysis and info is here:
// https://forum.pjrc.com/threads/29276-Limits-of-delay-effect-in-audio-library?p=97506&viewfull=1#post97506

#define SIZEOF_SAMPLE (sizeof(((audio_block_t*) 0)->data[0]))

static const uint32_t NOT_ENOUGH_MEMORY = 0xFFFFFFFF;

// This memory size array needs to match the sizes of 
// the entries in AudioEffectDelayMemoryType8_t
const uint32_t AudioExtMem8::memSizeSamples[AUDIO_MEMORY8_UNDEFINED] = {65536,393216,262144,4194304,8000};
AudioExtMem8* AudioExtMem8::first[AUDIO_MEMORY8_UNDEFINED] = {nullptr};


AudioExtMem8::~AudioExtMem8() 
{
	switch (memory_type)
	{
		case AUDIO_MEMORY8_HEAP:
			free((void*) memory_begin);
			break;
			
#if defined(ARDUINO_TEENSY41)
		case AUDIO_MEMORY8_EXTMEM:
			extmem_free((void*) memory_begin);
			break;
#endif // defined(ARDUINO_TEENSY41)
			
		// audio SPI memory is tracked by AudioExtMem8 
		// objects thenselves - no need to free	
		default:
			break;		
	}
	linkOut();
}

void AudioExtMem8::linkIn(void)
{
	if (memory_type < AUDIO_MEMORY8_UNDEFINED)
	{
		AudioExtMem8** ppEM = &first[memory_type]; 
		
		while (nullptr != *ppEM)
		{
			if (memory_begin > (*ppEM)->memory_begin)
				ppEM = &((*ppEM)->next);
			else
				break;
		}
		next = *ppEM;
		*ppEM = this;
	}
}


void AudioExtMem8::linkOut(void)
{
	if (memory_type < AUDIO_MEMORY8_UNDEFINED) // This Never Happens...
	{
		AudioExtMem8** ppEM = &first[memory_type]; 
		
		while (nullptr != *ppEM)
		{
			if (this != *ppEM)
				ppEM = &((*ppEM)->next);
			else
			{
				*ppEM = next;
				break;
			}
		}
		next = nullptr; // not really necessary, but...
	}
}


/**
 * Find space for given number of samples. This MUST be called before the
 * newly-created AudioExtMem8 object is linked into the allocation list.
 */
uint32_t AudioExtMem8::findSpace(AudioEffectDelayMemoryType8_t memory_type, uint32_t samples)
{
	uint32_t result = 0;
	bool gotOne = false;
	
	if (memory_type < AUDIO_MEMORY8_UNDEFINED) // This Never Happens...
	{
		AudioExtMem8** ppEM = &first[memory_type]; 
		
		do
		{
			uint32_t next_start;
			if (nullptr == *ppEM) // end of list, or first memory allocation
				next_start = memSizeSamples[memory_type];
			else // we've found an object using memory
			{
				AudioExtMem8* nextObj = (*ppEM)->next;
				result = (*ppEM)->memory_begin + (*ppEM)->memory_length; // end of object's allocation
				if (nullptr != nextObj)
					next_start = nextObj->memory_begin;
				else
					next_start = memSizeSamples[memory_type];
				
				ppEM = &((*ppEM)->next);
			}
			
			// simple-minded allocation: first found fit
			if (samples <= (next_start - result))
				gotOne = true;
			
		} while (!gotOne && nullptr != *ppEM);
	}	
	
	if (!gotOne)
		result = NOT_ENOUGH_MEMORY;
	
	return result;
}


/**
 * Find maximum contiguous space in a memory.
 */
uint32_t AudioExtMem8::findMaxSpace(AudioEffectDelayMemoryType8_t memory_type)
{
	uint32_t result = 0;
	uint32_t samples = 0;
	
	if (memory_type < AUDIO_MEMORY8_UNDEFINED) // This Never Happens...
	{
		AudioExtMem8** ppEM = &first[memory_type]; 
		
		do
		{
			uint32_t next_start;
			if (nullptr == *ppEM) // end of list, or first memory allocation
				next_start = memSizeSamples[memory_type];
			else // we've found an object using memory
			{
				AudioExtMem8* nextObj = (*ppEM)->next;
				result = (*ppEM)->memory_begin + (*ppEM)->memory_length; // end of object's allocation
				if (nullptr != nextObj)
					next_start = nextObj->memory_begin;
				else
					next_start = memSizeSamples[memory_type];
				
				ppEM = &((*ppEM)->next);
			}
			
			// if space is bigger, bump the answer
			if (samples <= (next_start - result))
				samples  = (next_start - result);
			
		} while (nullptr != *ppEM);
	}	
	
	return samples;
}


void AudioExtMem8::initialize(AudioEffectDelayMemoryType8_t type, uint32_t samples)
{
	//uint32_t memsize, avail;
	uint32_t avail;
	void* mem;
	
//#if defined(INTERNAL_TEST)
//	type = AUDIO_MEMORY8_INTERNAL;
//#endif // defined(INTERNAL_TEST)
	
	head_offset = 0;
	memory_type = type;
    
	//SPI.begin();
	//memsize = memSizeSamples[type];
	//Serial.printf("Requested %d samples\n",samples);
	
	switch (type)
	{
		case AUDIO_MEMORY8_PSRAM64:
		case AUDIO_MEMORY8_INTERNAL:
		case AUDIO_MEMORY8_HEAP:
		case AUDIO_MEMORY8_EXTMEM:
			break;
			
		default:
			samples = 0;
			break;
	}
	
#define noOLD_ALLOCATE	
#if defined(OLD_ALLOCATE)
	avail = memsize - allocated[type];
	if (avail < AUDIO_BLOCK_SAMPLES*2+1) {
		memory_type = AUDIO_MEMORY8_UNDEFINED;
		//return;
	}
	if (samples > avail) samples = avail;
	memory_begin = allocated[type];
	allocated[type] += samples;
#else
	switch (type)
	{
		// SPI memory
		// Emulate old behaviour: allocate biggest possible chunk
		// of delay memory if asked for more than is available.
		// Slightly different in dynamic system because of fragmentation,
		// but should be the same if used with legacy static design.
		case AUDIO_MEMORY8_PSRAM64:
		case AUDIO_MEMORY8_MEMORYBOARD:
			avail = findMaxSpace(type);
			if (samples > avail)
				samples = avail;
			//Serial.printf("findSpace says we could use %08lX\n",findSpace(type,samples));
			memory_begin = findSpace(type,samples);
			break;
			
		// processor heap: could be useful on Teensy 4.x etc.
		// In this case don't fill heap if asked for too much
		case AUDIO_MEMORY8_HEAP:
			mem = malloc(samples * SIZEOF_SAMPLE);
			if (nullptr != mem)
				memory_begin = (uint32_t) mem;
			else
				memory_begin = NOT_ENOUGH_MEMORY;
			break;
			
#if defined(ARDUINO_TEENSY41)
		// PSRAM external memory
		case AUDIO_MEMORY8_EXTMEM:
			mem = extmem_malloc(samples * SIZEOF_SAMPLE);
			if (nullptr != mem)
				memory_begin = (uint32_t) mem;
			else
				memory_begin = NOT_ENOUGH_MEMORY;
			break;
#endif // defined(ARDUINO_TEENSY41)
			
		default:  // invalid memory type
			memory_begin = NOT_ENOUGH_MEMORY;
			break;
	}
	if (NOT_ENOUGH_MEMORY == memory_begin)
		memory_type = AUDIO_MEMORY8_UNDEFINED;
#endif // defined(OLD_ALLOCATE)
	
	if (AUDIO_MEMORY8_UNDEFINED != memory_type)
	{
		memory_length = samples;
		zero(0, memory_length);
		linkIn();
	}
	else
		memory_length = 0;
}


#ifdef INTERNAL_TEST
static int16_t testmem[8000]; // testing only
#endif

void AudioExtMem8::read(uint32_t offset, uint32_t count, int16_t *data)
{
	uint32_t addr = memory_begin + offset;

#ifdef INTERNAL_TEST
	if (nullptr != data) while (count) { *data++ = testmem[addr++]; count--; } // testing only
#else
	switch (memory_type)
	{
		
        
		case AUDIO_MEMORY8_HEAP:
#if defined(ARDUINO_TEENSY41)
		case AUDIO_MEMORY8_EXTMEM:
#endif // defined(ARDUINO_TEENSY41)
			addr = memory_begin + offset*SIZEOF_SAMPLE;
			if (nullptr != data)
				memcpy(data,(void*) addr,count*SIZEOF_SAMPLE);
			break;
			
		default:
			break;
	}
#endif
}

void AudioExtMem8::write(uint32_t offset, uint32_t count, const int16_t *data)
{
	uint32_t addr = memory_begin + offset;

#ifdef INTERNAL_TEST
	while (count) { testmem[addr++] = *data++; count--; } // testing only
#else
	switch (memory_type)
	{
		
		case AUDIO_MEMORY8_HEAP:
#if defined(ARDUINO_TEENSY41)
		case AUDIO_MEMORY8_EXTMEM:
#endif // defined(ARDUINO_TEENSY41)
			addr = memory_begin + offset*SIZEOF_SAMPLE;
			if (nullptr != data)
				memcpy((void*) addr,data,count*SIZEOF_SAMPLE);
			else
				memset((void*) addr,0,count*SIZEOF_SAMPLE);
			break;	
			
		default:
			break;
	}
#endif
}
