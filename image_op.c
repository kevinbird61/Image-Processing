#ifndef IMAGE_OPERATION
#define IMAGE_OPERATION

unsigned char gaussian55[25] = {
    1,  4,  7,  4, 1,
    4, 16, 26, 16, 4,
    7, 26, 41, 26, 7,
    4, 16, 26, 16, 4,
    1,  4,  7,  4, 1,
};

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

#endif
