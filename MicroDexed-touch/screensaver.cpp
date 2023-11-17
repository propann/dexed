
#include "config.h"
#include "screensaver.h"
#include "ILI9341_t3n.h"

extern ILI9341_t3n display;
extern uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);
extern uint8_t screensaver_brightness;
extern uint16_t screensaver_counthue;

qix_s qix;

extern bool remote_active;

/*******************************************************************************/
// start code 3d cube
/*******************************************************************************/

// variables for 3dcube
float r = 0.2; // z
float p = 0;   // x
float q = 0;   // y
float a1x, a1y, a1z;
float a2x, a2y, a2z;
float a3x, a3y, a3z;
float a4x, a4y, a4z;
float a5x, a5y, a5z;
float a6x, a6y, a6z;
float a7x, a7y, a7z;
float a8x, a8y, a8z;
float b1x, b1y;
float b2x, b2y;
float b3x, b3y;
float b4x, b4y;
float b5x, b5y;
float b6x, b6y;
float b7x, b7y;
float b8x, b8y;

float b1x_old, b1y_old;
float b2x_old, b2y_old;
float b3x_old, b3y_old;
float b4x_old, b4y_old;
float b5x_old, b5y_old;
float b6x_old, b6y_old;
float b7x_old, b7y_old;
float b8x_old, b8y_old;

float scaler = 2;
float x_rot, y_rot, z_rot;
bool cube_dir;

FLASHMEM void InitializeCube(void)
{
  // define the points of the cube
  a1x = 1;
  a1y = 1;
  a1z = 1;
  a2x = 1;
  a2y = 1;
  a2z = -1;
  a3x = -1;
  a3y = 1;
  a3z = -1;
  a4x = -1;
  a4y = 1;
  a4z = 1;
  a5x = 1;
  a5y = -1;
  a5z = 1;
  a6x = 1;
  a6y = -1;
  a6z = -1;
  a7x = -1;
  a7y = -1;
  a7z = -1;
  a8x = -1;
  a8y = -1;
  a8z = 1;

  x_rot = 0.008;
  y_rot = 0.01;
  z_rot = 0.0112;

  return;
}

FLASHMEM void topolar(float a, float b, float* r, float* theta)
{
  *theta = atan2(b, a);
  *r = sqrt(pow(a, 2) + pow(b, 2));
  return;
}

FLASHMEM void torect(float r, float theta, float* a, float* b)
{
  *a = r * cos(theta);
  *b = r * sin(theta);
  return;
}

FLASHMEM void project(float ax, float ay, float az, float* bx, float* by)
{
  // three point perspective
  *bx = ax / (p * ax + q * ay + r * az + 1) * (float)9.4 * scaler + (DISPLAY_WIDTH / 2);
  *by = ay / (p * ax + q * ay + r * az + 1) * (float)9.4 * scaler + (DISPLAY_HEIGHT / 2) - 8;
}

FLASHMEM void rotate(float* a, float* b, float theta)
{
  float r, theta_new;

  topolar(*a, *b, &r, &theta_new);
  theta_new = theta_new + theta;
  torect(r, theta_new, a, b);
  return;
}

FLASHMEM void rotatex(float* ax, float* ay, float* az, float theta)
{
  rotate(ay, az, theta);
  return;
}

FLASHMEM void rotatey(float* ax, float* ay, float* az, float theta)
{
  rotate(az, ax, theta);
  return;
}

FLASHMEM void rotatez(float* ax, float* ay, float* az, float theta)
{
  rotate(ax, ay, theta);
  return;
}

FLASHMEM void RotateCubeX(float angle)
{
  // rotate the points of the the cube around the X axis
  rotatex(&a1x, &a1y, &a1z, angle);
  rotatex(&a2x, &a2y, &a2z, angle);
  rotatex(&a3x, &a3y, &a3z, angle);
  rotatex(&a4x, &a4y, &a4z, angle);
  rotatex(&a5x, &a5y, &a5z, angle);
  rotatex(&a6x, &a6y, &a6z, angle);
  rotatex(&a7x, &a7y, &a7z, angle);
  rotatex(&a8x, &a8y, &a8z, angle);
  return;
}

FLASHMEM void RotateCubeY(float angle)
{
  // rotate the points of the the cube around the Y axis
  rotatey(&a1x, &a1y, &a1z, angle);
  rotatey(&a2x, &a2y, &a2z, angle);
  rotatey(&a3x, &a3y, &a3z, angle);
  rotatey(&a4x, &a4y, &a4z, angle);
  rotatey(&a5x, &a5y, &a5z, angle);
  rotatey(&a6x, &a6y, &a6z, angle);
  rotatey(&a7x, &a7y, &a7z, angle);
  rotatey(&a8x, &a8y, &a8z, angle);
  return;
}

