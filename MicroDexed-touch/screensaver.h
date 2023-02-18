#ifndef _SCREENSAVER_H
#define _SCREENSAVER_H

uint8_t const qix_num = 22;

typedef struct qix_s
{
  int counthue;
  int x0s[qix_num], y0s[qix_num], x1s[qix_num], y1s[qix_num];
  int dx0 = 2, dx1 = 3, dy0 = 3, dy1 = 2;
} qix_t;

#endif