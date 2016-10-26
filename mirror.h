#ifndef MIRROR
#define MIRROR
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <tmmintrin.h>
#include <avxintrin.h>
#include <emmintrin.h>
#include "bmp.h"

static void swap_pixel(RGBTRIPLE *a, RGBTRIPLE *b);
void naive_flip_vertical(RGBTRIPLE *src, int w, int h);
void naive_flip_horizontal(RGBTRIPLE *src, int w, int h);

#endif // MIRROR
