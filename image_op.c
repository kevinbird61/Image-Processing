#ifndef IMAGE_OPERATION
#define IMAGE_OPERATION

unsigned char gaussian55[25] = {
  1,  4,  7,  4, 1,
  4, 16, 26, 16, 4,
  7, 26, 41, 26, 7,
  4, 16, 26, 16, 4,
  1,  4,  7,  4, 1,
};

void naive_gaussian_blur_5(unsigned char *src,int w,int h){
  for(int j=2;j<h-2;j++){
    for(int i=2;i<w-2;i++){
      int sum = 0;
      int index = 0;
      for(int sqr_j=j-2;sqr_j<j+3;sqr_j++){
        for(int sqr_i=i-2;sqr_i<i+3;sqr_i++){
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

void avx_gaussian_blur(){

}

#endif
