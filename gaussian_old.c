#include "gaussian_old.h"

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
            printf("%d\n",sum);
            sum /= 273;
            if(sum > 255)
                sum = 255;
            src[j*w+i] = sum;
        }
    }
}


