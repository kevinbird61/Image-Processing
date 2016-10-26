#include "gaussian.h"

void unroll_gaussian_blur_5_tri(unsigned char *src,int w,int h)
{
    for(int j=2; j<h-2; j++) {
        for(int i=2; i<w-2; i++) {
            int sum = 0;
            int index = 0;
            // Unroll the 5x5 for loop
            sum = src[(j-2)*w+(i-2)]*gaussian55[index++] + src[(j-2)*w+(i-1)]*gaussian55[index++]
                  + src[(j-2)*w+(i)]*gaussian55[index++] + src[(j-2)*w+(i+1)]*gaussian55[index++]
                  + src[(j-2)*w+(i+2)]*gaussian55[index++] + src[(j-1)*w+(i-2)]*gaussian55[index++]
                  + src[(j-1)*w+(i-1)]*gaussian55[index++] + src[(j-1)*w+(i)]*gaussian55[index++]
                  + src[(j-1)*w+(i+1)]*gaussian55[index++] + src[(j-1)*w+(i+2)]*gaussian55[index++]
                  + src[(j)*w+(i-2)]*gaussian55[index++] + src[(j)*w+(i-1)]*gaussian55[index++]
                  + src[(j)*w+(i)]*gaussian55[index++] + src[(j)*w+(i+1)]*gaussian55[index++]
                  + src[(j)*w+(i+2)]*gaussian55[index++] + src[(j+1)*w+(i-2)]*gaussian55[index++]
                  + src[(j+1)*w+(i-1)]*gaussian55[index++] + src[(j+1)*w+(i)]*gaussian55[index++]
                  + src[(j+1)*w+(i+1)]*gaussian55[index++] + src[(j+1)*w+(i+2)]*gaussian55[index++]
                  + src[(j+2)*w+(i-2)]*gaussian55[index++] + src[(j+2)*w+(i-1)]*gaussian55[index++]
                  + src[(j+2)*w+(i)]*gaussian55[index++] + src[(j+2)*w+(i+1)]*gaussian55[index++]
                  + src[(j+2)*w+(i+2)]*gaussian55[index++];

            sum = ((sum / 273) > 255 ? 255 : sum/273);
            src[j*w+i] = sum;
        }
    }
}

