#ifndef MIRROR
#define MIRROR
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <tmmintrin.h>
#include <emmintrin.h>
#include "bmp.h"

void naive_flip_vertical_ori(RGBTRIPLE *src, int w, int h);
void naive_flip_horizontal_ori(RGBTRIPLE *src, int w, int h); 
void naive_flip_vertical_tri(unsigned char *src, int w, int h);
void naive_flip_horizontal_tri(unsigned char *src, int w, int h);
#endif // MIRROR
