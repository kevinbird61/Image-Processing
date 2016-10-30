#ifndef HSV
#define HSV
#include <stdlib.h>
#include  "bmp.h"

typedef struct _HSV {
    float h;
    float s;
    float v;
} HSVTRIPLE;

void change_brightness(RGBTRIPLE *src, float brightness, int w, int h);
void change_saturation(RGBTRIPLE *src, float saturation, int w, int h);
#endif
