//***************************************************
// https://github.com/kurte/ILI9341_t3n
// http://forum.pjrc.com/threads/26305-Highly-optimized-ILI9341-(320x240-TFT-color-display)-library
//
// Warning this is Kurt's updated version which allows it to work on different SPI busses.
//
// On Teensy 4.x including Micromod you are free to use any digital pin for
// CS and DC, but you might get a modest speed increase if hardware CS is
// used for the DC pin
//
/***************************************************
  This is our library for the Adafruit  ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

/* ILI9341_t3DMA library code is placed under the MIT license
   Copyright (c) 2016 Frank Bösing
*/

#ifndef _ILI9341_t3NH_
#define _ILI9341_t3NH_

#define ILI9341_USE_DMAMEM

// Allow way to override using SPI

#ifdef __cplusplus
#include "Arduino.h"
#include <DMAChannel.h>
#include <SPI.h>
#include "config.h"

#endif
#include <stdint.h>

#define ILI9341_NOP 0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID 0x04
#define ILI9341_RDDST 0x09

#define ILI9341_SLPIN 0x10
#define ILI9341_SLPOUT 0x11
#define ILI9341_PTLON 0x12
#define ILI9341_NORON 0x13

#define ILI9341_RDMODE 0x0A
#define ILI9341_RDMADCTL 0x0B
#define ILI9341_RDPIXFMT 0x0C
#define ILI9341_RDIMGFMT 0x0D
#define ILI9341_RDSELFDIAG 0x0F

#define ILI9341_INVOFF 0x20
#define ILI9341_INVON 0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON 0x29

#define ILI9341_CASET 0x2A
#define ILI9341_PASET 0x2B
#define ILI9341_RAMWR 0x2C
#define ILI9341_RAMRD 0x2E

#define ILI9341_PTLAR 0x30
#define ILI9341_VSCRDEF 0x33
#define ILI9341_MADCTL 0x36
#define ILI9341_VSCRSADD 0x37
#define ILI9341_PIXFMT 0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR 0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1 0xC0
#define ILI9341_PWCTR2 0xC1
#define ILI9341_PWCTR3 0xC2
#define ILI9341_PWCTR4 0xC3
#define ILI9341_PWCTR5 0xC4
#define ILI9341_VMCTR1 0xC5
#define ILI9341_VMCTR2 0xC7

#define ILI9341_RDID1 0xDA
#define ILI9341_RDID2 0xDB
#define ILI9341_RDID3 0xDC
#define ILI9341_RDID4 0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

#define CL(_r, _g, _b) ((((_r)&0xF8) << 8) | (((_g)&0xFC) << 3) | ((_b) >> 3))

#define sint16_t int16_t

#ifdef __cplusplus
// At all other speeds, _pspi->beginTransaction() will use the fastest available
// clock

#define ILI9341_SPICLOCK 60000000
#define ILI9341_SPICLOCK_READ 2000000

class ILI9341_t3n : public Print {
public:
  ILI9341_t3n(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI = 11,
              uint8_t _SCLK = 13, uint8_t _MISO = 12);
  void begin(uint32_t spi_clock = ILI9341_SPICLOCK,
             uint32_t spi_clock_read = ILI9341_SPICLOCK_READ);
  void sleep(bool enable);
  void pushColor(uint16_t color);
  void fillScreen(uint16_t color);
  inline void fillWindow(uint16_t color) {
    fillScreen(color);
  }
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