void unroll_gaussian_blur_5_ori(RGBTRIPLE *src,int w,int h)
{
    for(int j=2; j<h-2; j++) {
        for(int i=2; i<w-2; i++) {
            int sum_r = 0,sum_g = 0,sum_b = 0;
            int index = 0;
            // Unroll the 5x5 for loop
            sum_r = src[(j-2)*w+(i-2)].rgbRed*gaussian55[index++] + src[(j-2)*w+(i-1)].rgbRed*gaussian55[index++]
                    + src[(j-2)*w+(i)].rgbRed*gaussian55[index++] + src[(j-2)*w+(i+1)].rgbRed*gaussian55[index++]
                    + src[(j-2)*w+(i+2)].rgbRed*gaussian55[index++] + src[(j-1)*w+(i-2)].rgbRed*gaussian55[index++]
                    + src[(j-1)*w+(i-1)].rgbRed*gaussian55[index++] + src[(j-1)*w+(i)].rgbRed*gaussian55[index++]
                    + src[(j-1)*w+(i+1)].rgbRed*gaussian55[index++] + src[(j-1)*w+(i+2)].rgbRed*gaussian55[index++]
                    + src[(j)*w+(i-2)].rgbRed*gaussian55[index++] + src[(j)*w+(i-1)].rgbRed*gaussian55[index++]
                    + src[(j)*w+(i)].rgbRed*gaussian55[index++] + src[(j)*w+(i+1)].rgbRed*gaussian55[index++]
                    + src[(j)*w+(i+2)].rgbRed*gaussian55[index++] + src[(j+1)*w+(i-2)].rgbRed*gaussian55[index++]
                    + src[(j+1)*w+(i-1)].rgbRed*gaussian55[index++] + src[(j+1)*w+(i)].rgbRed*gaussian55[index++]
                    + src[(j+1)*w+(i+1)].rgbRed*gaussian55[index++] + src[(j+1)*w+(i+2)].rgbRed*gaussian55[index++]
                    + src[(j+2)*w+(i-2)].rgbRed*gaussian55[index++] + src[(j+2)*w+(i-1)].rgbRed*gaussian55[index++]
                    + src[(j+2)*w+(i)].rgbRed*gaussian55[index++] + src[(j+2)*w+(i+1)].rgbRed*gaussian55[index++]
                    + src[(j+2)*w+(i+2)].rgbRed*gaussian55[index++];
            index=0;
            sum_g = src[(j-2)*w+(i-2)].rgbGreen*gaussian55[index++] + src[(j-2)*w+(i-1)].rgbGreen*gaussian55[index++]
                    + src[(j-2)*w+(i)].rgbGreen*gaussian55[index++] + src[(j-2)*w+(i+1)].rgbGreen*gaussian55[index++]
                    + src[(j-2)*w+(i+2)].rgbGreen*gaussian55[index++] + src[(j-1)*w+(i-2)].rgbGreen*gaussian55[index++]
                    + src[(j-1)*w+(i-1)].rgbGreen*gaussian55[index++] + src[(j-1)*w+(i)].rgbGreen*gaussian55[index++]
                    + src[(j-1)*w+(i+1)].rgbGreen*gaussian55[index++] + src[(j-1)*w+(i+2)].rgbGreen*gaussian55[index++]
                    + src[(j)*w+(i-2)].rgbGreen*gaussian55[index++] + src[(j)*w+(i-1)].rgbGreen*gaussian55[index++]
                    + src[(j)*w+(i)].rgbGreen*gaussian55[index++] + src[(j)*w+(i+1)].rgbGreen*gaussian55[index++]
                    + src[(j)*w+(i+2)].rgbGreen*gaussian55[index++] + src[(j+1)*w+(i-2)].rgbGreen*gaussian55[index++]
                    + src[(j+1)*w+(i-1)].rgbGreen*gaussian55[index++] + src[(j+1)*w+(i)].rgbGreen*gaussian55[index++]
                    + src[(j+1)*w+(i+1)].rgbGreen*gaussian55[index++] + src[(j+1)*w+(i+2)].rgbGreen*gaussian55[index++]
                    + src[(j+2)*w+(i-2)].rgbGreen*gaussian55[index++] + src[(j+2)*w+(i-1)].rgbGreen*gaussian55[index++]
                    + src[(j+2)*w+(i)].rgbGreen*gaussian55[index++] + src[(j+2)*w+(i+1)].rgbGreen*gaussian55[index++]
                    + src[(j+2)*w+(i+2)].rgbGreen*gaussian55[index++];
            index=0;
            sum_b = src[(j-2)*w+(i-2)].rgbBlue*gaussian55[index++] + src[(j-2)*w+(i-1)].rgbBlue*gaussian55[index++]
                    + src[(j-2)*w+(i)].rgbBlue*gaussian55[index++] + src[(j-2)*w+(i+1)].rgbBlue*gaussian55[index++]
                    + src[(j-2)*w+(i+2)].rgbBlue*gaussian55[index++] + src[(j-1)*w+(i-2)].rgbBlue*gaussian55[index++]
                    + src[(j-1)*w+(i-1)].rgbBlue*gaussian55[index++] + src[(j-1)*w+(i)].rgbBlue*gaussian55[index++]
                    + src[(j-1)*w+(i+1)].rgbBlue*gaussian55[index++] + src[(j-1)*w+(i+2)].rgbBlue*gaussian55[index++]
                    + src[(j)*w+(i-2)].rgbBlue*gaussian55[index++] + src[(j)*w+(i-1)].rgbBlue*gaussian55[index++]
                    + src[(j)*w+(i)].rgbBlue*gaussian55[index++] + src[(j)*w+(i+1)].rgbBlue*gaussian55[index++]
                    + src[(j)*w+(i+2)].rgbBlue*gaussian55[index++] + src[(j+1)*w+(i-2)].rgbBlue*gaussian55[index++]
                    + src[(j+1)*w+(i-1)].rgbBlue*gaussian55[index++] + src[(j+1)*w+(i)].rgbBlue*gaussian55[index++]
                    + src[(j+1)*w+(i+1)].rgbBlue*gaussian55[index++] + src[(j+1)*w+(i+2)].rgbBlue*gaussian55[index++]
                    + src[(j+2)*w+(i-2)].rgbBlue*gaussian55[index++] + src[(j+2)*w+(i-1)].rgbBlue*gaussian55[index++]
                    + src[(j+2)*w+(i)].rgbBlue*gaussian55[index++] + src[(j+2)*w+(i+1)].rgbBlue*gaussian55[index++]
                    + src[(j+2)*w+(i+2)].rgbBlue*gaussian55[index++];

            sum_r = ((sum_r / 273) > 255 ? 255 : sum_r/273);
            sum_g = ((sum_g / 273) > 255 ? 255 : sum_g/273);
            sum_b = ((sum_b / 273) > 255 ? 255 : sum_b/273);
            src[j*w+i].rgbRed = sum_r;
            src[j*w+i].rgbGreen = sum_g;
            src[j*w+i].rgbBlue = sum_b;
        }
    }
}

void naive_gaussian_blur_5(unsigned char *src,int w,int h)
{
    for(int j=2; j<h-2; j++) {
        for(int i=2; i<w-2; i++) {
            int sum = 0;
            int index = 0;
            for(int sqr_j=j-2; sqr_j<j+3; sqr_j++) {
                for(int sqr_i=i-2; sqr_i<i+3; sqr_i++) {
                    sum += src[sqr_j*w+sqr_i]*gaussian55[index++];
                }
            }
            sum /= 273;
            if(sum > 255)
                sum = 255;
            src[j*w+i] = sum;
        }
    }
}

