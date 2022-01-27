/*
  See rights and use declaration in License.h

  based on Adafruit ili9341 library @ Dec 2016
  modified for the Maple Mini (STM32) by Steve Strong 2017 - https://github.com/stevstrong/Adafruit_ILI9486_STM32
  ,
  Note: SPI pins are set in Library .h file, rather than using the constructor

  modified for Teensy 3.1 by Richard Palmer 2017
  modified, stripped down and customized drawing functions by positionhigh@gmx.de 2021
*/
#include "ILI9486_Teensy.h"
uint16_t lineBuffer[1];

/*****************************************************************************/
// Constructor uses hardware SPI, the pins being specific to each device
/*****************************************************************************/
ILI9486_Teensy::ILI9486_Teensy(void) : Adafruit_GFX(TFT_WIDTH, TFT_HEIGHT) {}
/*****************************************************************************/
void writedata16(uint16_t c)
{
  CD_DATA;
  CS_ACTIVE;
  SPI1.transfer(c >> 8);
  SPI1.transfer(c & 0xFF);
  CS_IDLE;
}
/*****************************************************************************/
void writedata16(uint16_t c, uint32_t num)
{
  int counter = 0;
  CD_DATA;
  CS_ACTIVE;
  while (num-- > 0) // >= ??? RP RP RP
  {
    SPI1.transfer16(c);
    if (counter++ > SPIBLOCKMAX)
    { // transcations can be long - up to h * w * 16 bit words, so allow other SPI after a reasonable block
      SPI1.endTransaction();
      SPI1.beginTransaction(SPISET);
      counter = 0;
    }
  }
  CS_IDLE;
}
/*****************************************************************************/
void writecommand(uint8_t c)
{
  CD_COMMAND;
  CS_ACTIVE;
  SPI1.transfer(c);
  CS_IDLE;
}
/*****************************************************************************/
void writedata(uint8_t c)
{
  CD_DATA;
  CS_ACTIVE;
  SPI1.transfer(c);
  CS_IDLE;
}
/*****************************************************************************/
// https://github.com/adafruit/adafruit-rpi-fbtft/blob/35890c52f9e3eef3237b76acc295585dd93fc8cd/fb_ili9486.c
#define DELAY 0x80
/*****************************************************************************/
const uint8_t ili9486_init_sequence[] =
{
  //	2, 0xb0, 0x0,	// Interface Mode Control
  //	1, 0x11,		// Sleep OUT
  //	DELAY, 150,
  2, 0x3A, 0x55,	// use 16 bits per pixel color
  2, 0x36, 0x48,	// MX, BGR == rotation 0
  //	2, 0xC2, 0x44,	// Power Control 3
  // VCOM Control 1
  //	5, 0xC5, 0x00, 0x00, 0x00, 0x00,
  // PGAMCTRL(Positive Gamma Control)
  16, 0xE0, 0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98,
  0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00,
  // NGAMCTRL(Negative Gamma Control)
  16, 0xE1, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75,
  0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00,
  // Digital Gamma Control 1
  16, 0xE2, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75,
  0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00,
  1, 0x11,	// Sleep OUT
  DELAY, 150, 	// wait some time
  1, 0x29,	// Display ON
  0			// end marker
};
/*****************************************************************************/
// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
/*****************************************************************************/
void commandList(const uint8_t *addr)
{
  uint8_t  numBytes, tmp;
  while ( (numBytes = (*addr++)) > 0 ) { // end marker == 0
    if ( numBytes & DELAY ) {
      tmp = *addr++;
      delay(tmp); // up to 255 millis
    } else {
      tmp = *addr++;
      writecommand(tmp); // first byte is command
      while (--numBytes) { //   For each argument...
        tmp = *addr++;
        writedata(tmp); // all consecutive bytes are data
      }
    }
  }
}
/*****************************************************************************/
void ILI9486_Teensy::begin(void)
{
  //	ctrlRegs = TFT_CNTRL->regs;
  pinMode(TFT_RS, OUTPUT);
  CD_DATA;
  pinMode(TFT_CS, OUTPUT);
  CS_IDLE;

  // toggle RST low to reset
  if (TFT_RST > 0) {
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, HIGH);
    delay(20);
    digitalWrite(TFT_RST, LOW);
    delay(20);
    digitalWrite(TFT_RST, HIGH);
    delay(200);
  }
  SPI1.beginTransaction(SPISET); //SPISettings(36000000,MSBFIRST,MODE0))

  // init registers
  commandList(ili9486_init_sequence);
  SPI1.endTransaction();
}
/*****************************************************************************/
void ILI9486_Teensy::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  SPI1.beginTransaction(SPISET);
  writecommand(ILI9486_CASET); // Column addr set
  writedata(x0 >> 8);
  writedata(x0 & 0xFF);     // XSTART
  writedata(x1 >> 8);
  writedata(x1 & 0xFF);     // XEND
  writecommand(ILI9486_PASET); // Row addr set
  writedata(y0 >> 8);
  writedata(y0);     // YSTART
  writedata(y1 >> 8);
  writedata(y1);     // YEND
  writecommand(ILI9486_RAMWR); // write to RAM
  SPI1.endTransaction();
}
/*****************************************************************************/
void ILI9486_Teensy::pushColor(uint16_t color)
{
  SPI1.beginTransaction(SPISET);
  writedata16(color);
  SPI1.endTransaction();
}
/*****************************************************************************/
void ILI9486_Teensy::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