  void setRotation(uint8_t r);
  void invertDisplay(boolean i);
  void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  // Pass 8-bit (each) R,G,B, get back 16-bit packed color
  static uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  }


  uint8_t readcommand8(uint8_t reg, uint8_t index = 0);
  uint16_t readScanLine();
  void setFrameRateControl(uint8_t mode);


  void writeRect(int16_t x, int16_t y, int16_t w, int16_t h,
                 const uint16_t *pcolors);

  void writeSubImageRect(int16_t x, int16_t y, int16_t w, int16_t h,
                         int16_t image_offset_x, int16_t image_offset_y, int16_t image_width, int16_t image_height,
                         const uint16_t *pcolors);
  void writeSubImageRectBytesReversed(int16_t x, int16_t y, int16_t w, int16_t h,
                                      int16_t image_offset_x, int16_t image_offset_y, int16_t image_width, int16_t image_height,
                                      const uint16_t *pcolors);

  // writeRect1BPP - 	write 1 bit per pixel paletted bitmap
  //					bitmap data in array at pixels, 4 bits per
  //
  //					color palette data in array at palette
  //					width must be at least 8 pixels
  void writeRect1BPP(int16_t x, int16_t y, int16_t w, int16_t h,
                     const uint8_t *pixels, const uint16_t *palette);

  // writeRectNBPP - 	write N(1, 2, 4, 8) bit per pixel paletted bitmap
  //					bitmap data in array at pixels
  //  Currently writeRect1BPP, writeRect2BPP, writeRect4BPP use this to do all
  //  of the work.
  //
  void writeRectNBPP(int16_t x, int16_t y, int16_t w, int16_t h,
                     uint8_t bits_per_pixel, const uint8_t *pixels,
                     const uint16_t *palette);

  // from Adafruit_GFX.h

  void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                        uint16_t color);
  void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                        int16_t delta, uint16_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w,
                  int16_t h, uint16_t color);
  void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
                uint16_t bg, uint8_t size_x, uint8_t size_y);
  void inline drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
                       uint16_t bg, uint8_t size) {
    drawChar(x, y, c, color, bg, size);
  }
  static const int16_t CENTER = 9998;
  void setCursor(int16_t x, int16_t y, bool autoCenter = false);
  void getCursor(int16_t *x, int16_t *y);
  void setTextColor(uint16_t c);
  void setTextColor(uint16_t c, uint16_t bg);
  void setTextSize(uint8_t sx, uint8_t sy);
  void inline setTextSize(uint8_t s) {
    setTextSize(s, s);
  }
  uint8_t getTextSizeX();
  uint8_t getTextSizeY();
  uint8_t getTextSize();

  bool console;

  // setOrigin sets an offset in display pixels where drawing to (0,0) will
  // appear
  // for example: setOrigin(10,10); drawPixel(5,5); will cause a pixel to be
  // drawn at hardware pixel (15,15)
  void setOrigin(int16_t x = 0, int16_t y = 0) {
    _originx = x;
    _originy = y;
    // if (Serial) Serial.printf_P(PSTR("Set Origin %d %d\n"), x, y);
    updateDisplayClip();
  }
  void getOrigin(int16_t *x, int16_t *y) {
    *x = _originx;
    *y = _originy;
  }

  // setClipRect() sets a clipping rectangle (relative to any set origin) for
  // drawing to be limited to.
  // Drawing is also restricted to the bounds of the display

  void setClipRect(int16_t x1, int16_t y1, int16_t w, int16_t h) {
    _clipx1 = x1;
    _clipy1 = y1;
    _clipx2 = x1 + w;
    _clipy2 = y1 + h;
    // if (Serial) Serial.printf_P(PSTR("Set clip Rect %d %d %d %d\n"), x1, y1, w, h);
    updateDisplayClip();
  }
  void setClipRect() {
    _clipx1 = 0;
    _clipy1 = 0;
    _clipx2 = _width;
    _clipy2 = _height;
    // if (Serial) Serial.printf_P(PSTR("clear clip Rect\n"));
    updateDisplayClip();
  }

  // overwrite print functions:
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buffer, size_t size);

  int16_t width(void) {
    return _width;
  }
  int16_t height(void) {
    return _height;
  }
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  int16_t getCursorX(void) const {
    return cursor_x;
  }
  int16_t getCursorY(void) const {
    return cursor_y;
  }

  void drawFontChar(unsigned int c);
  void drawGFXFontChar(unsigned int c);

  //    void getTextBounds(const uint8_t *buffer, uint16_t len, int16_t x, int16_t y,
  //                       int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  //    void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1,
  //                       int16_t *y1, uint16_t *w, uint16_t *h);
  //    void getTextBounds(const String &str, int16_t x, int16_t y, int16_t *x1,
  //                       int16_t *y1, uint16_t *w, uint16_t *h);
  //int16_t strPixelLen(const char *str, uint16_t cb = 0xffff); // optional number of characters...

  // added support for drawing strings/numbers/floats with centering
  // modified from tft_ili9341_ESP github library

  // Handle char arrays
  int16_t drawString(const String &string, int poX, int poY);
  int16_t drawString(const char string[], int16_t len, int poX, int poY);


