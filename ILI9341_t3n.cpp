// https://github.com/KurtE/ILI9341_t3n
// http://forum.pjrc.com/threads/26305-Highly-optimized-ILI9341-(320x240-TFT-color-display)-library

/***************************************************
  This is our library for the Adafruit ILI9341 Breakout and Shield
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

#include "ILI9341_t3n.h"
#include <SPI.h>

// 5x7 font
PROGMEM const unsigned char font[] = {
  0x00, 0x00, 0x00, 0x00, 0x00,  //
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E,  //
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E,  //
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C,  //
  0x18, 0x3C, 0x7E, 0x3C, 0x18,  //
  0x1C, 0x57, 0x7D, 0x57, 0x1C,  //
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C,  //
  0x00, 0x18, 0x3C, 0x18, 0x00,  //
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF,  //
  0x00, 0x18, 0x24, 0x18, 0x00,  //
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF,  //
  0x30, 0x48, 0x3A, 0x06, 0x0E,  //
  0x26, 0x29, 0x79, 0x29, 0x26,  //
  0x40, 0x7F, 0x05, 0x05, 0x07,  //
  0x40, 0x7F, 0x05, 0x25, 0x3F,  //
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A,  //
  0x7F, 0x3E, 0x1C, 0x1C, 0x08,  //
  0x08, 0x1C, 0x1C, 0x3E, 0x7F,  //
  0x14, 0x22, 0x7F, 0x22, 0x14,  //
  0x5F, 0x5F, 0x00, 0x5F, 0x5F,  //
  0x06, 0x09, 0x7F, 0x01, 0x7F,  //
  0x00, 0x66, 0x89, 0x95, 0x6A,  //
  0x60, 0x60, 0x60, 0x60, 0x60,  //
  0x94, 0xA2, 0xFF, 0xA2, 0x94,  //
  0x08, 0x04, 0x7E, 0x04, 0x08,  //
  0x10, 0x20, 0x7E, 0x20, 0x10,  //
  0x08, 0x08, 0x2A, 0x1C, 0x08,  //
  0x08, 0x1C, 0x2A, 0x08, 0x08,  //
  0x1E, 0x10, 0x10, 0x10, 0x10,  //
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C,  //
  0x30, 0x38, 0x3E, 0x38, 0x30,  //
  0x06, 0x0E, 0x3E, 0x0E, 0x06,  //
  0x00, 0x00, 0x00, 0x00, 0x00,  //
  0x00, 0x00, 0x5F, 0x00, 0x00,  // !
  0x00, 0x07, 0x00, 0x07, 0x00,  // "
  0x14, 0x7F, 0x14, 0x7F, 0x14,  // #
  0x24, 0x2A, 0x7F, 0x2A, 0x12,  // $
  0x23, 0x13, 0x08, 0x64, 0x62,  // %
  0x36, 0x49, 0x55, 0x22, 0x50,  // &
  0x00, 0x05, 0x03, 0x00, 0x00,  // '
  0x00, 0x1C, 0x22, 0x41, 0x00,  // (
  0x00, 0x41, 0x22, 0x1C, 0x00,  // )
  0x08, 0x2A, 0x1C, 0x2A, 0x08,  // *
  0x08, 0x08, 0x3E, 0x08, 0x08,  // +
  0x00, 0x50, 0x30, 0x00, 0x00,  // ,
  0x08, 0x08, 0x08, 0x08, 0x08,  // -
  0x00, 0x60, 0x60, 0x00, 0x00,  // .
  0x20, 0x10, 0x08, 0x04, 0x02,  // /
  0x7F, 0x51, 0x49, 0x45, 0x7F,  // 0
  0x40, 0x41, 0x7F, 0x40, 0x40,  // 1
  0x79, 0x49, 0x49, 0x49, 0x4F,  // 2
  0x49, 0x49, 0x49, 0x49, 0x7F,  // 3
  0x0F, 0x08, 0x08, 0x08, 0x7F,  // 4
  0x4F, 0x49, 0x49, 0x49, 0x79,  // 5
  0x7F, 0x49, 0x49, 0x49, 0x79,  // 6
  0x01, 0x01, 0x71, 0x09, 0x07,  // 7
  0x7F, 0x49, 0x49, 0x49, 0x7F,  // 8
  0x0F, 0x09, 0x09, 0x09, 0x7F,  // 9
  0x00, 0x36, 0x36, 0x00, 0x00,  // :
  0x00, 0x56, 0x36, 0x00, 0x00,  // ;
  0x00, 0x08, 0x14, 0x22, 0x41,  // <
  0x14, 0x14, 0x14, 0x14, 0x14,  // =
  0x41, 0x22, 0x14, 0x08, 0x00,  // >
  0x02, 0x01, 0x51, 0x09, 0x06,  // ?
  0x32, 0x49, 0x79, 0x41, 0x3E,  // @  //64
  0x7E, 0x09, 0x09, 0x09, 0x7E,  // A
  0x7F, 0x49, 0x49, 0x49, 0x36,  // B
  0x3E, 0x41, 0x41, 0x41, 0x22,  // C
  0x7F, 0x41, 0x41, 0x41, 0x3E,  // D
  0x7F, 0x49, 0x49, 0x49, 0x41,  // E
  0x7F, 0x09, 0x09, 0x01, 0x01,  // F
  0x3E, 0x41, 0x41, 0x51, 0x32,  // G
  0x7F, 0x08, 0x08, 0x08, 0x7F,  // H
  0x41, 0x41, 0x7F, 0x41, 0x41,  // I
  0x20, 0x40, 0x41, 0x3F, 0x01,  // J
  0x7F, 0x08, 0x14, 0x22, 0x41,  // K
  0x7F, 0x40, 0x40, 0x40, 0x40,  // L
  0x7F, 0x02, 0x04, 0x02, 0x7F,  // M
  0x7F, 0x02, 0x04, 0x08, 0x7F,  // N
  0x3E, 0x41, 0x41, 0x41, 0x3E,  // O
  0x7F, 0x09, 0x09, 0x09, 0x06,  // P
  0x3E, 0x41, 0x51, 0x21, 0x5E,  // Q
  0x7F, 0x09, 0x09, 0x19, 0x66,  // R
  0x46, 0x49, 0x49, 0x49, 0x31,  // S
  0x01, 0x01, 0x7F, 0x01, 0x01,  // T
  0x3F, 0x40, 0x40, 0x40, 0x3F,  // U
  0x1F, 0x20, 0x40, 0x20, 0x1F,  // V
  0x7F, 0x20, 0x18, 0x20, 0x7F,  // W
  0x63, 0x14, 0x08, 0x14, 0x63,  // X
  0x47, 0x48, 0x48, 0x48, 0x3F,  // Y
  0x61, 0x51, 0x49, 0x45, 0x43,  // Z
  0x00, 0x00, 0x7F, 0x41, 0x41,  // [
  0x02, 0x04, 0x08, 0x10, 0x20,  // "\"
  0x41, 0x41, 0x7F, 0x00, 0x00,  // ]
  0x04, 0x02, 0x01, 0x02, 0x04,  // ^
  0x40, 0x40, 0x40, 0x40, 0x40,  // _
  0x00, 0x01, 0x02, 0x04, 0x00,  // `
  0x38, 0x44, 0x44, 0x44, 0x7C,  // a
  0x7E, 0x44, 0x44, 0x44, 0x38,  // b
  0x38, 0x44, 0x44, 0x44, 0x44,  // c
  0x38, 0x44, 0x44, 0x44, 0x7E,  // d
  0x38, 0x54, 0x54, 0x54, 0x58,  // e
  0x10, 0x7C, 0x12, 0x12, 0x12,  // f
  0x1C, 0x54, 0x54, 0x54, 0x3C,  // g
  0x7E, 0x04, 0x04, 0x04, 0x78,  // h
  0x00, 0x44, 0x7D, 0x40, 0x00,  // i
  0x20, 0x40, 0x44, 0x3D, 0x00,  // j
  0x00, 0x7F, 0x10, 0x28, 0x44,  // k
  0x3C, 0x40, 0x40, 0x40, 0x40,  // l
  0x7C, 0x04, 0x78, 0x04, 0x7C,  // m
  0x7C, 0x04, 0x04, 0x04, 0x78,  // n
  0x38, 0x44, 0x44, 0x44, 0x38,  // o
  0x7C, 0x24, 0x24, 0x24, 0x1C,  // p
  0x38, 0x24, 0x24, 0x24, 0x7C,  // q
  0x78, 0x04, 0x04, 0x04, 0x04,  // r
  0x58, 0x54, 0x54, 0x54, 0x34,  // s
  0x04, 0x3E, 0x44, 0x44, 0x44,  // t
  0x3C, 0x40, 0x40, 0x40, 0x3C,  // u
  0x1C, 0x20, 0x40, 0x20, 0x1C,  // v
  0x3C, 0x40, 0x30, 0x40, 0x3C,  // w
  0x44, 0x28, 0x10, 0x28, 0x44,  // x
  0x4C, 0x50, 0x50, 0x50, 0x3C,  // y
  0x44, 0x64, 0x54, 0x4C, 0x44,  // z
  0x00, 0x08, 0x36, 0x41, 0x00,  // {
  0x00, 0x00, 0x7F, 0x00, 0x00,  // |
  0x00, 0x41, 0x36, 0x08, 0x00,  // }
  0x08, 0x08, 0x2A, 0x1C, 0x08,  // ->
  0x08, 0x1C, 0x2A, 0x08, 0x08,  // <-
  0x1E, 0xA1, 0xA1, 0x61, 0x12,
  0x3A, 0x40, 0x40, 0x20, 0x7A,
  0x38, 0x54, 0x54, 0x55, 0x59,
  0x21, 0x55, 0x55, 0x79, 0x41,
  0x22, 0x54, 0x54, 0x78, 0x42,  // a-umlaut
  0x21, 0x55, 0x54, 0x78, 0x40,
  0x20, 0x54, 0x55, 0x79, 0x40,
  0x0C, 0x1E, 0x52, 0x72, 0x12,
  0x40, 0x30, 0x0c, 0x02, 0x7F,  // BRAIDS Wave 1 SAW  // 0x39, 0x55, 0x55, 0x55, 0x59,  // note: grid is vertical slices
  0x10, 0x08, 0x04, 0x1e, 0x00,  // BRAIDS Centered SAW
  0x40, 0x20, 0x10, 0x08, 0x7C,  // BRAIDS BABY SAW
  0x60, 0x1C, 0x03, 0x1C, 0x60,  // BRAIDS TRIANGLE
  0x7f, 0x01, 0x01, 0x01, 0x7f,  // BRAIDS SQUARE //160
  0x78, 0x08, 0x08, 0x08, 0x78,  // BRAIDS BABY SQUARE
  0x40, 0x40, 0x7F, 0x01, 0x7F,  // BRAIDS PULSE  0x7D, 0x12, 0x11, 0x12, 0x7D A-umlaut
  0xF0, 0x28, 0x25, 0x28, 0xF0,
  0x3e, 0x08, 0x00, 0x08, 0x00,  // Sample Play Mode : Trigger Mode. part1 //144
  0x08, 0x00, 0x3E, 0x1C, 0x08,  // Sample Play Mode : Trigger Mode. part2 //145
  0x3e, 0x08, 0x08, 0x08, 0x08,  // Sample Play Mode : Hold Mode part1
  0x08, 0x08, 0x08, 0x08, 0x3e,  // Sample Play Mode : Hold Mode part2
  0x3A, 0x44, 0x44, 0x44, 0x3A,  // o-umlaut
  0x32, 0x4A, 0x48, 0x48, 0x30,
  0x3A, 0x41, 0x41, 0x21, 0x7A,  //150
  0x3A, 0x42, 0x40, 0x20, 0x78,
  0x00, 0x9D, 0xA0, 0xA0, 0x7D,
  0x3D, 0x42, 0x42, 0x42, 0x3D,  // O-umlaut
  0x3D, 0x40, 0x40, 0x40, 0x3D,
  0x3C, 0x24, 0xFF, 0x24, 0x24,
  0x48, 0x7E, 0x49, 0x43, 0x66,
  0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
  0xFF, 0x09, 0x29, 0xF6, 0x20,
  0xC0, 0x88, 0x7E, 0x09, 0x03,
  0x20, 0x54, 0x54, 0x79, 0x41,  //160
  0x00, 0x00, 0x44, 0x7D, 0x41,
  0x30, 0x48, 0x48, 0x4A, 0x32,
  0x38, 0x40, 0x40, 0x22, 0x7A,
  0x00, 0x7A, 0x0A, 0x0A, 0x72,
  0x7D, 0x0D, 0x19, 0x31, 0x7D,
  0x26, 0x29, 0x29, 0x2F, 0x28,
  0x26, 0x29, 0x29, 0x29, 0x26,
  0x30, 0x48, 0x4D, 0x40, 0x20,
  0x38, 0x08, 0x08, 0x08, 0x08,
  0x08, 0x08, 0x08, 0x08, 0x38,  //170
  0x2F, 0x10, 0xC8, 0xAC, 0xBA,
  0x2F, 0x10, 0x28, 0x34, 0xFA,
  0x00, 0x00, 0x7B, 0x00, 0x00,
  0x08, 0x14, 0x2A, 0x14, 0x22,
  0x22, 0x14, 0x2A, 0x14, 0x08,
  0x55, 0x00, 0x55, 0x00, 0x55,  // #176 (25% block) missing in old // code
  0xAA, 0x55, 0xAA, 0x55, 0xAA,  // 50% block
  0xFF, 0x55, 0xFF, 0x55, 0xFF,  // 75% block
  0x00, 0x00, 0x00, 0xFF, 0x00,
  0x10, 0x10, 0x10, 0xFF, 0x00,
  0x14, 0x14, 0x14, 0xFF, 0x00,
  0x10, 0x10, 0xFF, 0x00, 0xFF,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x14, 0x14, 0x14, 0xFC, 0x00,
  0x14, 0x14, 0xF7, 0x00, 0xFF,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x14, 0x14, 0xF4, 0x04, 0xFC,
  0x14, 0x14, 0x17, 0x10, 0x1F,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0x1F, 0x00,
  0x10, 0x10, 0x10, 0xF0, 0x00,
  0x00, 0x00, 0x00, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0xF0, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x10,
  0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x10, 0xFF, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x14,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x00, 0x00, 0x1F, 0x10, 0x17,
  0x00, 0x00, 0xFC, 0x04, 0xF4,
  0x14, 0x14, 0x17, 0x10, 0x17,
  0x14, 0x14, 0xF4, 0x04, 0xF4,
  0x00, 0x00, 0xFF, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x14, 0x14, 0xF7, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x17, 0x14,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0xF4, 0x14,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x00, 0x00, 0x1F, 0x10, 0x1F,
  0x00, 0x00, 0x00, 0x1F, 0x14,
  0x00, 0x00, 0x00, 0xFC, 0x14,
  0x00, 0x00, 0xF0, 0x10, 0xF0,
  0x10, 0x10, 0xFF, 0x10, 0xFF,
  0x14, 0x14, 0x14, 0xFF, 0x14,
  0x10, 0x10, 0x10, 0x1F, 0x00,
  0x00, 0x00, 0x00, 0xF0, 0x10,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0xFF,
  0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
  0x38, 0x44, 0x44, 0x38, 0x44,
  0xFC, 0x4A, 0x4A, 0x4A, 0x34,  // sharp-s or beta
  0x7E, 0x02, 0x02, 0x06, 0x06,
  0x02, 0x7E, 0x02, 0x7E, 0x02,
  0x63, 0x55, 0x49, 0x41, 0x63,
  0x38, 0x44, 0x44, 0x3C, 0x04,
  0x40, 0x7E, 0x20, 0x1E, 0x20,
  0x06, 0x02, 0x7E, 0x02, 0x02,
  0x99, 0xA5, 0xE7, 0xA5, 0x99,
  0x1C, 0x2A, 0x49, 0x2A, 0x1C,
  0x4C, 0x72, 0x01, 0x72, 0x4C,
  0x30, 0x4A, 0x4D, 0x4D, 0x30,
  0x30, 0x48, 0x78, 0x48, 0x30,
  0xBC, 0x62, 0x5A, 0x46, 0x3D,
  0x3E, 0x49, 0x49, 0x49, 0x00,
  0x7E, 0x01, 0x01, 0x01, 0x7E,
  0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
  0x44, 0x44, 0x5F, 0x44, 0x44,
  0x40, 0x51, 0x4A, 0x44, 0x40,
  0x40, 0x44, 0x4A, 0x51, 0x40,
  0x00, 0x00, 0xFF, 0x01, 0x03,
  0xE0, 0x80, 0xFF, 0x00, 0x00,
  0x08, 0x08, 0x6B, 0x6B, 0x08,
  0x36, 0x12, 0x36, 0x24, 0x36,
  0x06, 0x0F, 0x09, 0x0F, 0x06,
  0x00, 0x00, 0x18, 0x18, 0x00,
  0x00, 0x00, 0x10, 0x10, 0x00,
  0x30, 0x40, 0xFF, 0x01, 0x01,
  0x00, 0x1F, 0x01, 0x01, 0x1E,
  0x00, 0x19, 0x1D, 0x17, 0x12,
  0x00, 0x3C, 0x3C, 0x3C, 0x3C,
  0x00, 0x00, 0x00, 0x00, 0x00  // #255 NBSP
};

// Teensy 3.1 can only generate 30 MHz SPI when running at 120 MHz (overclock)

// Constructor when using hardware ILI9241_KINETISK__pspi->  Faster, but must
// use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
ILI9341_t3n::ILI9341_t3n(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t mosi,
                         uint8_t sclk, uint8_t miso) {
  _cs = cs;
  _dc = dc;
  _rst = rst;
  _mosi = mosi;
  _sclk = sclk;
  _miso = miso;
  _width = TFT_WIDTH;
  _height = TFT_HEIGHT;

  rotation = 0;
  cursor_y = cursor_x = 0;
  textsize_x = textsize_y = 1;
  textcolor = textbgcolor = 0xFFFF;

  setClipRect();
  setOrigin();

  // Added to see how much impact actually using non hardware CS pin might be
  _cspinmask = 0;
  _csport = NULL;
}

//=======================================================================

void ILI9341_t3n::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
                                uint16_t y1) {
  beginSPITransaction(_SPI_CLOCK);
  setAddr(x0, y0, x1, y1);
  writecommand_last(ILI9341_RAMWR);  // write to RAM
  endSPITransaction();
}

void ILI9341_t3n::pushColor(uint16_t color) {
  beginSPITransaction(_SPI_CLOCK);
  writedata16_last(color);
  endSPITransaction();
}

void ILI9341_t3n::drawPixel(int16_t x, int16_t y, uint16_t color) {
  x += _originx;
  y += _originy;
  if ((x < _displayclipx1) || (x >= _displayclipx2) || (y < _displayclipy1) || (y >= _displayclipy2))
    return;

#ifdef REMOTE_CONSOLE
  if (console) {
    Serial.write(99);
    Serial.write(90);
    Serial.write(highByte(x));
    Serial.write(lowByte(x));
    Serial.write(highByte(y));
    Serial.write(lowByte(y));
    Serial.write(highByte(color));
    Serial.write(lowByte(color));
    Serial.write(88);
    //delayMicroseconds(60);  //necessary to avoid random pixels in remote console
  }
#endif

  beginSPITransaction(_SPI_CLOCK);
  setAddr(x, y, x, y);
  writecommand_cont(ILI9341_RAMWR);
  writedata16_last(color);
  endSPITransaction();
}

void ILI9341_t3n::drawFastVLine(int16_t x, int16_t y, int16_t h,
                                uint16_t color) {
  x += _originx;
  y += _originy;
  // Rectangular clipping
  if ((x < _displayclipx1) || (x >= _displayclipx2) || (y >= _displayclipy2))
    return;
  if (y < _displayclipy1) {
    h = h - (_displayclipy1 - y);
    y = _displayclipy1;
  }
  if ((y + h - 1) >= _displayclipy2)
    h = _displayclipy2 - y;
  if (h < 1)
    return;

  beginSPITransaction(_SPI_CLOCK);
  setAddr(x, y, x, y + h - 1);
  writecommand_cont(ILI9341_RAMWR);
  while (h-- > 1) {
    writedata16_cont(color);
  }
  writedata16_last(color);
  endSPITransaction();
}

void ILI9341_t3n::drawFastHLine(int16_t x, int16_t y, int16_t w,
                                uint16_t color) {
  x += _originx;
  y += _originy;

  // Rectangular clipping
  if ((y < _displayclipy1) || (x >= _displayclipx2) || (y >= _displayclipy2))
    return;
  if (x < _displayclipx1) {
    w = w - (_displayclipx1 - x);
    x = _displayclipx1;
  }
  if ((x + w - 1) >= _displayclipx2)
    w = _displayclipx2 - x;
  if (w < 1)
    return;

  beginSPITransaction(_SPI_CLOCK);
  setAddr(x, y, x + w - 1, y);
  writecommand_cont(ILI9341_RAMWR);
  while (w-- > 1) {
    writedata16_cont(color);
  }
  writedata16_last(color);
  endSPITransaction();
}

void ILI9341_t3n::fillScreen(uint16_t color) {
#ifdef REMOTE_CONSOLE
  console = true;
#endif
  fillRect(0, 0, _width, _height, color);
}

void ILI9341_t3n::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                           uint16_t color) {
  x += _originx;
  y += _originy;

  // Rectangular clipping (drawChar w/big text requires this)
  if ((x >= _displayclipx2) || (y >= _displayclipy2))
    return;
  if (((x + w) <= _displayclipx1) || ((y + h) <= _displayclipy1))
    return;
  if (x < _displayclipx1) {
    w -= (_displayclipx1 - x);
    x = _displayclipx1;
  }
  if (y < _displayclipy1) {
    h -= (_displayclipy1 - y);
    y = _displayclipy1;
  }
  if ((x + w - 1) >= _displayclipx2)
    w = _displayclipx2 - x;
  if ((y + h - 1) >= _displayclipy2)
    h = _displayclipy2 - y;

  if (console) {
#ifdef REMOTE_CONSOLE
    //remote console
    if (w != DISPLAY_WIDTH && h != DISPLAY_HEIGHT) {
      Serial.write(99);
      Serial.write(94);
      Serial.write(highByte(x));
      Serial.write(lowByte(x));
      Serial.write(highByte(y));
      Serial.write(lowByte(y));
      Serial.write(highByte(w));
      Serial.write(lowByte(w));
      Serial.write(highByte(h));
      Serial.write(lowByte(h));
      Serial.write(highByte(color));
      Serial.write(lowByte(color));
      Serial.write(88);
      //delayMicroseconds(50);  //necessary to avoid random pixels in remote console
    } else  //is fillscreen
    {
      Serial.write(99);
      Serial.write(93);
      Serial.write(highByte(color));
      Serial.write(lowByte(color));
      Serial.write(88);
    }
#endif
  }

  // TODO: this can result in a very long transaction time
  // should break this into multiple transactions, even though
  // it'll cost more overhead, so we don't stall other SPI libs
  beginSPITransaction(_SPI_CLOCK);
  setAddr(x, y, x + w - 1, y + h - 1);
  writecommand_cont(ILI9341_RAMWR);
  for (y = h; y > 0; y--) {
    for (x = w; x > 1; x--) {
      writedata16_cont(color);
    }
    writedata16_last(color);
#if 0
    if (y > 1 && (y & 1)) {
      endSPITransaction();
      beginSPITransaction(_SPI_CLOCK);
    }
#endif
  }
  endSPITransaction();
}

#define MADCTL_MY 0x80
#define MADCTL_MX 0x40
#define MADCTL_MV 0x20
#define MADCTL_ML 0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH 0x04

FLASHMEM void ILI9341_t3n::setRotation(uint8_t m) {
  rotation = m % 4;  // can't be higher than 3
  beginSPITransaction(_SPI_CLOCK);
  writecommand_cont(ILI9341_MADCTL);
  switch (rotation) {
    case 0:
      writedata8_last(MADCTL_MX | MADCTL_BGR);
      _width = TFT_WIDTH;
      _height = TFT_HEIGHT;
      break;
    case 1:
      writedata8_last(MADCTL_MV | MADCTL_BGR);
      _width = TFT_HEIGHT;
      _height = TFT_WIDTH;
      break;
    case 2:
      writedata8_last(MADCTL_MY | MADCTL_BGR);
      _width = TFT_WIDTH;
      _height = TFT_HEIGHT;
      break;
    case 3:
      writedata8_last(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      _width = TFT_HEIGHT;
      _height = TFT_WIDTH;
      break;
  }
  endSPITransaction();
  setClipRect();
  setOrigin();

  cursor_x = 0;
  cursor_y = 0;
}

void ILI9341_t3n::invertDisplay(boolean i) {
  beginSPITransaction(_SPI_CLOCK);
  writecommand_last(i ? ILI9341_INVON : ILI9341_INVOFF);
  endSPITransaction();
}


///============================================================================
// writeRect1BPP - 	write 1 bit per pixel paletted bitmap
//					bitmap data in array at pixels, 4 bits per
//pixel
//					color palette data in array at palette
//					width must be at least 8 pixels
void ILI9341_t3n::writeRect1BPP(int16_t x, int16_t y, int16_t w, int16_t h,
                                const uint8_t *pixels,
                                const uint16_t *palette) {
  // Simply call through our helper
  writeRectNBPP(x, y, w, h, 1, pixels, palette);
}

///============================================================================
// writeRectNBPP - 	write N(1, 2, 4, 8) bit per pixel paletted bitmap
//					bitmap data in array at pixels
void ILI9341_t3n::writeRectNBPP(int16_t x, int16_t y, int16_t w, int16_t h,
                                uint8_t bits_per_pixel, const uint8_t *pixels,
                                const uint16_t *palette) {
  x += _originx;
  y += _originy;
  uint8_t pixels_per_byte = 8 / bits_per_pixel;
  uint16_t count_of_bytes_per_row =
    (w + pixels_per_byte - 1) / pixels_per_byte;  // Round up to handle non multiples
  uint8_t row_shift_init =
    8 - bits_per_pixel;                                // We shift down 6 bits by default
  uint8_t pixel_bit_mask = (1 << bits_per_pixel) - 1;  // get mask to use below
  // Rectangular clipping

  // See if the whole thing out of bounds...
  if ((x >= _displayclipx2) || (y >= _displayclipy2))
    return;
  if (((x + w) <= _displayclipx1) || ((y + h) <= _displayclipy1))
    return;

  // In these cases you can not do simple clipping, as we need to synchronize
  // the colors array with the
  // We can clip the height as when we get to the last visible we don't have to
  // go any farther.
  // also maybe starting y as we will advance the color array.
  // Again assume multiple of 8 for width
  if (y < _displayclipy1) {
    int dy = (_displayclipy1 - y);
    h -= dy;
    pixels += dy * count_of_bytes_per_row;
    y = _displayclipy1;
  }

  if ((y + h - 1) >= _displayclipy2)
    h = _displayclipy2 - y;

  // For X see how many items in color array to skip at start of row and
  // likewise end of row
  if (x < _displayclipx1) {
    uint16_t x_clip_left = _displayclipx1 - x;
    w -= x_clip_left;
    x = _displayclipx1;
    // Now lets update pixels to the rigth offset and mask
    uint8_t x_clip_left_bytes_incr = x_clip_left / pixels_per_byte;
    pixels += x_clip_left_bytes_incr;
    row_shift_init =
      8 - (x_clip_left - (x_clip_left_bytes_incr * pixels_per_byte) + 1) * bits_per_pixel;
  }

  if ((x + w - 1) >= _displayclipx2) {
    w = _displayclipx2 - x;
  }

  const uint8_t *pixels_row_start =
    pixels;  // remember our starting position offset into row

  beginSPITransaction(_SPI_CLOCK);
  setAddr(x, y, x + w - 1, y + h - 1);
  writecommand_cont(ILI9341_RAMWR);
  for (; h > 0; h--) {
    pixels = pixels_row_start;             // setup for this row
    uint8_t pixel_shift = row_shift_init;  // Setup mask

    for (int i = 0; i < w; i++) {
      writedata16_cont(palette[((*pixels) >> pixel_shift) & pixel_bit_mask]);
      if (!pixel_shift) {
        pixel_shift = 8 - bits_per_pixel;  // setup next mask
        pixels++;
      } else {
        pixel_shift -= bits_per_pixel;
      }
    }
    pixels_row_start += count_of_bytes_per_row;
  }
  writecommand_last(ILI9341_NOP);
  endSPITransaction();
}

static const uint8_t PROGMEM init_commands[] = { 4, 0xEF, 0x03, 0x80, 0x02,
                                                 4, 0xCF, 0x00, 0XC1, 0X30,
                                                 5, 0xED, 0x64, 0x03, 0X12, 0X81,
                                                 4, 0xE8, 0x85, 0x00, 0x78,
                                                 6, 0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02,
                                                 2, 0xF7, 0x20,
                                                 3, 0xEA, 0x00, 0x00,
                                                 2, ILI9341_PWCTR1, 0x23,        // Power control
                                                 2, ILI9341_PWCTR2, 0x10,        // Power control
                                                 3, ILI9341_VMCTR1, 0x3e, 0x28,  // VCM control
                                                 2, ILI9341_VMCTR2, 0x86,        // VCM control2
                                                 2, ILI9341_MADCTL, 0x48,        // Memory Access Control
                                                 2, ILI9341_PIXFMT, 0x55,
                                                 3, ILI9341_FRMCTR1, 0x00, 0x18,
                                                 4, ILI9341_DFUNCTR, 0x08, 0x82, 0x27,  // Display Function Control
                                                 2, 0xF2, 0x00,                         // Gamma Function Disable
                                                 2, ILI9341_GAMMASET, 0x01,             // Gamma curve selected
                                                 16, ILI9341_GMCTRP1, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E,
                                                 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,  // Set Gamma
                                                 16, ILI9341_GMCTRN1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31,
                                                 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,  // Set Gamma
                                                 3, 0xb1, 0x00, 0x10,                             // FrameRate Control 119Hz
                                                 0 };

FLASHMEM void ILI9341_t3n::begin(uint32_t spi_clock, uint32_t spi_clock_read) {
  // verify SPI pins are valid;
  // allow user to say use current ones...
  _SPI_CLOCK = spi_clock;            // #define ILI9341_SPICLOCK 30000000
  _SPI_CLOCK_READ = spi_clock_read;  //#define ILI9341_SPICLOCK_READ 2000000

  // LOG.printf_P(PSTR("_t3n::begin mosi:%d miso:%d SCLK:%d CS:%d DC:%d SPI clocks: %lu %lu\n"), _mosi, _miso, _sclk, _cs, _dc, _SPI_CLOCK, _SPI_CLOCK_READ);
  // Serial.flush();

  if (SPI.pinIsMOSI(_mosi) && ((_miso == 0xff) || SPI.pinIsMISO(_miso)) && SPI.pinIsSCK(_sclk)) {
    _pspi = &SPI;
    _spi_num = 0;  // Which buss is this spi on?
#ifdef KINETISK
    _pkinetisk_spi = &KINETISK_SPI0;  // Could hack our way to grab this from SPI
    // object, but...
    _fifo_full_test = (3 << 12);
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
    _pimxrt_spi = &IMXRT_LPSPI4_S;  // Could hack our way to grab this from SPI
                                    // object, but...
#else
    _pkinetisl_spi = &KINETISL_SPI0;
#endif

#if defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__) || defined(__MKL26Z64__)
  } else if (SPI1.pinIsMOSI(_mosi) && ((_miso == 0xff) || SPI1.pinIsMISO(_miso)) && SPI1.pinIsSCK(_sclk)) {
    _pspi = &SPI1;
    _spi_num = 1;  // Which buss is this spi on?
#ifdef KINETISK
    _pkinetisk_spi = &KINETISK_SPI1;  // Could hack our way to grab this from SPI
    // object, but...
    _fifo_full_test = (0 << 12);
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
    _pimxrt_spi = &IMXRT_LPSPI3_S;  // Could hack our way to grab this from SPI
                                    // object, but...
#else
    _pkinetisl_spi = &KINETISL_SPI1;
#endif
#if !defined(__MKL26Z64__)
  } else if (SPI2.pinIsMOSI(_mosi) && ((_miso == 0xff) || SPI2.pinIsMISO(_miso)) && SPI2.pinIsSCK(_sclk)) {
    _pspi = &SPI2;
    _spi_num = 2;  // Which buss is this spi on?
#ifdef KINETISK
    _pkinetisk_spi = &KINETISK_SPI2;  // Could hack our way to grab this from SPI
    // object, but...
    _fifo_full_test = (0 << 12);
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
    _pimxrt_spi = &IMXRT_LPSPI1_S;  // Could hack our way to grab this from SPI
                                    // object, but...
#endif
#endif
#endif
  } else {
#ifdef DEBUG
    LOG.println(
      "ILI9341_t3n: The IO pins on the constructor are not valid SPI pins");

    LOG.printf_P(PSTR("    mosi:%d miso:%d SCLK:%d CS:%d DC:%d\n"), _mosi, _miso,
                    _sclk, _cs, _dc);
    Serial.flush();
#endif
    return;  // most likely will go bomb
  }
  // Make sure we have all of the proper SPI pins selected.
  _pspi->setMOSI(_mosi);
  _pspi->setSCK(_sclk);
  if (_miso != 0xff)
    _pspi->setMISO(_miso);

  // Hack to get hold of the SPI Hardware information...
  uint32_t *pa = (uint32_t *)((void *)_pspi);
  _spi_hardware = (SPIClass::SPI_Hardware_t *)(void *)pa[1];

  _pspi->begin();

  // LOG.println(F("   T4 setup CS/DC")); Serial.flush();
  pending_rx_count = 0;  // Make sure it is zero if we we do a second begin...
  _csport = portOutputRegister(_cs);
  _cspinmask = digitalPinToBitMask(_cs);
  pinMode(_cs, OUTPUT);
  DIRECT_WRITE_HIGH(_csport, _cspinmask);
  _spi_tcr_current = _pimxrt_spi->TCR;  // get the current TCR value

  // TODO:  Need to setup DC to actually work.
  if (_pspi->pinIsChipSelect(_dc)) {
    uint8_t dc_cs_index = _pspi->setCS(_dc);
    // LOG.printf_P(PSTR("    T4 hardware DC: %x\n"), dc_cs_index);
    _dcport = 0;
    _dcpinmask = 0;
    // will depend on which PCS but first get this to work...
    dc_cs_index--;  // convert to 0 based
    _tcr_dc_assert = LPSPI_TCR_PCS(dc_cs_index);
    _tcr_dc_not_assert = LPSPI_TCR_PCS(3);
  } else {
    // LOG.println(F("ILI9341_t3n: DC is not valid hardware CS pin"));
    _dcport = portOutputRegister(_dc);
    _dcpinmask = digitalPinToBitMask(_dc);
    pinMode(_dc, OUTPUT);
    DIRECT_WRITE_HIGH(_dcport, _dcpinmask);
    _tcr_dc_assert = LPSPI_TCR_PCS(0);
    _tcr_dc_not_assert = LPSPI_TCR_PCS(1);
  }
  maybeUpdateTCR(_tcr_dc_not_assert | LPSPI_TCR_FRAMESZ(7));

  // toggle RST low to reset
  if (_rst < 255) {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(5);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(150);
  }

  beginSPITransaction(_SPI_CLOCK / 4);
  const uint8_t *addr = init_commands;
  while (1) {
    uint8_t count = *addr++;
    if (count-- == 0)
      break;
    writecommand_cont(*addr++);
    while (count-- > 0) {
      writedata8_cont(*addr++);
    }
  }
  writecommand_last(ILI9341_SLPOUT);  // Exit Sleep
  endSPITransaction();
  delay(120);
  beginSPITransaction(_SPI_CLOCK);
  writecommand_last(ILI9341_DISPON);  // Display on
  endSPITransaction();
}

/*
  This is the core graphics library for all our displays, providing a common
  set of graphics primitives (points, lines, circles, etc.).  It needs to be
  paired with a hardware-specific library for each display device we carry
  (to handle the lower-level functions).

  Adafruit invests time and resources providing this open source code, please
  support Adafruit & open-source hardware by purchasing products from Adafruit!

  Copyright (c) 2013 Adafruit Industries.  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/



/**************************************************************************/
/*!
   @brief    Draw a circle outline
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void ILI9341_t3n::drawCircle(int16_t x0, int16_t y0, int16_t r,
                             uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0, y0 + r, color);
  drawPixel(x0, y0 - r, color);
  drawPixel(x0 + r, y0, color);
  drawPixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

/**************************************************************************/
/*!
    @brief    Quarter-circle drawer, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of
   the circle we're doing
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void ILI9341_t3n::drawCircleHelper(int16_t x0, int16_t y0, int16_t r,
                                   uint8_t cornername, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

/**************************************************************************/
/*!
   @brief    Draw a circle with filled color
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void ILI9341_t3n::fillCircle(int16_t x0, int16_t y0, int16_t r,
                             uint16_t color) {

#ifdef REMOTE_CONSOLE
  //remote console
  Serial.write(99);
  Serial.write(97);
  Serial.write(highByte(x0));
  Serial.write(lowByte(x0));
  Serial.write(highByte(y0));
  Serial.write(lowByte(y0));
  Serial.write(highByte(r));
  Serial.write(lowByte(r));
  Serial.write(highByte(color));
  Serial.write(lowByte(color));
  Serial.write(88);
  console = false;
#endif

  drawFastVLine(x0, y0 - r, 2 * r + 1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
#ifdef REMOTE_CONSOLE
  console = true;
#endif
}

/**************************************************************************/
/*!
    @brief  Quarter-circle drawer with fill, used for circles and roundrects
    @param  x0       Center-point x coordinate
    @param  y0       Center-point y coordinate
    @param  r        Radius of circle
    @param  corners  Mask bits indicating which quarters we're doing
    @param  delta    Offset from center-point, used for round-rects
    @param  color    16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void ILI9341_t3n::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
                                   uint8_t corners, int16_t delta,
                                   uint16_t color) {

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  delta++;  // Avoid some +1's in the loop

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if (x < (y + 1)) {
      if (corners & 1)
        drawFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
      if (corners & 2)
        drawFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
    }
    if (y != py) {
      if (corners & 1)
        drawFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
      if (corners & 2)
        drawFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
      py = y;
    }
    px = x;
  }
}

// Bresenham's algorithm - thx wikpedia
void ILI9341_t3n::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                           uint16_t color) {

#ifdef REMOTE_CONSOLE
  Serial.write(99);
  Serial.write(96);
  Serial.write(highByte(x0));
  Serial.write(lowByte(x0));
  Serial.write(highByte(y0));
  Serial.write(lowByte(y0));
  Serial.write(highByte(x1));
  Serial.write(lowByte(x1));
  Serial.write(highByte(y1));
  Serial.write(lowByte(y1));
  Serial.write(highByte(color));
  Serial.write(lowByte(color));
  Serial.write(88);
  console = false;
#endif

  if (y0 == y1) {
    if (x1 > x0) {
      drawFastHLine(x0, y0, x1 - x0 + 1, color);
    } else if (x1 < x0) {
      drawFastHLine(x1, y0, x0 - x1 + 1, color);
    } else {
      drawPixel(x0, y0, color);
    }
    return;

  } else if (x0 == x1) {
    if (y1 > y0) {
      drawFastVLine(x0, y0, y1 - y0 + 1, color);
    } else {
      drawFastVLine(x0, y1, y0 - y1 + 1, color);
    }
    return;
#ifdef REMOTE_CONSOLE
    console = true;  //maybe a good idea to remove this line
#endif
  }

  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    ILI9341_swap(x0, y0);
    ILI9341_swap(x1, y1);
  }
  if (x0 > x1) {
    ILI9341_swap(x0, x1);
    ILI9341_swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  int16_t xbegin = x0;
  if (steep) {
    for (; x0 <= x1; x0++) {
      err -= dy;
      if (err < 0) {
        int16_t len = x0 - xbegin;
        if (len) {
          drawFastVLine(y0, xbegin, len + 1, color);
        } else {
          drawPixel(y0, x0, color);
        }
        xbegin = x0 + 1;
        y0 += ystep;
        err += dx;
      }
    }
    if (x0 > xbegin + 1) {
      drawFastVLine(y0, xbegin, x0 - xbegin, color);
    }

  } else {
    for (; x0 <= x1; x0++) {
      err -= dy;
      if (err < 0) {
        int16_t len = x0 - xbegin;
        if (len) {
          drawFastHLine(xbegin, y0, len + 1, color);
        } else {
          drawPixel(x0, y0, color);
        }
        xbegin = x0 + 1;
        y0 += ystep;
        err += dx;
      }
    }
    if (x0 > xbegin + 1) {
      drawFastHLine(xbegin, y0, x0 - xbegin, color);
    }
  }
}

// Draw a rectangle
void ILI9341_t3n::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

  if (console) {
#ifdef REMOTE_CONSOLE
    Serial.write(99);
    Serial.write(98);
    Serial.write(highByte(x));
    Serial.write(lowByte(x));
    Serial.write(highByte(y));
    Serial.write(lowByte(y));
    Serial.write(highByte(w));
    Serial.write(lowByte(w));
    Serial.write(highByte(h));
    Serial.write(lowByte(h));
    Serial.write(highByte(color));
    Serial.write(lowByte(color));
    Serial.write(88);
    //delayMicroseconds(50);  //necessary to avoid random pixels in remote console
    console = false;
#endif
  }
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x + w - 1, y, h, color);
}

// overwrite functions from class Print:

size_t ILI9341_t3n::write(uint8_t c) {
  return write(&c, 1);
}

size_t ILI9341_t3n::write(const uint8_t *buffer, size_t size) {
  // Lets try to handle some of the special font centering code that was done
  // for default fonts.

#ifdef REMOTE_CONSOLE
  Serial.write(99);
  Serial.write(72);
  Serial.write(size);
  Serial.write(highByte(cursor_x));
  Serial.write(lowByte(cursor_x));
  Serial.write(highByte(cursor_y));
  Serial.write(lowByte(cursor_y));
  Serial.write(highByte(textcolor));
  Serial.write(lowByte(textcolor));
  Serial.write(highByte(textbgcolor));
  Serial.write(lowByte(textbgcolor));
  Serial.write(textsize_x);
  Serial.write(buffer, size);
  Serial.write(88);
  console = false;
#endif

  size_t cb = size;
  while (cb) {
    uint8_t c = *buffer++;
    cb--;

    // if (c == '\n') {
    //   // cursor_y += textsize_y * 8;
    //   // cursor_x = 0;
    // } else if (c == '\r') {
    //   // skip em
    // } else {
      drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
               textsize_y);
      cursor_x += textsize_x * 6;
    // }
  }
  return size;
}

// Draw a character
void ILI9341_t3n::drawChar(int16_t x, int16_t y, unsigned char c,
                           uint16_t fgcolor, uint16_t bgcolor, uint8_t size_x,
                           uint8_t size_y) {
  if ((x >= _width) ||               // Clip right
      (y >= _height) ||              // Clip bottom
      ((x + 6 * size_x - 1) < 0) ||  // Clip left  TODO: is this correct?
      ((y + 8 * size_y - 1) < 0))    // Clip top   TODO: is this correct?
    return;

// #ifdef REMOTE_CONSOLE
//   //remote console
//   Serial.write(99);
//   Serial.write(95);
//   Serial.write(highByte(x));
//   Serial.write(lowByte(x));
//   Serial.write(highByte(y));
//   Serial.write(lowByte(y));
//   Serial.write(highByte(fgcolor));
//   Serial.write(lowByte(fgcolor));
//   Serial.write(highByte(bgcolor));
//   Serial.write(lowByte(bgcolor));
//   Serial.write(c);
//   Serial.write(size_x);
//   Serial.write(88);
//   // delayMicroseconds(60); //necessary to avoid random pixels in remote console
//   console = false;
// #endif

  if (c == 32) {
    if (fgcolor == bgcolor) {
      if (size_x == 2)
        fillRect(x, y, CHAR_width, CHAR_height - 1, COLOR_BACKGROUND);
      else
        fillRect(x, y, CHAR_width_small, CHAR_height_small, COLOR_BACKGROUND);
    } else if (size_x == 2)
      fillRect(x, y, CHAR_width, CHAR_height - 1, bgcolor);
    else
      fillRect(x, y, CHAR_width_small, CHAR_height_small, bgcolor);
  } else if (fgcolor == bgcolor) {
    // This transparent approach is only about 20% faster
    if ((size_x == 1) && (size_y == 1)) {
      uint8_t mask = 0x01;
      int16_t xoff, yoff;
      for (yoff = 0; yoff < 8; yoff++) {
        uint8_t line = 0;
        for (xoff = 0; xoff < 5; xoff++) {
          if (font[c * 5 + xoff] & mask)
            line |= 1;
          line <<= 1;
        }
        line >>= 1;
        xoff = 0;
        while (line) {
          if (line == 0x1F) {
            drawFastHLine(x + xoff, y + yoff, 5, fgcolor);
            break;
          } else if (line == 0x1E) {
            drawFastHLine(x + xoff, y + yoff, 4, fgcolor);
            break;
          } else if ((line & 0x1C) == 0x1C) {
            drawFastHLine(x + xoff, y + yoff, 3, fgcolor);
            line <<= 4;
            xoff += 4;
          } else if ((line & 0x18) == 0x18) {
            drawFastHLine(x + xoff, y + yoff, 2, fgcolor);
            line <<= 3;
            xoff += 3;
          } else if ((line & 0x10) == 0x10) {
            drawPixel(x + xoff, y + yoff, fgcolor);
            line <<= 2;
            xoff += 2;
          } else {
            line <<= 1;
            xoff += 1;
          }
        }
        mask = mask << 1;
      }
    } else {
      uint8_t mask = 0x01;
      int16_t xoff, yoff;
      for (yoff = 0; yoff < 8; yoff++) {
        uint8_t line = 0;
        for (xoff = 0; xoff < 5; xoff++) {
          if (font[c * 5 + xoff] & mask)
            line |= 1;
          line <<= 1;
        }
        line >>= 1;
        xoff = 0;
        while (line) {
          if (line == 0x1F) {
            fillRect(x + xoff * size_x, y + yoff * size_y, 5 * size_x, size_y,
                     fgcolor);
            break;
          } else if (line == 0x1E) {
            fillRect(x + xoff * size_x, y + yoff * size_y, 4 * size_x, size_y,
                     fgcolor);
            break;
          } else if ((line & 0x1C) == 0x1C) {
            fillRect(x + xoff * size_x, y + yoff * size_y, 3 * size_x, size_y,
                     fgcolor);
            line <<= 4;
            xoff += 4;
          } else if ((line & 0x18) == 0x18) {
            fillRect(x + xoff * size_x, y + yoff * size_y, 2 * size_x, size_y,
                     fgcolor);
            line <<= 3;
            xoff += 3;
          } else if ((line & 0x10) == 0x10) {
            fillRect(x + xoff * size_x, y + yoff * size_y, size_x, size_y,
                     fgcolor);
            line <<= 2;
            xoff += 2;
          } else {
            line <<= 1;
            xoff += 1;
          }
        }
        mask = mask << 1;
      }
    }
  } else {
    // This solid background approach is about 5 time faster
    uint8_t xc, yc;
    uint8_t xr, yr;
    uint8_t mask = 0x01;
    uint16_t color;

    // We need to offset by the origin.
    x += _originx;
    y += _originy;
    int16_t x_char_start = x;  // remember our X where we start outputting...

    if ((x >= _displayclipx2) ||                    // Clip right
        (y >= _displayclipy2) ||                    // Clip bottom
        ((x + 6 * size_x - 1) < _displayclipx1) ||  // Clip left  TODO: this is not correct
        ((y + 8 * size_y - 1) < _displayclipy1))    // Clip top   TODO: this is not correct
      return;

    // need to build actual pixel rectangle we will output into.
    int16_t y_char_top = y;  // remember the y
    int16_t w = 6 * size_x;
    int16_t h = 8 * size_y;

    if (x < _displayclipx1) {
      w -= (_displayclipx1 - x);
      x = _displayclipx1;
    }
    if ((x + w - 1) >= _displayclipx2)
      w = _displayclipx2 - x;
    if (y < _displayclipy1) {
      h -= (_displayclipy1 - y);
      y = _displayclipy1;
    }
    if ((y + h - 1) >= _displayclipy2)
      h = _displayclipy2 - y;

    beginSPITransaction(_SPI_CLOCK);
    setAddr(x, y, x + w - 1, y + h - 1);
    y = y_char_top;  // restore the actual y.
    writecommand_cont(ILI9341_RAMWR);
    for (yc = 0; (yc < 8) && (y < _displayclipy2); yc++) {
      for (yr = 0; (yr < size_y) && (y < _displayclipy2); yr++) {
        x = x_char_start;  // get our first x position...
        if (y >= _displayclipy1) {
          for (xc = 0; xc < 5; xc++) {
            if (font[c * 5 + xc] & mask) {
              color = fgcolor;
            } else {
              color = bgcolor;
            }
            for (xr = 0; xr < size_x; xr++) {
              if ((x >= _displayclipx1) && (x < _displayclipx2)) {
                writedata16_cont(color);
              }
              x++;
            }
          }
          for (xr = 0; xr < size_x; xr++) {
            if ((x >= _displayclipx1) && (x < _displayclipx2)) {
              writedata16_cont(bgcolor);
            }
            x++;
          }
        }
        y++;
      }
      mask = mask << 1;
    }
    writecommand_last(ILI9341_NOP);
    endSPITransaction();
  }
}

//void ILI9341_t3n::charBounds(char c, int16_t *x, int16_t *y, int16_t *minx,
//                             int16_t *miny, int16_t *maxx, int16_t *maxy) {
//
//  // Default font
//
//  if (c == '\n') {        // Newline?
//    *x = 0;               // Reset x to zero,
//    *y += textsize_y * 8; // advance y one line
//    // min/max x/y unchaged -- that waits for next 'normal' character
//  } else if (c != '\r') { // Normal char; ignore carriage returns
//
//    int x2 = *x + textsize_x * 6 - 1, // Lower-right pixel of char
//        y2 = *y + textsize_y * 8 - 1;
//    if (x2 > *maxx)
//      *maxx = x2; // Track max x, y
//    if (y2 > *maxy)
//      *maxy = y2;
//    if (*x < *minx)
//      *minx = *x; // Track min x, y
//    if (*y < *miny)
//      *miny = *y;
//    *x += textsize_x * 6; // Advance x one char
//  }
//}


void ILI9341_t3n::setCursor(int16_t x, int16_t y, bool autoCenter) {
  _center_x_text = autoCenter;  // remember the state.
  _center_y_text = autoCenter;  // remember the state.
  if (x == ILI9341_t3n::CENTER) {
    _center_x_text = true;
    x = _width / 2;
  }
  if (y == ILI9341_t3n::CENTER) {
    _center_y_text = true;
    y = _height / 2;
  }
  if (x < 0)
    x = 0;
  else if (x >= _width)
    x = _width - 1;
  cursor_x = x;
  if (y < 0)
    y = 0;
  else if (y >= _height)
    y = _height - 1;
  cursor_y = y;
}

void ILI9341_t3n::getCursor(int16_t *x, int16_t *y) {
  *x = cursor_x;
  *y = cursor_y;
}

void ILI9341_t3n::setTextSize(uint8_t s_x, uint8_t s_y) {
  textsize_x = (s_x > 0) ? s_x : 1;
  textsize_y = (s_y > 0) ? s_y : 1;
}

void ILI9341_t3n::setTextColor(uint16_t c) {
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

void ILI9341_t3n::setTextColor(uint16_t c, uint16_t b) {
  textcolor = c;
  textbgcolor = b;
  // pre-expand colors for fast alpha-blending later
  textcolorPrexpanded =
    (textcolor | (textcolor << 16)) & 0b00000111111000001111100000011111;
  textbgcolorPrexpanded =
    (textbgcolor | (textbgcolor << 16)) & 0b00000111111000001111100000011111;
}

void ILI9341_t3n::sleep(bool enable) {
  beginSPITransaction(_SPI_CLOCK);
  if (enable) {
    writecommand_cont(ILI9341_DISPOFF);
    writecommand_last(ILI9341_SLPIN);
    endSPITransaction();
  } else {
    writecommand_cont(ILI9341_DISPON);
    writecommand_last(ILI9341_SLPOUT);
    endSPITransaction();
    delay(5);
  }
}


/***************************************************************************************
** Function name:           drawString (with or without user defined font)
** Description :            draw string with padding if it is defined
***************************************************************************************/
// Without font number, uses font set by setTextFont()
int16_t ILI9341_t3n::drawString(const String &string, int poX, int poY) {
  int16_t len = string.length() + 2;
  char buffer[len];
  string.toCharArray(buffer, len);
  return drawString(buffer, len - 2, poX, poY);
}

