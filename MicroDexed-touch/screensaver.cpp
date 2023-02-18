
#include "config.h"
#include "screensaver.h"
#include "ILI9341_t3n.h"

extern ILI9341_t3n display;
extern uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);
extern uint8_t screensaver_brightness;

qix_s qix;

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

    x_rot = 0.02;
    y_rot = 0.025;
    z_rot = 0.028;

    return;
}

FLASHMEM void topolar(float a, float b, float *r, float *theta)
{
    *theta = atan2(b, a);
    *r = sqrt(pow(a, 2) + pow(b, 2));
    return;
}

FLASHMEM void torect(float r, float theta, float *a, float *b)
{
    *a = r * cos(theta);
    *b = r * sin(theta);
    return;
}

FLASHMEM void project(float ax, float ay, float az, float *bx, float *by)
{
    // three point perspective
    *bx = ax / (p * ax + q * ay + r * az + 1) * (float)9.4 * scaler + (DISPLAY_WIDTH / 2);
    *by = ay / (p * ax + q * ay + r * az + 1) * (float)9.4 * scaler + (DISPLAY_HEIGHT / 2) - 8;
}

FLASHMEM void rotate(float *a, float *b, float theta)
{
    float r, theta_new;

    topolar(*a, *b, &r, &theta_new);
    theta_new = theta_new + theta;
    torect(r, theta_new, a, b);
    return;
}

FLASHMEM void rotatex(float *ax, float *ay, float *az, float theta)
{
    rotate(ay, az, theta);
    return;
}

FLASHMEM void rotatey(float *ax, float *ay, float *az, float theta)
{
    rotate(az, ax, theta);
    return;
}

FLASHMEM void rotatez(float *ax, float *ay, float *az, float theta)
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
    qix.counthue = qix.counthue + 1;
    if (qix.counthue > 359)
        qix.counthue = 0;

    int col = ColorHSV(qix.counthue, 254, screensaver_brightness);

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
    qix.counthue = qix.counthue + 1;
    if (qix.counthue > 359 - qix_num)
        qix.counthue = 0;

    display.drawLine(qix.x0s[qix_num - 1], qix.y0s[qix_num - 1], qix.x1s[qix_num - 1], qix.y1s[qix_num - 1], 0);
    for (uint8_t j = 0; j < qix_num - 1; j++)
    {
        display.drawLine(qix.x0s[j], qix.y0s[j], qix.x1s[j], qix.y1s[j], ColorHSV(qix.counthue + j, 254, screensaver_brightness));
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
            dv = (rand() & 6) + 4;  \
        }                           \
        if (v >= max_v)             \
        {                           \
            v = max_v - 1;          \
            dv = -(rand() & 6) - 4; \
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
    // setup 3d Cube
    // InitializeCube();

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
