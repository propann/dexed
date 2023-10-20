/* Touchscreen library for XPT2046 Touch Controller Chip
   Copyright (c) 2015, Paul Stoffregen, paul@pjrc.com

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice, development funding notice, and this permission
   notice shall be included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
#include "config.h"
#if defined GENERIC_DISPLAY

#include "XPT2046_Touchscreen.h"

#define Z_THRESHOLD 400

#define SPI_SETTING SPISettings(2000000, MSBFIRST, SPI_MODE0)


FLASHMEM bool XPT2046_Touchscreen::begin(SPIClass &wspi)
{
 
  _pspi = &wspi;
  _pspi->begin();
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);

  return true;
}

TS_Point XPT2046_Touchscreen::getPoint()
{
  update();
  return TS_Point(xraw, yraw, zraw);
}

TS_Point XPT2046_Touchscreen::getPixel()
{
  update();
  uint16_t xPixel = (uint16_t)(_cal_dx * (xraw - _cal_vi1) / _cal_dvi + CAL_OFFSET);
  uint16_t yPixel = (uint16_t)(_cal_dy * (yraw - _cal_vj1) / _cal_dvj + CAL_OFFSET);

  // this->rotateCal(xPixel, yPixel);

  return TS_Point(xPixel, yPixel, zraw);
}

void XPT2046_Touchscreen::getCalibrationPoints(uint16_t &x1, uint16_t &y1, uint16_t &x2, uint16_t &y2)
{
  x1 = y1 = CAL_OFFSET;
  x2 = DISPLAY_WIDTH - CAL_OFFSET;
  y2 = DISPLAY_HEIGHT - CAL_OFFSET;
}

void XPT2046_Touchscreen::setCalibration(TS_Calibration cal)
{
  _cal_dx = DISPLAY_WIDTH - 2 * CAL_OFFSET;
  _cal_dy = DISPLAY_HEIGHT - 2 * CAL_OFFSET;

  _cal_vi1 = cal.vi1;
  _cal_vj1 = cal.vj1;
  _cal_dvi = (int32_t)cal.vi2 - cal.vi1;
  _cal_dvj = (int32_t)cal.vj2 - cal.vj1;
}

bool XPT2046_Touchscreen::touched()
{
  update();
  bool isTouched = (zraw >= Z_THRESHOLD);
  return isTouched;
}

// TODO: perhaps a future version should offer an option for more oversampling,
//       with the RANSAC algorithm https://en.wikipedia.org/wiki/RANSAC

void XPT2046_Touchscreen::update()
{
  if (_pspi) {
    _pspi->beginTransaction(SPI_SETTING);
    digitalWrite(csPin, LOW);
    _pspi->transfer(0xB1 /* queue Z1 */);
    int16_t z1 = _pspi->transfer16(0xC1 /* read Z1, queue Z2 */) >> 3;
    uint16_t z = z1 + 4095;
    int16_t z2 = _pspi->transfer16(0x91 /* read Z2, queue X */) >> 3;
    z -= z2;

    _pspi->transfer16(0x91);                  // dummy read, queue X again
    int16_t x = _pspi->transfer16(0xD0) >> 3; // read Y, queue Y, power down after next measurement
    int16_t y = _pspi->transfer16(0x00) >> 3; // read X, dummy write
    digitalWrite(csPin, HIGH);
    _pspi->endTransaction();
   
    switch (rotation) {
    case 0:
      xraw = 4095 - y;
      yraw = x;
      break;
    case 1:
      xraw = x;
      yraw = y;
      break;
    case 2:
      xraw = y;
      yraw = 4095 - x;
      break;
    default: // 3
      xraw = 4095 - x;
      yraw = 4095 - y;
    }
    zraw = z;
  }
}
#endif
