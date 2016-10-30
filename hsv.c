#include "hsv.h"

static void find_min_max(RGBTRIPLE rgb, unsigned char *max, unsigned char *min)
{
    *max = 0;
    *min = 255;
    if(rgb.rgbRed > *max)
        *max = rgb.rgbRed;
    if(rgb.rgbRed < *min)
        *min = rgb.rgbRed;
    if(rgb.rgbGreen > *max)
        *max = rgb.rgbGreen;
    if(rgb.rgbGreen < *min)
        *min = rgb.rgbGreen;
    if(rgb.rgbBlue > *max)
        *max = rgb.rgbBlue;
    if(rgb.rgbBlue < *min)
        *min = rgb.rgbBlue;
}

void rgb2hsv(const RGBTRIPLE *rgb, HSVTRIPLE *hsv, int w, int h)
{
    int len = w * h;
    for(int i = 0; i < len; i++) {
        unsigned char max, min;
        find_min_max(rgb[i], &max, &min);
        //calculate H
        if(max == min)
            hsv[i].h = 0;
        else if(max == rgb[i].rgbRed && rgb[i].rgbGreen >= rgb[i].rgbBlue)
            hsv[i].h = 60 * (rgb[i].rgbGreen - rgb[i].rgbBlue) / (max-min) + 0;
        else if(max == rgb[i].rgbRed && rgb[i].rgbGreen < rgb[i].rgbBlue)
            hsv[i].h = 60 * (rgb[i].rgbGreen - rgb[i].rgbBlue) / (max-min) + 360;
        else if(max == rgb[i].rgbGreen)
            hsv[i].h = 60 * (rgb[i].rgbBlue - rgb[i].rgbRed) / (max-min) + 120;
        else if(max == rgb[i].rgbBlue)
            hsv[i].h = 60 * (rgb[i].rgbRed-rgb[i].rgbGreen) / (max-min) + 240;
        //calculate S
        if(max == 0)
            hsv[i].s = 0;
        else
            hsv[i].s = 1 - (min * 1000 / max) * 0.001;
        //calculate V
        hsv[i].v = max * 1000/ 255 * 0.001;
    }
}

void hsv2rgb(RGBTRIPLE *rgb, HSVTRIPLE *hsv, int w, int h)
{
    int len = w * h;
    for(int i = 0; i < len; i++) {
        int hi = (int)(hsv[i].h / 60) % 6;
        float f = (hsv[i].h / 60) - hi;
        float p = hsv[i].v * (1 - hsv[i].s);
        float q = hsv[i].v * (1 - (f * hsv[i].s));
        float t = hsv[i].v * (1 - ((1 - f) * hsv[i].s));
        switch(hi) {
            case 0:
                rgb[i].rgbRed = (unsigned char)(hsv[i].v * 255);
                rgb[i].rgbGreen = (unsigned char)(t * 255);
                rgb[i].rgbBlue = (unsigned char)(p * 255);
                break;
            case 1:
                rgb[i].rgbRed = (unsigned char)(q * 255);
                rgb[i].rgbGreen = (unsigned char)(hsv[i].v * 255);
                rgb[i].rgbBlue = (unsigned char)(p * 255);
                break;
            case 2:
                rgb[i].rgbRed = (unsigned char)(p * 255);
                rgb[i].rgbGreen = (unsigned char)(hsv[i].v * 255);
                rgb[i].rgbBlue = (unsigned char)(t * 255);
                break;
            case 3:
                rgb[i].rgbRed = (unsigned char)(p * 255);
                rgb[i].rgbGreen = (unsigned char)(q * 255);
                rgb[i].rgbBlue = (unsigned char)(hsv[i].v * 255);
                break;
            case 4:
                rgb[i].rgbRed = (unsigned char)(t * 255);
                rgb[i].rgbGreen = (unsigned char)(p * 255);
                rgb[i].rgbBlue = (unsigned char)(hsv[i].v * 255);
                break;
            case 5:
                rgb[i].rgbRed = (unsigned char)(hsv[i].v * 255);
                rgb[i].rgbGreen = (unsigned char)(p * 255);
                rgb[i].rgbBlue = (unsigned char)(q * 255);
                break;
        }
    }
}

void change_brightness(RGBTRIPLE *src, float brightness, int w, int h)
{
    HSVTRIPLE *hsv = (HSVTRIPLE *)malloc(sizeof(HSVTRIPLE) * w * h);
    rgb2hsv(src, hsv, w, h);
    for(int i = 0; i < w * h; i++)
        hsv[i].v = (brightness * hsv[i].v > 1 ? 1 : brightness * hsv[i].v);
    hsv2rgb(src, hsv, w, h);
}

void change_saturation(RGBTRIPLE *src, float saturation, int w, int h)
{
    HSVTRIPLE *hsv = (HSVTRIPLE *)malloc(sizeof(HSVTRIPLE) * w * h);
    rgb2hsv(src, hsv, w, h);
    for(int i = 0; i < w * h; i++)
        hsv[i].s = (saturation * hsv[i].s > 1 ? 1 : saturation * hsv[i].s);
    hsv2rgb(src, hsv, w, h);
}