protected:
  SPIClass *_pspi = nullptr;
  SPIClass::SPI_Hardware_t *_spi_hardware;

  uint8_t _spi_num;          // Which buss is this spi on?
  uint32_t _SPI_CLOCK;       // #define ILI9341_SPICLOCK 30000000
  uint32_t _SPI_CLOCK_READ;  //#define ILI9341_SPICLOCK_READ 2000000

  IMXRT_LPSPI_t *_pimxrt_spi;

  int16_t _width, _height;  // Display w/h as modified by current rotation
  int16_t cursor_x, cursor_y;
  bool _center_x_text = false;
  bool _center_y_text = false;
  int16_t _clipx1, _clipy1, _clipx2, _clipy2;
  int16_t _originx, _originy;
  int16_t _displayclipx1, _displayclipy1, _displayclipx2, _displayclipy2;
  bool _invisible = false;
  bool _standard = true;  // no bounding rectangle or origin set.

  inline void updateDisplayClip() {
    _displayclipx1 = max(0, min(_clipx1 + _originx, width()));
    _displayclipx2 = max(0, min(_clipx2 + _originx, width()));

    _displayclipy1 = max(0, min(_clipy1 + _originy, height()));
    _displayclipy2 = max(0, min(_clipy2 + _originy, height()));
    _invisible =
      (_displayclipx1 == _displayclipx2 || _displayclipy1 == _displayclipy2);
    _standard = (_displayclipx1 == 0) && (_displayclipx2 == _width) && (_displayclipy1 == 0) && (_displayclipy2 == _height);
  }

  uint16_t textcolor, textbgcolor;
  uint32_t textcolorPrexpanded, textbgcolorPrexpanded;
  uint8_t textsize_x, textsize_y, rotation;

  uint32_t padX;

  uint8_t _rst;
  uint8_t _cs, _dc;
  uint8_t pcs_data, pcs_command;
  uint8_t _miso, _mosi, _sclk;

  ///////////////////////////////


#if defined(__IMXRT1052__) || defined(__IMXRT1062__)
  uint8_t pending_rx_count = 0;  // hack ...
  void waitFifoNotFull(void);
  void waitFifoEmpty(void);
  void waitTransmitComplete(void);
  uint16_t waitTransmitCompleteReturnLast();
  void waitTransmitComplete(uint32_t mcr);

#endif
  //////////////////////////////

  // add support to allow only one hardware CS (used for dc)
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
  uint32_t _cspinmask;
  volatile uint32_t *_csport;
  uint32_t _spi_tcr_current;
  uint32_t _dcpinmask;
  uint32_t _tcr_dc_assert;
  uint32_t _tcr_dc_not_assert;
  volatile uint32_t *_dcport;

#endif

  void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny,
                  int16_t *maxx, int16_t *maxy);

  void setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
    __attribute__((always_inline)) {
    writecommand_cont(ILI9341_CASET);  // Column addr set
    writedata16_cont(x0);              // XSTART
    writedata16_cont(x1);              // XEND
    writecommand_cont(ILI9341_PASET);  // Row addr set
    writedata16_cont(y0);              // YSTART
    writedata16_cont(y1);              // YEND
  }
  //. From Onewire utility files
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x

  void DIRECT_WRITE_LOW(volatile uint32_t *base, uint32_t mask)
    __attribute__((always_inline)) {
    *(base + 34) = mask;
  }
  void DIRECT_WRITE_HIGH(volatile uint32_t *base, uint32_t mask)
    __attribute__((always_inline)) {
    *(base + 33) = mask;
  }
#endif

  void beginSPITransaction(uint32_t clock) __attribute__((always_inline)) {
    _pspi->beginTransaction(SPISettings(clock, MSBFIRST, SPI_MODE0));
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
    if (!_dcport)
      _spi_tcr_current = _pimxrt_spi->TCR;  // Only if DC is on hardware CS
#endif
    if (_csport) {
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
      DIRECT_WRITE_LOW(_csport, _cspinmask);
#else
      *_csport &= ~_cspinmask;
#endif
    }
  }
  void endSPITransaction() __attribute__((always_inline)) {
    if (_csport) {
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
      DIRECT_WRITE_HIGH(_csport, _cspinmask);
#else
      *_csport |= _cspinmask;
#endif
    }
    _pspi->endTransaction();
  }