FLASHMEM void RotateCubeZ(float angle)
{
  // rotate the points of the cube around the Z axis
  rotatez(&a1x, &a1y, &a1z, angle);
  rotatez(&a2x, &a2y, &a2z, angle);
  rotatez(&a3x, &a3y, &a3z, angle);
  rotatez(&a4x, &a4y, &a4z, angle);
  rotatez(&a5x, &a5y, &a5z, angle);
  rotatez(&a6x, &a6y, &a6z, angle);
  rotatez(&a7x, &a7y, &a7z, angle);
  rotatez(&a8x, &a8y, &a8z, angle);
  return;
}

FLASHMEM void ProjectCube()
{
  // project the 3 points of the cube into the X,Y plane
  project(a1x, a1y, a1z, &b1x, &b1y);
  project(a2x, a2y, a2z, &b2x, &b2y);
  project(a3x, a3y, a3z, &b3x, &b3y);
  project(a4x, a4y, a4z, &b4x, &b4y);
  project(a5x, a5y, a5z, &b5x, &b5y);
  project(a6x, a6y, a6z, &b6x, &b6y);
  project(a7x, a7y, a7z, &b7x, &b7y);
  project(a8x, a8y, a8z, &b8x, &b8y);
}

FLASHMEM void DrawCube()
{
  int col = ColorHSV(screensaver_counthue, 254, screensaver_brightness);

  display.drawLine(b5x_old, b5y_old, b8x_old, b8y_old, COLOR_BACKGROUND);
  display.drawLine(b5x_old, b5y_old, b6x_old, b6y_old, COLOR_BACKGROUND);
  display.drawLine(b2x_old, b2y_old, b6x_old, b6y_old, COLOR_BACKGROUND);
  display.drawLine(b8x_old, b8y_old, b7x_old, b7y_old, COLOR_BACKGROUND);
  display.drawLine(b2x_old, b2y_old, b3x_old, b3y_old, COLOR_BACKGROUND);
  display.drawLine(b7x_old, b7y_old, b6x_old, b6y_old, COLOR_BACKGROUND);
  display.drawLine(b7x_old, b7y_old, b3x_old, b3y_old, COLOR_BACKGROUND);
  display.drawLine(b1x_old, b1y_old, b5x_old, b5y_old, COLOR_BACKGROUND);
  display.drawLine(b4x_old, b4y_old, b3x_old, b3y_old, COLOR_BACKGROUND);
  display.drawLine(b1x_old, b1y_old, b4x_old, b4y_old, COLOR_BACKGROUND);
  display.drawLine(b4x_old, b4y_old, b8x_old, b8y_old, COLOR_BACKGROUND);
  display.drawLine(b1x_old, b1y_old, b2x_old, b2y_old, COLOR_BACKGROUND);

  display.drawLine(b5x, b5y, b8x, b8y, col);
  display.drawLine(b5x, b5y, b6x, b6y, col);
  display.drawLine(b2x, b2y, b6x, b6y, col);
  display.drawLine(b8x, b8y, b7x, b7y, col);
  display.drawLine(b2x, b2y, b3x, b3y, col);
  display.drawLine(b7x, b7y, b6x, b6y, col);
  display.drawLine(b7x, b7y, b3x, b3y, col);
  display.drawLine(b1x, b1y, b5x, b5y, col);
  display.drawLine(b4x, b4y, b3x, b3y, col);
  display.drawLine(b1x, b1y, b4x, b4y, col);
  display.drawLine(b4x, b4y, b8x, b8y, col);
  display.drawLine(b1x, b1y, b2x, b2y, col);

  b1x_old = b1x;
  b1y_old = b1y;
  b2x_old = b2x;
  b2y_old = b2y;
  b3x_old = b3x;
  b3y_old = b3y;
  b4x_old = b4x;
  b4y_old = b4y;
  b5x_old = b5x;
  b5y_old = b5y;
  b6x_old = b6x;
  b6y_old = b6y;
  b7x_old = b7x;
  b7y_old = b7y;
  b8x_old = b8x;
  b8y_old = b8y;

  return;
}

