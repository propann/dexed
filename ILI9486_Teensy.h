/*
  See rights and use declaration in License.h
  based on Adafruit ili9341 library @ Dec 2016
  modified for the Maple Mini by Steve Strong 2017
  modified for Teensy by Richard Palmer 2017
  modified for microdexed on Teensy 4.x by positionhigh@gmx.de 2021
*/
#ifndef _ILI9486H_Teensy
#define _ILI9486H_Teensy
#include "Adafruit_GFX.h"
#include <SPI.h>

#define SPISET SPISettings(36000000,MSBFIRST,SPI_MODE0)  //DEFAULT 36000000
//#define SPIBLOCKMAX 320 // one ROW is a good value to avoid really long SPI transfers
#define SPIBLOCKMAX TFT_HEIGHT*8 // one ROW is a good value to avoid really long SPI transfers

#define ILI9486_INVOFF	0x20
#define ILI9486_INVON	0x21
#define ILI9486_CASET	0x2A
#define ILI9486_PASET	0x2B
#define ILI9486_RAMWR	0x2C
#define ILI9486_MADCTL	0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

/*
  Define pins and Output Data Registers
*/
//Control pins |RS |CS |RST|
//#define TFT_CNTRL      GPIOA

#define TFT_CS_MASK    BIT4
#define TFT_RS_MASK    BIT0
#define CS_ACTIVE    digitalWrite(TFT_CS, LOW);
#define CS_IDLE      digitalWrite(TFT_CS, HIGH);
#define CD_COMMAND   digitalWrite(TFT_RS, LOW);
#define CD_DATA     digitalWrite(TFT_RS, HIGH);

    
#define swapvalue(a, b) { int16_t t = a; a = b; b = t; }
/*****************************************************************************/
class ILI9486_Teensy : public Adafruit_GFX
{
  public:
    ILI9486_Teensy(void);
    void	begin(void),
          setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1),
          pushColor(uint16_t color),
          fillScreen(uint16_t color),
          drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
          drawPixel(int16_t x, int16_t y, uint16_t color),
          drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
          drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
          fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
          setRotation(uint8_t r),
          invertDisplay(boolean i);
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
    void reset(void);
  private:
    uint8_t	tabcolor;
};
void	writecommand(uint8_t c),
      writedata(uint8_t d),
      writedata16(uint16_t d),
      writedata16(uint16_t d, uint32_t num),
      commandList(uint8_t *addr);

#endif //endif of the header file