int16_t ILI9341_t3n::drawString(const char string[], int16_t len, int poX, int poY) {
  int16_t sumX = 0;

  setCursor(poX, poY);
  for (uint8_t i = 0; i < len; i++) {
    drawFontChar(string[i]);
    setCursor(cursor_x, cursor_y);
  }

  return sumX;
}

//////////////////////////////////////////////////////

void ILI9341_t3n::waitFifoNotFull(void) {
  uint32_t tmp __attribute__((unused));
  do {
    if ((_pimxrt_spi->RSR & LPSPI_RSR_RXEMPTY) == 0) {
      tmp = _pimxrt_spi->RDR;  // Read any pending RX bytes in
      if (pending_rx_count)
        pending_rx_count--;  // decrement count of bytes still levt
    }
  } while ((_pimxrt_spi->SR & LPSPI_SR_TDF) == 0);
}
void ILI9341_t3n::waitFifoEmpty(void) {
  uint32_t tmp __attribute__((unused));
  do {
    if ((_pimxrt_spi->RSR & LPSPI_RSR_RXEMPTY) == 0) {
      tmp = _pimxrt_spi->RDR;  // Read any pending RX bytes in
      if (pending_rx_count)
        pending_rx_count--;  // decrement count of bytes still levt
    }
  } while ((_pimxrt_spi->SR & LPSPI_SR_TCF) == 0);
}
void ILI9341_t3n::waitTransmitComplete(void) {
  uint32_t tmp __attribute__((unused));
  //    digitalWriteFast(2, HIGH);

  while (pending_rx_count) {
    if ((_pimxrt_spi->RSR & LPSPI_RSR_RXEMPTY) == 0) {
      tmp = _pimxrt_spi->RDR;  // Read any pending RX bytes in
      pending_rx_count--;      // decrement count of bytes still levt
    }
  }
  _pimxrt_spi->CR = LPSPI_CR_MEN | LPSPI_CR_RRF;  // Clear RX FIFO
  //    digitalWriteFast(2, LOW);
}

uint16_t ILI9341_t3n::waitTransmitCompleteReturnLast() {
  uint32_t val = 0;
  //    digitalWriteFast(2, HIGH);

  while (pending_rx_count) {
    if ((_pimxrt_spi->RSR & LPSPI_RSR_RXEMPTY) == 0) {
      val = _pimxrt_spi->RDR;  // Read any pending RX bytes in
      pending_rx_count--;      // decrement count of bytes still levt
    }
  }
  _pimxrt_spi->CR = LPSPI_CR_MEN | LPSPI_CR_RRF;  // Clear RX FIFO
  return val;
  //    digitalWriteFast(2, LOW);
}

void ILI9341_t3n::waitTransmitComplete(uint32_t mcr) {
  // BUGBUG:: figure out if needed...
  waitTransmitComplete();
}