FLASHMEM void qix_screensaver()
{
  display.drawLine(qix.x0s[qix_num - 1], qix.y0s[qix_num - 1], qix.x1s[qix_num - 1], qix.y1s[qix_num - 1], 0);
  for (uint8_t j = 0; j < qix_num - 1; j++)
  {
    uint16_t hsv = (screensaver_counthue + 2 * j) % 360;
    display.drawLine(qix.x0s[j], qix.y0s[j], qix.x1s[j], qix.y1s[j], ColorHSV(hsv, 254, screensaver_brightness));
  }
  for (uint8_t j = qix_num - 1; j >= 1; j--)
  {
    qix.x0s[j] = qix.x0s[j - 1];
    qix.x1s[j] = qix.x1s[j - 1];
    qix.y0s[j] = qix.y0s[j - 1];
    qix.y1s[j] = qix.y1s[j - 1];
  }
  qix.x0s[0] += qix.dx0;
  qix.x1s[0] += qix.dx1;
  qix.y0s[0] += qix.dy0;
  qix.y1s[0] += qix.dy1;
#define limit(v, dv, max_v)         \
    {                               \
        if (v < 0)                  \
        {                           \
            v = 0;                  \
            dv = ((random(5) / 4.0) + 2);  \
        }                           \
        if (v >= max_v)             \
        {                           \
            v = max_v - 1;          \
            dv = -((random(5) / 4.0) + 2); \
        }                           \
    }
  limit(qix.x0s[0], qix.dx0, TFT_HEIGHT);
  limit(qix.x1s[0], qix.dx1, TFT_HEIGHT);
  limit(qix.y0s[0], qix.dy0, TFT_WIDTH);
  limit(qix.y1s[0], qix.dy1, TFT_WIDTH);
#undef limit
}

FLASHMEM void cube_frame()
{
  // rotate the cube points around the defined axis
  RotateCubeX(x_rot);
  RotateCubeY(y_rot);
  RotateCubeZ(z_rot);

  // perform a 2D project of the 3D object onto the X,Y plane
  // with Z perspective
  ProjectCube();
  // Draw the cube on the screen
  DrawCube();
}

FLASHMEM void cube_screensaver()
{
  cube_frame();

  if (scaler < 7 && cube_dir)
  {
    scaler = scaler + 0.03;
  }
  else if (cube_dir)
  {
    cube_dir = false;
  }
  if (scaler > 2 && cube_dir == false)
  {
    scaler = scaler - 0.03;
  }
  else
    cube_dir = true;
}

/*
copyright 2007 Mike Edwards

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 */

 //using the algorithm from http://freespace.virgin.net/hugo.elias/models/m_perlin.html

float Noise2(float x, float y)
{
  long noise;
  noise = (long)x + (long)y * 57;
  return (1.0 - ((long)(noise * (noise * noise * 15731L + 789221L) + 1376312589L) & 0x7fffffff) / 1073741824.0);
}

float SmoothNoise2(float x, float y)
{
  float corners, sides, center;
  corners = (Noise2(x - 1, y - 1) + Noise2(x + 1, y - 1) + Noise2(x - 1, y + 1) + Noise2(x + 1, y + 1)) / 16;
  sides = (Noise2(x - 1, y) + Noise2(x + 1, y) + Noise2(x, y - 1) + Noise2(x, y + 1)) / 8;
  center = Noise2(x, y) / 4;
  return (corners + sides + center);
}
float CosineInterpolate(float a, float b, float x)
{
  float ft = x * 3.1415927;
  float f = (1 - cos(ft)) * .5;
  return(a * (1 - f) + b * f);
}
float Interpolate(float a, float b, float x)
{
  return(CosineInterpolate(a, b, x));
}
float InterpolatedNoise2(float x, float y)
{
  float v1, v2, v3, v4, i1, i2, fractionX, fractionY;
  long longX, longY;
  longX = (long)(x);
  fractionX = x - longX;
  longY = (long)(y);
  fractionY = y - longY;
  v1 = SmoothNoise2(longX, longY);
  v2 = SmoothNoise2(longX + 1, longY);
  v3 = SmoothNoise2(longX, longY + 1);
  v4 = SmoothNoise2(longX + 1, longY + 1);
  i1 = Interpolate(v1, v2, fractionX);
  i2 = Interpolate(v3, v4, fractionX);
  return(Interpolate(i1, i2, fractionY));
}
float LinearInterpolate(float a, float b, float x)
{
  return(a * (1 - x) + b * x);
}
float  persistence;
float PerlinNoise2(float x, float y, float persistance, int octaves)
{
  float frequency, amplitude;
  float total = 0.0;
  for (int i = 0; i <= octaves - 1; i++)
  {
    frequency = pow(2, i);
    amplitude = pow(persistence, i);
    total = total + InterpolatedNoise2(x * frequency, y * frequency) * amplitude;
  }
  return(total);
}
FLASHMEM void terrain_project(float ax, float ay, float az, float* bx, float* by)
{
  // three point perspective
  *bx = ax / (p * ax + q * ay + r * az + 1) + DISPLAY_WIDTH / 2;
  *by = ay / (p * ax + q * ay + r * az + 1) + 110;

}
using namespace std;
int fly = 1;

