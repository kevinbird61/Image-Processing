#include"mirror.h"

void neon_flip_vertical_tri(unsigned char *src, int w, int h)
{
    int half_height = h / 2;
    for(int i = 0; i < half_height; i++) {
        for(int j = 0; j < w; j+=16) {
            uint8_t *s1 = src + (i*w) + j;
            uint8_t *s2 = src + ((h-1-i)*w) + j;

            uint8x16_t v1 =  vld1q_u8((uint8_t *)s1);
            uint8x16_t v2 =  vld1q_u8((uint8_t *)s2);

            vst1q_u8((uint8_t *)s2,v1);
            vst1q_u8((uint8_t *)s1,v2);
        }
    }
}

void neon_flip_horizontal_tri(unsigned char *src, int w, int h)
{
    int half_width = w / 2;
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < half_width; j+=16) {
            uint8_t *s1 = src + (i*w) + j;
            uint8_t *s2 = src + (i*w) + (w-j-1) - 15;
            //const __m128i mask = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
            //const uint8x16_t mask = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
            //const uint8x8_t up_mask = {0, 1, 2, 3, 4, 5, 6, 7};
            //const uint8x8_t down_mask = {8, 9, 10, 11, 12, 13, 14, 15};
            const uint8x8_t down_mask = {7 ,6 ,5 , 4, 3, 2, 1};
            const uint8x8_t up_mask = {15,14,13,12,11,10,9,8};
            //uint8_t mask[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
            // uint8x16_t mask_v = vld1q_u8((uint8_t *)mask);

            uint8x16_t v1 =  vld1q_u8((uint8_t *)s1);
            uint8x16_t v2 =  vld1q_u8((uint8_t *)s2);

            //v1 = _mm_shuffle_epi8(v1, mask);
            //v2 = _mm_shuffle_epi8(v2, mask);
            //v1 = vcombine_u8(vtbl2_u8((uint8x8x2_t )v1, vget_low_u8(mask)), vtbl2_u8((uint8x8x2_t )v2, vget_high_u8(mask)));
            //v2 = vcombine_u8(vtbl2_u8((uint8x8x2_t )v2, vget_low_u8(mask)), vtbl2_u8((uint8x8x2_t )v2, vget_high_u8(mask)));
            uint8x8x2_t v1_882,v2_882;
            v1_882.val[0] = vget_low_u8(v1);
            v1_882.val[1] = vget_high_u8(v1);
            v2_882.val[0] = vget_low_u8(v2);
            v2_882.val[1] = vget_high_u8(v2);
            uint8x8_t v1_up = vtbl2_u8(v1_882, up_mask);
            uint8x8_t v1_down = vtbl2_u8(v1_882, down_mask);
            uint8x8_t v2_up = vtbl2_u8(v2_882, up_mask);
            uint8x8_t v2_down = vtbl2_u8(v2_882, down_mask);

            v1 = vcombine_u8(v1_up, v1_down);
            v2 = vcombine_u8(v2_up, v2_down);

            vst1q_u8((uint8_t *)s2,v1);
            vst1q_u8((uint8_t *)s1,v2);

        }
    }
}


