#ifndef GAUSSIAN
#define GAUSSIAN
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "bmp.h"

#ifdef ARM
#include <arm_neon.h>
#else
#include <xmmintrin.h>
#include <immintrin.h>
#include <tmmintrin.h>
#include <avxintrin.h>
#include <emmintrin.h>
#endif

int deno33 = 16;
int deno55 = 273;

// Gaussian kernel #1
unsigned char gaussian33[9] = {
    1,2,1,
    2,4,2,
    1,2,1
};
// Gaussian kernel #2
unsigned char gaussian55[25] = {
    1,  4,  7,  4, 1,
    4, 16, 26, 16, 4,
    7, 26, 41, 26, 7,
    4, 16, 26, 16, 4,
    1,  4,  7,  4, 1,
};

// Pthread data structure
typedef struct thread_info {
    pthread_t thread_id; // the id of thread
    int total_thread_size; // count for total
    int width; // image width
    int height; // image height
} tInfo;

unsigned char *global_src;
RGBTRIPLE *global_src_ori;

#ifdef ARM
void neon_gaussian_blur_5_tri(unsigned char *src,int w,int h);
#else
void unroll_gaussian_blur_5_tri(unsigned char *src,int w,int h);
void unroll_gaussian_blur_5_ori(RGBTRIPLE *src,int w,int h);
void naive_gaussian_blur_5(unsigned char *src,int w,int h);
void naive_gaussian_blur_5_original(RGBTRIPLE *src,int w,int h);
void sse_gaussian_blur_5_tri(unsigned char *src,int w,int h);
void sse_gaussian_blur_5_ori(RGBTRIPLE *src,int w,int h);
void sse_gaussian_blur_5_prefetch_ori(RGBTRIPLE *src,int w,int h);
void pt_gaussian_blur_5_tri(unsigned char *src,int num_threads,int w,int h);
void pt_sse_gaussian_blur_5_ori(RGBTRIPLE *src,int num_threads,int w,int h);
#endif

#endif