class Terrain {
public:
  int cols, rows;
  int scl = 16;
  int xoffset = DISPLAY_WIDTH / 2 + scl * 2;
  int octaves;
  float yy = 3.0;
  float ang = -0.04;
  void setup()
  {
    // display.fillScreen(COLOR_BACKGROUND);
    fly = 1;
    cols = DISPLAY_WIDTH / scl;
    rows = DISPLAY_HEIGHT / scl + 7;
    //persistence affects the degree to which the "finer" noise is seen
    persistence = 0.25;
    //octaves are the number of "layers" of noise that get computed
    octaves = 3;
  }
  void draw()
  {
    float d_x1 = 0;
    float d_x2 = 0;
    float d_y1 = 0;
    float d_y2 = 0;
    int col;
    uint8_t z_shift = 0;
    for (int y = 0; y < rows; y++) {

      for (int x = 0; x < cols + 4; x++) {
        for (uint8_t d = 0; d < 2; d++) {

          if (d == 0)
          {
            col = COLOR_BACKGROUND;
            z_shift = 1;
          }
          else
          {
            col = ColorHSV(screensaver_counthue, 254, screensaver_brightness);
            z_shift = 0;
          }
          a1x = x * scl - xoffset;
          a1y = y * yy;
          a1z = PerlinNoise2((float)x, y - fly + z_shift, persistence, octaves);
          rotatex(&a1x, &a1y, &a1z, ang);
          terrain_project(a1x, a1y, a1z, &b1x, &b1y);
          d_x1 = b1x;
          d_y1 = b1y;
          a1x = (x + 1) * scl - xoffset;
          a1y = y * yy;
          a1z = PerlinNoise2((float)(x + 1), y - fly + z_shift, persistence, octaves);
          rotatex(&a1x, &a1y, &a1z, ang);
          terrain_project(a1x, a1y, a1z, &b1x, &b1y);
          d_x2 = b1x;
          d_y2 = b1y;
          display.drawLine(d_x1, d_y1, d_x2, d_y2, col);
          a1y = y * yy + yy;
          a1z = PerlinNoise2((float)(x + 1), (y + 1 - fly + z_shift), persistence, octaves);
          rotatex(&a1x, &a1y, &a1z, ang);
          terrain_project(a1x, a1y, a1z, &b1x, &b1y);
          d_x1 = b1x;
          d_y1 = b1y;
          display.drawLine(d_x1, d_y1, d_x2, d_y2, col);
        }
      }
    }
    fly = fly + 1;
  }
};

extern void setCursor_textGrid(uint8_t pos_x, uint8_t pos_y);
extern void draw_logo_instant(uint8_t yoffset);
Terrain  terrain;
FLASHMEM void terrain_init()
{
  terrain.setup();
  draw_logo_instant(20);
}
FLASHMEM void terrain_frame()
{
  terrain.draw();
}

class PatternFlock {
public:

  Boid boids[boidCount];
  Boid predator;
  PVector wind;

  int flock_buffer_x[boidCount];
  int flock_buffer_y[boidCount];

  int predator_buffer_x;
  int predator_buffer_y;

  void start() {
      for (int i = 0; i < boidCount; i++) {
          boids[i] = Boid(random(DISPLAY_WIDTH), random(DISPLAY_HEIGHT));
      }
      predator = Boid(random(DISPLAY_WIDTH), random(DISPLAY_HEIGHT));
      predator.maxforce *= 1.6666666;
      predator.maxspeed *= 1.1;
      predator.neighbordist = 25.0;
      predator.desiredseparation = 0.0;
  }

  unsigned int drawFrame() {
    bool applyWind = random(0, 255) > 250;
    if (applyWind) {
        wind.x = Boid::randomf();
        wind.y = Boid::randomf();
    }

    int col = ColorHSV(0, 0, screensaver_brightness);
    for (int i = 0; i < boidCount; i++) {
        Boid* boid = &boids[i];

        // flee from predator
        boid->repelForce(predator.location, 25);
      
        boid->run(boids);
        PVector location = boid->location;

        display.fillRect(flock_buffer_x[i], flock_buffer_y[i], 2, 2, COLOR_BACKGROUND);
        display.fillRect(location.x, location.y, 2, 2, col);

        flock_buffer_x[i] = location.x;
        flock_buffer_y[i] = location.y;

        if (applyWind) {
            boid->applyForce(wind);
            applyWind = false;
        }
    }

    predator.run(boids);
    PVector location = predator.location;
    display.fillCircle(predator_buffer_x, predator_buffer_y, 3, COLOR_BACKGROUND);
    display.fillCircle(location.x, location.y, 3, ColorHSV(screensaver_counthue, 255, screensaver_brightness));
    predator_buffer_x = location.x;
    predator_buffer_y = location.y;

    return 50;
  }
};
PatternFlock flock_instance;

FLASHMEM void flock_init()
{
  flock_instance.start();
}
FLASHMEM void flock_frame()
{
  flock_instance.drawFrame();
}
