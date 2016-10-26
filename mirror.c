#include "mirror.h"

static void swap_pixel(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void naive_flip_vertical(RGBTRIPLE *src, int w, int h)
{
    int half_height = h / 2;
    for(int i = 0; i < half_height; i++) {
        for(int j = 0; j < w; j++) {
            swap_pixel(&src[i*w+j], &src[(h-1-i)*w+j]);
        }
    }
}

void naive_flip_horizontal(RGBTRIPLE *src, int w, int h)
{
    int half_width = w / 2;
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < half_width; j++) {
            swap_pixel(&src[i*w+j], &src[i*w+w-j-1]);
        }
    }
}
