#include "gaussian.h"

void neon_gaussian_blur_5_tri(unsigned char *src,int w,int h)
{
    // const data

    uint8x16_t vk0 = vdupq_n_u8(0);
    uint8x16_t vk1 = vdupq_n_u8(0);
    uint8x16_t vk2 = vdupq_n_u8(0);
    uint8x16_t vk3 = vdupq_n_u8(0);
    uint8x16_t vk4 = vdupq_n_u8(0);

    //const unsigned char sse_g1[16] = { 1,0,4,0,7,0,4,0,1,0,0,0,0,0,0,0 };
    //uint8_t sse_g1[16] = { 1,0,4,0,7,0,4,0,1,0,0,0,0,0,0,0 };
    uint8x16_t vg1 = {1,0,4,0,7,0,4,0,1,0,0,0,0,0,0,0};
    //const unsigned char sse_g2[16] = { 4,0,16,0,26,0,16,0,4,0,0,0,0,0,0,0 };
    //uint8_t sse_g2[16] = { 4,0,16,0,26,0,16,0,4,0,0,0,0,0,0,0 };
    uint8x16_t vg2 = {4,0,16,0,26,0,16,0,4,0,0,0,0,0,0,0};
    //const unsigned char sse_g3[16] = { 7,0,26,0,41,0,26,0,7,0,0,0,0,0,0,0 };
    //uint8_t sse_g3[16] = { 7,0,26,0,41,0,26,0,7,0,0,0,0,0,0,0 };
    uint8x16_t vg3 = {7,0,26,0,41,0,26,0,7,0,0,0,0,0,0,0};
    //const unsigned char sse_g4[16] = { 4,0,16,0,26,0,16,0,4,0,0,0,0,0,0,0 };
    //uint8_t sse_g4[16] = { 4,0,16,0,26,0,16,0,4,0,0,0,0,0,0,0 };
    uint8x16_t vg4 = {4,0,16,0,26,0,16,0,4,0,0,0,0,0,0,0};
    // const unsigned char sse_g5[16] = { 1,0,4,0,7,0,4,0,1,0,0,0,0,0,0,0 };
    //uint8_t sse_g5[16] = { 1,0,4,0,7,0,4,0,1,0,0,0,0,0,0,0 };
    uint8x16_t vg5 = {4,0,16,0,26,0,16,0,4,0,0,0,0,0,0,0};

    // Operation to image
    for(int i=0; i<w-2; i++) {
        for(int j=0; j<h-2; j++) {
            int sum = 0;
            //
//            uint8x16_t vg1 = vld1q_u8((uint8_t *)sse_g1);
//            uint8x16_t vg2 = vld1q_u8((uint8_t *)sse_g2);
//            uint8x16_t vg3 = vld1q_u8((uint8_t *)sse_g3);
//            uint8x16_t vg4 = vld1q_u8((uint8_t *)sse_g4);
//            uint8x16_t vg5 = vld1q_u8((uint8_t *)sse_g5);

            //
            uint16x8_t vsum = vdupq_n_u16(0),vtemplow = vdupq_n_u16(0),vtemphigh = vdupq_n_u16(0),vempty = vdupq_n_u16(0);

            // First element src[j*w+i]
            // Load /in data
            //
            uint8x16_t L0 = vld1q_u8((uint8_t *)(src+(j+0)*w + i));
            uint8x16_t L1 = vld1q_u8((uint8_t *)(src+(j+1)*w + i));
            uint8x16_t L2 = vld1q_u8((uint8_t *)(src+(j+2)*w + i));
            uint8x16_t L3 = vld1q_u8((uint8_t *)(src+(j+3)*w + i));
            uint8x16_t L4 = vld1q_u8((uint8_t *)(src+(j+4)*w + i));
            // Get the data we need (5 element per-line) , because we only
            // need 5 element from sse instruction set , so only get low part(contain 8 elements)
            //
            uint8x16x2_t V0 = vzipq_u8(L0,vk0);//L0: v0
            uint8x16x2_t V1 = vzipq_u8(L1,vk1);//L1: v1
            uint8x16x2_t V2 = vzipq_u8(L2,vk2);//L2: v2
            uint8x16x2_t V3 = vzipq_u8(L3,vk3);//L3: v3
            uint8x16x2_t V4 = vzipq_u8(L4,vk4);//L4: v4
            //vzipq_u8(L0,vk0);//L0: v0
            //vzipq_u8(L1,vk1);//L1: v1
            //vzipq_u8(L2,vk2);//L2: v2
            //vzipq_u8(L3,vk3);//L3: v3
            //vzipq_u8(L4,vk4);//L4: v4

            // Multiple with specific Gaussian coef.
            //
            //uint16x8_t M0 = vmulq_u16((uint16x8_t)vrev16q_u8(L0), (uint16x8_t)vrev16q_u8(vg1));
            //uint16x8_t M1 = vmulq_u16((uint16x8_t)vrev16q_u8(L1), (uint16x8_t)vrev16q_u8(vg2));
            //uint16x8_t M2 = vmulq_u16((uint16x8_t)vrev16q_u8(L2), (uint16x8_t)vrev16q_u8(vg3));
            //uint16x8_t M3 = vmulq_u16((uint16x8_t)vrev16q_u8(L3), (uint16x8_t)vrev16q_u8(vg4));
            //uint16x8_t M4 = vmulq_u16((uint16x8_t)vrev16q_u8(L4), (uint16x8_t)vrev16q_u8(vg5));
            //uint16x8_t M0 = vmulq_u16((uint16x8_t)L0, (uint16x8_t)vg1);
            //uint16x8_t M1 = vmulq_u16((uint16x8_t)L1, (uint16x8_t)vg2);
            //uint16x8_t M2 = vmulq_u16((uint16x8_t)L2, (uint16x8_t)vg3);
            //uint16x8_t M3 = vmulq_u16((uint16x8_t)L3, (uint16x8_t)vg4);
            //uint16x8_t M4 = vmulq_u16((uint16x8_t)L4, (uint16x8_t)vg5);

            uint16x8_t L0_l = vmovl_u8(vget_low_u8(V0.val[1])), L0_h = vmovl_u8(vget_high_u8(V0.val[1]));
            uint16x8_t vg1_l = vmovl_u8(vget_low_u8(vg1)), vg1_h = vmovl_u8(vget_high_u8(vg1));
            uint16x8_t M0_l = vmulq_u16(L0_l,vg1_l), M0_h = vmulq_u16(L0_h,vg1_h);
            //vuzpq_u16(M0_l,M0_h);
            uint16x8x2_t P0 = vuzpq_u16(M0_l,M0_h);
            uint16x8_t L1_l = vmovl_u8(vget_low_u8(V1.val[1])), L1_h = vmovl_u8(vget_high_u8(V1.val[1]));
            uint16x8_t vg2_l = vmovl_u8(vget_low_u8(vg2)), vg2_h = vmovl_u8(vget_high_u8(vg2));
            uint16x8_t M1_l = vmulq_u16(L1_l,vg2_l), M1_h = vmulq_u16(L1_h,vg2_h);
            //vuzpq_u16(M0_l,M0_h);
            uint16x8x2_t P1 = vuzpq_u16(M1_l,M1_h);
            uint16x8_t L2_l = vmovl_u8(vget_low_u8(V2.val[1])), L2_h = vmovl_u8(vget_high_u8(V2.val[1]));
            uint16x8_t vg3_l = vmovl_u8(vget_low_u8(vg3)), vg3_h = vmovl_u8(vget_high_u8(vg3));
            uint16x8_t M2_l = vmulq_u16(L2_l,vg3_l), M2_h = vmulq_u16(L2_h,vg3_h);
            //vuzpq_u16(M0_l,M0_h);
            uint16x8x2_t P2 = vuzpq_u16(M2_l,M2_h);
            uint16x8_t L3_l = vmovl_u8(vget_low_u8(V3.val[1])), L3_h = vmovl_u8(vget_high_u8(V3.val[1]));
            uint16x8_t vg4_l = vmovl_u8(vget_low_u8(vg4)), vg4_h = vmovl_u8(vget_high_u8(vg4));
            uint16x8_t M3_l = vmulq_u16(L3_l,vg4_l), M3_h = vmulq_u16(L3_h,vg4_h);
            //vuzpq_u16(M0_l,M0_h);
            uint16x8x2_t P3 = vuzpq_u16(M3_l,M3_h);
            uint16x8_t L4_l = vmovl_u8(vget_low_u8(V4.val[1])), L4_h = vmovl_u8(vget_high_u8(V4.val[1]));
            uint16x8_t vg5_l = vmovl_u8(vget_low_u8(vg5)), vg5_h = vmovl_u8(vget_high_u8(vg5));
            uint16x8_t M4_l = vmulq_u16(L4_l,vg5_l), M4_h = vmulq_u16(L4_h,vg5_h);
            //vuzpq_u16(M0_l,M0_h);
            uint16x8x2_t P4 = vuzpq_u16(M4_l,M4_h);

            /*            uint16x8_t vg2_l = vmovl_u8(vget_low_u8(vg2)), vg2_h = vmovl_u8(vget_high_u8(vg2));// or pairwise add?
                        uint16x8_t M1_l = vmulq_u16(L1_l,vg2_l), M1_h = vmulq_u16(L1_h,vg2_h);
                        //unpack M0_l and insert the first lane of M0_h to the 5th
                        vuzpq_u16(M1_l,M1_h);
                        uint16x8_t L2_l = vmovl_u8(vget_low_u8(L2)), L2_h = vmovl_u8(vget_high_u8(L2));
                        uint16x8_t vg3_l = vmovl_u8(vget_low_u8(vg3)), vg3_h = vmovl_u8(vget_high_u8(vg3));// or pairwise add?
                        uint16x8_t M2_l = vmulq_u16(L2_l,vg3_l), M2_h = vmulq_u16(L2_h,vg3_h);
                        //unpack M0_l and insert the first lane of M0_h to the 5th
                        vuzpq_u16(M2_l,M2_h);
                        uint16x8_t L3_l = vmovl_u8(vget_low_u8(L3)), L3_h = vmovl_u8(vget_high_u8(L3));
                        uint16x8_t vg4_l = vmovl_u8(vget_low_u8(vg4)), vg4_h = vmovl_u8(vget_high_u8(vg4));// or pairwise add?
                        uint16x8_t M3_l = vmulq_u16(L3_l,vg4_l), M3_h = vmulq_u16(L3_h,vg4_h);
                        //unpack M0_l and insert the first lane of M0_h to the 5th
                        vuzpq_u16(M3_l,M3_h);
                        uint16x8_t L4_l = vmovl_u8(vget_low_u8(L4)), L4_h = vmovl_u8(vget_high_u8(L4));
                        uint16x8_t vg5_l = vmovl_u8(vget_low_u8(vg5)), vg5_h = vmovl_u8(vget_high_u8(vg5));// or pairwise add?
                        uint16x8_t M4_l = vmulq_u16(L4_l,vg5_l), M4_h = vmulq_u16(L4_h,vg5_h);
                        //unpack M0_l and insert the first lane of M0_h to the 5th
                        vuzpq_u16(M4_l,M4_h);*/

            // Summation the 5 line
            //
            /*
            vsum = vaddq_u16(vsum, M0);
            vsum = vaddq_u16(vsum, M1);
            vsum = vaddq_u16(vsum, M2);
            vsum = vaddq_u16(vsum, M3);
            vsum = vaddq_u16(vsum, M4);
            */
            /*
                        vsum = vaddq_u16(vsum, M0_l);
                        vsum = vaddq_u16(vsum, M1_l);
                        vsum = vaddq_u16(vsum, M2_l);
                        vsum = vaddq_u16(vsum, M3_l);
                        vsum = vaddq_u16(vsum, M4_l);
            */
            vsum = vaddq_u16(vsum, P0.val[0]);
            vsum = vaddq_u16(vsum, P1.val[0]);
            vsum = vaddq_u16(vsum, P2.val[0]);
            vsum = vaddq_u16(vsum, P3.val[0]);
            vsum = vaddq_u16(vsum, P4.val[0]);
            // Vsum summation

            // Summation all - (Summation all - (Summation with shift-off 5 number))
            //

            uint16x4x2_t result = vzip_u16(vget_low_u16(vsum),vget_low_u16(vempty));
            vtemplow = vcombine_u16(result.val[0],result.val[1]);

            result = vzip_u16(vget_high_u16(vsum),vget_high_u16(vempty));
            vtemphigh = vcombine_u16(result.val[0],result.val[1]);


            //
            sum += (uint32_t)vgetq_lane_u16(vtemplow,0);
            sum += (uint32_t)vgetq_lane_u16(vtemplow,1);
            sum += (uint32_t)vgetq_lane_u16(vtemplow,2);
            sum += (uint32_t)vgetq_lane_u16(vtemplow,3);
            sum += (uint32_t)vgetq_lane_u16(vtemphigh,0);
            printf("%d\n",sum);
            //sum /= deno55;
            sum /= 273;
            if(sum > 255)
                sum = 255;
            src[(j+2)*w+(i+2)] = sum;
        }
    }
}
/*
void neon_gaussian_blur_5_ori(RGBTRIPLE *src,int w,int h)
{
    const uint8x16_t v0hi = vdupq_n_u8(0);
    const uint8x16_t v1hi = vdupq_n_u8(0);
    const uint8x16_t v2hi = vdupq_n_u8(0);
    const uint8x16_t v3hi = vdupq_n_u8(0);
    const uint8x16_t v4hi = vdupq_n_u8(0);
    const unsigned char neon_g1_lo[16] = {1,0,1,0,1,0,4,0,4,0,4,0,7,0,7,0};
    const unsigned char neon_g1_hi[16] = {7,0,4,0,4,0,4,0,1,0,1,0,1,0,0,0};
    const unsigned char neon_g2_lo[16] = {4,0,4,0,4,0,16,0,16,0,16,0,26,0,26,0};
    const unsigned char neon_g2_hi[16] = {26,0,16,0,16,0,16,0,4,0,4,0,4,0,0,0};
    const unsigned char neon_g3_lo[16] = {7,0,7,0,7,0,26,0,26,0,26,0,41,0,41,0};
    const unsigned char neon_g3_hi[16] = {41,0,26,0,26,0,26,0,7,0,7,0,7,0,0,0};
    const unsigned char neon_g4_lo[16] = {4,0,4,0,4,0,16,0,16,0,16,0,26,0,26,0};
    const unsigned char neon_g4_hi[16] = {26,0,16,0,16,0,16,0,4,0,4,0,4,0,0,0};
    const unsigned char neon_g5_lo[16] = {1,0,1,0,1,0,4,0,4,0,4,0,7,0,7,0};
    const unsigned char neon_g5_hi[16] = {7,0,4,0,4,0,4,0,1,0,1,0,1,0,0,0};
    for(int i=0; i<(w-2); i++) {
        for(int j=0; j<(h-2); j++) {
            int sum_r = 0,sum_g = 0,sum_b = 0;
            uint8x16_t vg1lo = vld1q_u8((uint8x16_t *)neon_g1_lo);
            uint8x16_t vg1hi = vld1q_u8((uint8x16_t *)neon_g1_hi);
            uint8x16_t vg2lo = vld1q_u8((uint8x16_t *)neon_g2_lo);
            uint8x16_t vg2hi = vld1q_u8((uint8x16_t *)neon_g2_hi);
            uint8x16_t vg3lo = vld1q_u8((uint8x16_t *)neon_g3_lo);
            uint8x16_t vg3hi = vld1q_u8((uint8x16_t *)neon_g3_hi);
            uint8x16_t vg4lo = vld1q_u8((uint8x16_t *)neon_g4_lo);
            uint8x16_t vg4hi = vld1q_u8((uint8x16_t *)neon_g4_hi);
            uint8x16_t vg5lo = vld1q_u8((uint8x16_t *)neon_g5_lo);
            uint8x16_t vg5hi = vld1q_u8((uint8x16_t *)neon_g5_hi);

            uint8x16_t v0lo = vld1q_u8((uint8x16_t *)(src+(j+0)*w + i));
            uint8x16_t v1lo = vld1q_u8((uint8x16_t *)(src+(j+1)*w + i));
            uint8x16_t v2lo = vld1q_u8((uint8x16_t *)(src+(j+2)*w + i));
            uint8x16_t v3lo = vld1q_u8((uint8x16_t *)(src+(j+3)*w + i));
            uint8x16_t v4lo = vld1q_u8((uint8x16_t *)(src+(j+4)*w + i));

            vzipq_u8(v0lo,v0hi);
            vzipq_u8(v1lo,v1hi);
            vzipq_u8(v2lo,v2hi);
            vzipq_u8(v3lo,v3hi);
            vzipq_u8(v4lo,v4hi);



        }
    }
}
*/
