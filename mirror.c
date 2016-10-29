#include "mirror.h"

#define THREADS sysconf(_SC_NPROCESSORS_ONLN)

static void swap_pixel(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

static void swap_byte(unsigned char *a, unsigned char *b)
{
    unsigned char tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void naive_flip_vertical_ori(RGBTRIPLE *src, int w, int h)
{
    int half_height = h / 2;
    for(int i = 0; i < half_height; i++) {
        for(int j = 0; j < w; j++) {
            swap_pixel(&src[i*w+j], &src[(h-1-i)*w+j]);
        }
    }
}

void naive_flip_vertical_tri(unsigned char *src, int w, int h)
{
    int half_height = h / 2;
    for(int i = 0; i < half_height; i++) {
        for(int j = 0; j < w; j++) {
            swap_byte(&src[i*w+j], &src[(h-1-i)*w+j]);
        }
    }
}

void omp_flip_vertical_tri(unsigned char *src, int w, int h)
{
    int half_height = h / 2;
    #pragma omp parallel num_threads(THREADS)
    {
        #pragma omp for
        for(int i = 0; i < half_height; i++) {
            for(int j = 0; j < w; j++) {
                swap_byte(&src[i*w+j], &src[(h-1-i)*w+j]);
            }
        }
    }
}

void sse_flip_vertical_tri(unsigned char *src, int w, int h)
{
    int half_height = h / 2;
    for(int i = 0; i < half_height; i++) {
        for(int j = 0; j < w; j+=16) {
            unsigned char *s1 = src + (i*w) + j;
            unsigned char *s2 = src + ((h-1-i)*w) + j;
            __m128i v1 = _mm_loadu_si128((__m128i *)s1);
            __m128i v2 = _mm_loadu_si128((__m128i *)s2);
            _mm_storeu_si128((__m128i *)s2, v1);
            _mm_storeu_si128((__m128i *)s1, v2);
        }
    }
}

void naive_flip_horizontal_ori(RGBTRIPLE *src, int w, int h)
{
    int half_width = w / 2;
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < half_width; j++) {
            swap_pixel(&src[i*w+j], &src[i*w+w-j-1]);
        }
    }
}

void naive_flip_horizontal_tri(unsigned char *src, int w, int h)
{
    int half_width = w / 2;
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < half_width; j++) {
            swap_byte(&src[i*w+j], &src[i*w+w-j-1]);
        }
    }
}

void omp_flip_horizontal_tri(unsigned char *src, int w, int h)
{
    int half_width = w / 2;
    #pragma omp parallel num_threads(THREADS)
    {
        #pragma omp for
        for(int i = 0; i < h; i++) {
            for(int j = 0; j < half_width; j++) {
                swap_byte(&src[i*w+j], &src[i*w+w-j-1]);
            }
        }
    }
}

void sse_flip_horizontal_tri(unsigned char *src, int w, int h)
{
    int half_width = w / 2;
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < half_width; j+=16) {
            unsigned char *s1 = src + (i*w) + j;
            unsigned char *s2 = src + (i*w) + (w-j-1) - 15;
            const __m128i mask = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
            __m128i v1 = _mm_loadu_si128((__m128i *)s1);
            __m128i v2 = _mm_loadu_si128((__m128i *)s2);
            v1 = _mm_shuffle_epi8(v1, mask);
            v2 = _mm_shuffle_epi8(v2, mask);
            _mm_storeu_si128((__m128i *)s2, v1);
            _mm_storeu_si128((__m128i *)s1, v2);
        }
    }
}
