#include "scope.h"
#include "ILI9341_t3n.h"
#include "sequencer.h"
extern ILI9341_t3n display;
extern sequencer_t seq;

FLASHMEM void Realtime_Scope::FillArray()
{
  __disable_irq();
  uint16_t i = 0;
  do
  {
    int16_t wave_data = buffer[i];
    int8_t y = map(wave_data, 32767, -32768, 20, -20) + 32;
    scopebuffer[i] = y;
    i = i + 1;
  } while (i < (AUDIO_BLOCK_SAMPLES));
  __enable_irq();
}

FLASHMEM void Realtime_Scope::AddtoBuffer(int16_t *audio)
{
  const int16_t *end = audio + AUDIO_BLOCK_SAMPLES;
  __disable_irq();
  do
  {
    buffer[count++] = *audio;
    audio++;
  } while (audio < end);
  if (count > (AUDIO_BLOCK_SAMPLES)-1)
  {
    count = 0;
  }
  __enable_irq();
  if (scope_is_drawing == false)
    FillArray();
}

FLASHMEM void Realtime_Scope::update(void)
{
  audio_block_t *block;
  block = receiveReadOnly(0);
  if (block)
  {
    AddtoBuffer(block->data);
    release(block);
  }
}

FLASHMEM void Realtime_Scope::clear(void)
{
  for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
  {
    scopebuffer_old[i] = 32;
  }
}

FLASHMEM void Realtime_Scope::draw_scope(uint16_t x, int y, uint8_t w)
{
  if (scope_delay > 84)
  {
    uint16_t i = 0;
    scope_is_drawing = true;
    //display.console = false;
     display.console = true;
    do
    {
      if (scopebuffer_old[i] != scopebuffer[i])
      {
        display.drawPixel(x + i, scopebuffer_old[i] + y, COLOR_BACKGROUND);
        display.drawPixel(x + i, scopebuffer[i] + y, COLOR_SYSTEXT);
      }
      scopebuffer_old[i] = scopebuffer[i];
      i = i + 1;

    } while (i < w);
    scope_delay = 0;
    scope_is_drawing = false;
  }
  scope_delay++;
}