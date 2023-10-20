/*!
 * @file Adafruit_FT6206.cpp
 *
 * @mainpage Adafruit FT2606 Library
 *
 * @section intro_sec Introduction
 *
 * This is a library for the Adafruit Capacitive Touch Screens
 *
 * ----> http://www.adafruit.com/products/1947
 *
 * Check out the links above for our tutorials and wiring diagrams
 * This chipset uses I2C to communicate
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License

 * MIT license, all text above must be included in any redistribution
 */

#include "Arduino.h"
#include "Adafruit_FT6206.h"
#include <Wire.h>
#include "config.h"

#ifdef CAPACITIVE_TOUCH_DISPLAY

#if defined(__SAM3X8E__)
#define Wire Wire1
#endif


/**************************************************************************/
/*!
    @brief  Instantiates a new FT6206 class
*/
/**************************************************************************/
// I2C, no address adjustments or pins
Adafruit_FT6206::Adafruit_FT6206()
{
  touches = 0;
}

/**************************************************************************/
/*!
    @brief  Setups the I2C interface and hardware, identifies if chip is found
    @param  thresh Optional threshhold-for-touch value, default is
   FT6206_DEFAULT_THRESSHOLD but you can try changing it if your screen is
   too/not sensitive.
    @returns True if an FT6206 is found, false on any failure
*/
/**************************************************************************/
boolean Adafruit_FT6206::begin(uint8_t thresh)
{
  Wire.begin();

  // change threshhold to be higher/lower
  writeRegister8(FT62XX_REG_THRESHHOLD, thresh);

  if (readRegister8(FT62XX_REG_VENDID) != FT62XX_VENDID)
  {
    return false;
  }
  uint8_t id = readRegister8(FT62XX_REG_CHIPID);
  if ((id != FT6206_CHIPID) && (id != FT6236_CHIPID) && (id != FT6236U_CHIPID))
  {
    return false;
  }

  return true;
}

/**************************************************************************/
/*!
    @brief  Determines if there are any touches detected
    @returns Number of touches detected, can be 0, 1 or 2
*/
/**************************************************************************/

uint8_t Adafruit_FT6206::touched(void)
{
  uint8_t n = readRegister8(FT62XX_REG_NUMTOUCHES);
  if (n > 2) {
    n = 0;
  }
  return n;
}

/**************************************************************************/
/*!
    @brief  Queries the chip and retrieves a point data
    @param  n The # index (0 or 1) to the points we can detect. In theory we can
   detect 2 points but we've found that you should only use this for
   single-touch since the two points cant share the same half of the screen.
    @returns {@link TS_Point} object that has the x and y coordinets set. If the
   z coordinate is 0 it means the point is not touched. If z is 1, it is
   currently touched.
*/
/**************************************************************************/
TS_Point Adafruit_FT6206::getPoint(uint8_t n)
{
  readData();
  if ((touches == 0) || (n > MAX_NUM_TOUCH_POINTS))
  {
    return TS_Point(0, 0, 0);
  }
  else
  {
    return TS_Point(touchX[n], touchY[n], 1);
  }
}

/************ lower level i/o **************/

/**************************************************************************/
/*!
    @brief  Reads the bulk of data from captouch chip. Fill in {@link touches},
   {@link touchX}, {@link touchY} and {@link touchID} with results
*/
/**************************************************************************/
void Adafruit_FT6206::readData(void)
{

  uint8_t i2cdat[16];
  Wire.beginTransmission(FT62XX_ADDR);
  Wire.write((byte)0);
  Wire.endTransmission();

  static constexpr uint8_t numBytesRead = 3 + MAX_NUM_TOUCH_POINTS * 4;
  Wire.requestFrom((byte)FT62XX_ADDR, (byte)numBytesRead);
  for (uint8_t i = 0; i < numBytesRead; i++) {
    i2cdat[i] = Wire.read();
  }

  touches = i2cdat[0x02];
  if ((touches > MAX_NUM_TOUCH_POINTS) || (touches == 0))
  {
    touches = 0;
  }

  for (uint8_t i = 0; i < MAX_NUM_TOUCH_POINTS; i++)
  {
    touchX[i] = i2cdat[0x03 + i * 6] & 0x0F;
    touchX[i] <<= 8;
    touchX[i] |= i2cdat[0x04 + i * 6];
    touchY[i] = i2cdat[0x05 + i * 6] & 0x0F;
    touchY[i] <<= 8;
    touchY[i] |= i2cdat[0x06 + i * 6];
    touchID[i] = i2cdat[0x05 + i * 6] >> 4;
  }
}

uint8_t Adafruit_FT6206::readRegister8(uint8_t reg)
{
  uint8_t x;
  // use i2c
  Wire.beginTransmission(FT62XX_ADDR);
  Wire.write((byte)reg);
  Wire.endTransmission();

  Wire.requestFrom((byte)FT62XX_ADDR, (byte)1);
  x = Wire.read();

  return x;
}

void Adafruit_FT6206::writeRegister8(uint8_t reg, uint8_t val)
{
  // use i2c
  Wire.beginTransmission(FT62XX_ADDR);
  Wire.write((byte)reg);
  Wire.write((byte)val);
  Wire.endTransmission();
}

/****************/

/**************************************************************************/
/*!
    @brief  Instantiates a new FT6206 class with x, y and z set to 0 by default
*/
/**************************************************************************/
TS_Point::TS_Point(void)
{
  x = y = z = 0;
}

/**************************************************************************/
/*!
    @brief  Instantiates a new FT6206 class with x, y and z set by params.
    @param  _x The X coordinate
    @param  _y The Y coordinate
    @param  _z The Z coordinate
*/
/**************************************************************************/

TS_Point::TS_Point(int16_t _x, int16_t _y, int16_t _z)
{
  x = _x;
  y = _y;
  z = _z;
}

/**************************************************************************/
/*!
    @brief  Simple == comparator for two TS_Point objects
    @returns True if x, y and z are the same for both points, False otherwise.
*/
/**************************************************************************/
bool TS_Point::operator==(TS_Point p1)
{
  return ((p1.x == x) && (p1.y == y) && (p1.z == z));
}

/**************************************************************************/
/*!
    @brief  Simple != comparator for two TS_Point objects
    @returns False if x, y and z are the same for both points, True otherwise.
*/
/**************************************************************************/
bool TS_Point::operator!=(TS_Point p1)
{
  return ((p1.x != x) || (p1.y != y) || (p1.z != z));
}

#endif