#ifdef REMOTE_CONSOLE
  //remote console
  Serial.write(99);
  Serial.write(90);
  Serial.write(highByte(x) );
  Serial.write(lowByte(x) );
  Serial.write(highByte(y));
  Serial.write(lowByte(y));
  Serial.write(highByte(color));
  Serial.write(lowByte(color));
  Serial.write(88);
   delayMicroseconds(20); //necessary to avoid random pixels in remote console
#endif

  setAddrWindow(x, y, x + 1, y + 1);
  pushColor(color);
}
/*****************************************************************************/
void ILI9486_Teensy::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height || h < 1)) return;
  if ((y + h - 1) >= _height)	{
    h = _height - y;
  }
  if (h < 2 ) {
    drawPixel(x, y, color);
    return;
  }
#ifdef REMOTE_CONSOLE
  //remote console
  Serial.write(99);
  Serial.write(91);
  Serial.write(highByte(x) );
  Serial.write(lowByte(x) );
  Serial.write(highByte(y));
  Serial.write(lowByte(y));
  Serial.write(highByte(h));
  Serial.write(lowByte(h));
  Serial.write(highByte(color));
  Serial.write(lowByte(color));
  Serial.write(88);
#endif
  setAddrWindow(x, y, x, y + h - 1);
  SPI1.beginTransaction(SPISET);
  writedata16(color, h);
  SPI1.endTransaction();
}
/*****************************************************************************/
void ILI9486_Teensy::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height || w < 1)) return;
  if ((x + w - 1) >= _width) {
    w = _width - x;
  }
  if (w < 2 ) {
    drawPixel(x, y, color);
    return;
  }
#ifdef REMOTE_CONSOLE
  //remote console
  Serial.write(99);
  Serial.write(92);
  Serial.write(highByte(x) );
  Serial.write(lowByte(x) );
  Serial.write(highByte(y));
  Serial.write(lowByte(y));
  Serial.write(highByte(w));
  Serial.write(lowByte(w));
  Serial.write(highByte(color));
  Serial.write(lowByte(color));
  Serial.write(88);
#endif
  setAddrWindow(x, y, x + w - 1, y);
  SPI1.beginTransaction(SPISET);

  writedata16(color, w);
  SPI1.endTransaction();
}
/*****************************************************************************/
void ILI9486_Teensy::fillScreen(uint16_t color)
{
#ifdef REMOTE_CONSOLE
  //remote console
  Serial.write(99);
  Serial.write(93);
  Serial.write(highByte(color));
  Serial.write(lowByte(color));
  Serial.write(88);
#endif
  setAddrWindow(0, 0,  _width, _height);
  SPI1.beginTransaction(SPISET);
  writedata16(color, (_width * _height));
  SPI1.endTransaction();
}
/*****************************************************************************/
void ILI9486_Teensy::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  // rudimentary clipping (drawChar w/big text requires this)
  if ((x >= _width) || (y >= _height || h < 1 || w < 1)) return;
  if ((x + w - 1) >= _width) {
    w = _width  - x;
  }
  if ((y + h - 1) >= _height) {
    h = _height - y;
  }
  if (w == 1 && h == 1) {
    drawPixel(x, y, color);
    return;
  }
#ifdef REMOTE_CONSOLE
  //remote console
  Serial.write(99);
  Serial.write(94);
  Serial.write(highByte(x) );
  Serial.write(lowByte(x) );
  Serial.write(highByte(y));
  Serial.write(lowByte(y));
  Serial.write(highByte(w));
  Serial.write(lowByte(w));
  Serial.write(highByte(h));
  Serial.write(lowByte(h));
  Serial.write(highByte(color));
  Serial.write(lowByte(color));
  Serial.write(88);
  delayMicroseconds(20); //necessary to avoid random pixels in remote console