#if defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
#define TCR_MASK \
  (LPSPI_TCR_PCS(3) | LPSPI_TCR_FRAMESZ(31) | LPSPI_TCR_CONT | LPSPI_TCR_RXMSK)
  void maybeUpdateTCR(
    uint32_t requested_tcr_state) { /*__attribute__((always_inline)) */
    if ((_spi_tcr_current & TCR_MASK) != requested_tcr_state) {
      bool dc_state_change = (_spi_tcr_current & LPSPI_TCR_PCS(3)) != (requested_tcr_state & LPSPI_TCR_PCS(3));
      _spi_tcr_current = (_spi_tcr_current & ~TCR_MASK) | requested_tcr_state;
      // only output when Transfer queue is empty.
      if (!dc_state_change || !_dcpinmask) {
        while ((_pimxrt_spi->FSR & 0x1f))
          ;
        _pimxrt_spi->TCR = _spi_tcr_current;  // update the TCR

      } else {
        waitTransmitComplete();
        if (requested_tcr_state & LPSPI_TCR_PCS(3))
          DIRECT_WRITE_HIGH(_dcport, _dcpinmask);
        else
          DIRECT_WRITE_LOW(_dcport, _dcpinmask);
        _pimxrt_spi->TCR = _spi_tcr_current & ~(LPSPI_TCR_PCS(3) | LPSPI_TCR_CONT);  // go ahead and update TCR anyway?
      }
    }
  }

  // BUGBUG:: currently assumming we only have CS_0 as valid CS
  void writecommand_cont(uint8_t c) __attribute__((always_inline)) {
    maybeUpdateTCR(_tcr_dc_assert | LPSPI_TCR_FRAMESZ(7) /*| LPSPI_TCR_CONT*/);
    _pimxrt_spi->TDR = c;
    pending_rx_count++;  //
    waitFifoNotFull();
  }
  void writedata8_cont(uint8_t c) __attribute__((always_inline)) {
    maybeUpdateTCR(_tcr_dc_not_assert | LPSPI_TCR_FRAMESZ(7) | LPSPI_TCR_CONT);
    _pimxrt_spi->TDR = c;
    pending_rx_count++;  //
    waitFifoNotFull();
  }
  void writedata16_cont(uint16_t d) __attribute__((always_inline)) {
    maybeUpdateTCR(_tcr_dc_not_assert | LPSPI_TCR_FRAMESZ(15) | LPSPI_TCR_CONT);
    _pimxrt_spi->TDR = d;
    pending_rx_count++;  //
    waitFifoNotFull();
  }
  void writecommand_last(uint8_t c) __attribute__((always_inline)) {
    maybeUpdateTCR(_tcr_dc_assert | LPSPI_TCR_FRAMESZ(7));
    _pimxrt_spi->TDR = c;
    //		_pimxrt_spi->SR = LPSPI_SR_WCF | LPSPI_SR_FCF | LPSPI_SR_TCF;
    pending_rx_count++;  //
    waitTransmitComplete();
  }
  void writedata8_last(uint8_t c) __attribute__((always_inline)) {
    maybeUpdateTCR(_tcr_dc_not_assert | LPSPI_TCR_FRAMESZ(7));
    _pimxrt_spi->TDR = c;
    //		_pimxrt_spi->SR = LPSPI_SR_WCF | LPSPI_SR_FCF | LPSPI_SR_TCF;
    pending_rx_count++;  //
    waitTransmitComplete();
  }
  void writedata16_last(uint16_t d) __attribute__((always_inline)) {
    maybeUpdateTCR(_tcr_dc_not_assert | LPSPI_TCR_FRAMESZ(15));
    _pimxrt_spi->TDR = d;
    //		_pimxrt_spi->SR = LPSPI_SR_WCF | LPSPI_SR_FCF | LPSPI_SR_TCF;
    pending_rx_count++;  //
    waitTransmitComplete();
  }

#endif
};

#ifndef ILI9341_swap
#define ILI9341_swap(a, b) \
  { \
    typeof(a) t = a; \
    a = b; \
    b = t; \
  }
#endif
#endif  // __cplusplus
#endif