void naive_gaussian_blur_5_original(RGBTRIPLE *src,int w,int h)
{
    for(int j=2; j<h-2; j++) {
        for(int i=2; i<w-2; i++) {
            int sum_r = 0,sum_g = 0, sum_b = 0;
            int index = 0;
            for(int sqr_j=j-2; sqr_j<j+3; sqr_j++) {
                for(int sqr_i=i-2; sqr_i<i+3; sqr_i++) {
                    sum_r += src[sqr_j*w+sqr_i].rgbRed*gaussian55[index];
                    sum_g += src[sqr_j*w+sqr_i].rgbGreen*gaussian55[index];
                    sum_b += src[sqr_j*w+sqr_i].rgbBlue*gaussian55[index];
                    index++;
                }
            }
            src[j*w+i].rgbRed = ((sum_r/273) > 255 ) ? 255 : sum_r/273 ;
            src[j*w+i].rgbGreen = ((sum_g/273) > 255 ) ? 255 : sum_g/273 ;
            src[j*w+i].rgbBlue = ((sum_b/273) > 255 ) ? 255 : sum_b/273 ;
        }
    }
}

void sse_gaussian_blur_5_tri(unsigned char *src,int w,int h)
{
    // const data
    const __m128i vk0 = _mm_set1_epi8(0);
    const unsigned char sse_g1[16] = { 1,0,4,0,7,0,4,0,1,0,0,0,0,0,0,0 };
    const unsigned char sse_g2[16] = { 4,0,16,0,26,0,16,0,4,0,0,0,0,0,0,0 };
    const unsigned char sse_g3[16] = { 7,0,26,0,41,0,26,0,7,0,0,0,0,0,0,0 };
    const unsigned char sse_g4[16] = { 4,0,16,0,26,0,16,0,4,0,0,0,0,0,0,0 };
    const unsigned char sse_g5[16] = { 1,0,4,0,7,0,4,0,1,0,0,0,0,0,0,0 };
    // Operation to image
    for(int i=0; i<w-2; i++) {
        for(int j=0; j<h-2; j++) {
            int sum = 0;
            int index = 0;
            __m128i vg1 = _mm_loadu_si128((__m128i *)sse_g1);
            __m128i vg2 = _mm_loadu_si128((__m128i *)sse_g2);
            __m128i vg3 = _mm_loadu_si128((__m128i *)sse_g3);
            __m128i vg4 = _mm_loadu_si128((__m128i *)sse_g4);
            __m128i vg5 = _mm_loadu_si128((__m128i *)sse_g5);

            __m128i vsum = _mm_set1_epi8(0),vtemplow = _mm_set1_epi8(0),vtemphigh = _mm_set1_epi8(0),vempty = _mm_set1_epi8(0);
            // First element src[j*w+i]
            // Load in data
            __m128i L0 = _mm_loadu_si128((__m128i *)(src+(j+0)*w + i));
            __m128i L1 = _mm_loadu_si128((__m128i *)(src+(j+1)*w + i));
            __m128i L2 = _mm_loadu_si128((__m128i *)(src+(j+2)*w + i));
            __m128i L3 = _mm_loadu_si128((__m128i *)(src+(j+3)*w + i));
            __m128i L4 = _mm_loadu_si128((__m128i *)(src+(j+4)*w + i));
            // Get the data we need (5 element per-line) , because we only
            // need 5 element from sse instruction set , so only get low part(contain 8 elements)
            __m128i v0 = _mm_unpacklo_epi8(L0,vk0);
            __m128i v1 = _mm_unpacklo_epi8(L1,vk0);
            __m128i v2 = _mm_unpacklo_epi8(L2,vk0);
            __m128i v3 = _mm_unpacklo_epi8(L3,vk0);
            __m128i v4 = _mm_unpacklo_epi8(L4,vk0);
            // Multiple with specific Gaussian coef.
            v0 = _mm_maddubs_epi16(v0,vg1);
            v1 = _mm_maddubs_epi16(v1,vg2);
            v2 = _mm_maddubs_epi16(v2,vg3);
            v3 = _mm_maddubs_epi16(v3,vg4);
            v4 = _mm_maddubs_epi16(v4,vg5);
            // Summation the 5 line
            vsum = _mm_add_epi16(vsum,v0);
            vsum = _mm_add_epi16(vsum,v1);
            vsum = _mm_add_epi16(vsum,v2);
            vsum = _mm_add_epi16(vsum,v3);
            vsum = _mm_add_epi16(vsum,v4);
            // Vsum summation
            // Summation all - (Summation all - (Summation with shift-off 5 number))
            vtemplow = _mm_unpacklo_epi16(vsum,vempty); // 1,2,3,4
            vtemphigh = _mm_unpackhi_epi16(vsum,vempty); // 5

            sum += _mm_cvtsi128_si32(vtemplow); // get 1
            sum += _mm_cvtsi128_si32(_mm_srli_si128(vtemplow,4)); // get 2
            sum += _mm_cvtsi128_si32(_mm_srli_si128(vtemplow,8)); // get 3
            sum += _mm_cvtsi128_si32(_mm_srli_si128(vtemplow,12)); // get 4
            sum += _mm_cvtsi128_si32(vtemphigh); // get 5

            sum /= deno55;
            if(sum > 255)
                sum = 255;
            src[(j+2)*w+(i+2)] = sum;
        }
    }
}