#endif

  setAddrWindow(x, y, x + w - 1, y + h - 1);
  SPI1.beginTransaction(SPISET);

  writedata16(color, (w * h));
  SPI1.endTransaction();
}
/*
  Draw lines faster by calculating straight sections and drawing them with fastVline and fastHline.
*/
/*****************************************************************************/
void ILI9486_Teensy::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  if ((y0 < 0 && y1 < 0) || (y0 > _height && y1 > _height)) return;
  if ((x0 < 0 && x1 < 0) || (x0 > _width && x1 > _width)) return;
  if (x0 < 0) x0 = 0;
  if (x1 < 0) x1 = 0;
  if (y0 < 0) y0 = 0;
  if (y1 < 0) y1 = 0;
  if (y0 == y1) {
    if (x1 > x0) {
      drawFastHLine(x0, y0, x1 - x0 + 1, color);
    }
    else if (x1 < x0) {
      drawFastHLine(x1, y0, x0 - x1 + 1, color);
    }
    else {
      drawPixel(x0, y0, color);
    }
    return;
  }
  else if (x0 == x1) {
    if (y1 > y0) {
      drawFastVLine(x0, y0, y1 - y0 + 1, color);
    }
    else {
      drawFastVLine(x0, y1, y0 - y1 + 1, color);
    }
    return;
  }
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swapvalue(x0, y0);
    swapvalue(x1, y1);
  }
  if (x0 > x1) {
    swapvalue(x0, x1);
    swapvalue(y0, y1);
  }
  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);
  int16_t err = dx / 2;
  int16_t ystep;
  if (y0 < y1) {
    ystep = 1;
  }
  else {
    ystep = -1;
  }
  int16_t xbegin = x0;
  if (steep) {
    for (; x0 <= x1; x0++) {
      err -= dy;
      if (err < 0) {
        int16_t len = x0 - xbegin;
        if (len) {
          drawFastVLine (y0, xbegin, len + 1, color);
          //writeVLine_cont_noCS_noFill(y0, xbegin, len + 1);
        }
        else {
          drawPixel(y0, x0, color);
          //writePixel_cont_noCS(y0, x0, color);
        }
        xbegin = x0 + 1;
        y0 += ystep;
        err += dx;
      }
    }
    if (x0 > xbegin + 1) {
      //writeVLine_cont_noCS_noFill(y0, xbegin, x0 - xbegin);
      drawFastVLine(y0, xbegin, x0 - xbegin, color);
    }
  }
  else {
    for (; x0 <= x1; x0++) {
      err -= dy;
      if (err < 0) {
        int16_t len = x0 - xbegin;
        if (len) {
          drawFastHLine(xbegin, y0, len + 1, color);
          //writeHLine_cont_noCS_noFill(xbegin, y0, len + 1);
        }
        else {
          drawPixel(x0, y0, color);
          //writePixel_cont_noCS(x0, y0, color);
        }
        xbegin = x0 + 1;
        y0 += ystep;
        err += dx;
      }
    }
    if (x0 > xbegin + 1) {
      //writeHLine_cont_noCS_noFill(xbegin, y0, x0 - xbegin);
      drawFastHLine(xbegin, y0, x0 - xbegin, color);
    }
  }
}
/*****************************************************************************/
// Pass 8-bit (each) R,G,B, get back 16-bit packed color
/*****************************************************************************/
uint16_t ILI9486_Teensy::color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
/*****************************************************************************/
void ILI9486_Teensy::setRotation(uint8_t m)
{
  writecommand(ILI9486_MADCTL);
  rotation = m & 3; // can't be higher than 3
  switch (rotation) {
    case 0:
      writedata(MADCTL_MX | MADCTL_BGR);
      _width  = TFT_WIDTH;
      _height = TFT_HEIGHT;
      break;
    case 1:
      writedata(MADCTL_MV | MADCTL_BGR);
      _width  = TFT_HEIGHT;
      _height = TFT_WIDTH;
      break;
    case 2:
      writedata(MADCTL_MY | MADCTL_BGR);
      _width  = TFT_WIDTH;
      _height = TFT_HEIGHT;
      break;
    case 3:
      writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      _width  = TFT_HEIGHT;
      _height = TFT_WIDTH;
      break;
  }
}
/*****************************************************************************/
