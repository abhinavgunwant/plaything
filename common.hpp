#ifndef PLAYTHING_COMMON
#define PLAYTHING_COMMON

#define VEC_ZERO { 0.0f, 0.0f, 0.0f }

;

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define SCREEN_WIDTH_2 800
#define SCREEN_HEIGHT_2 450

#define COLOR_TRANSPARENT { 0, 0, 0, 0 }
#define COLOR_INV_BG { 0, 0, 0, 50 }
#define COLOR_INV_BG_ACTIVE { 0, 100, 255, 150 }

#define COLOR_EQUAL(x, y) x.r == y.r && x.g == y.g && x.b == y.b && x.a == y.a

#endif
