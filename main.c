#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "gaussian.h"
#include "bmp.h"
//#include "mirror.h"
#define FILTER(a,b) a&b
/*********************************************************/
/*  Global variables declaration：                                             */
/*  bmpHeader    ： BMP's header part
/*  bmpInfo      ： BMP's file infomation
/*  **BMPSaveData： BMP's data , which will be store into output
/*  **BMPData    ： BMP's temperary data storage
/*********************************************************/
BMPHEADER bmpHeader;
BMPINFO bmpInfo;
RGBTRIPLE *BMPSaveData = NULL;
RGBTRIPLE *BMPData = NULL;
unsigned char *color_r;
unsigned char *color_g;
unsigned char *color_b;
/*********************************************************/
/* Function declaration：
/*  readBMP    ： read the source bmp data , and store data into BMPSaveData
/*  saveBMP    ： write the BMPSaveData into output file , which is also .bmp
/*  fetchloc   :  get the element which is on (X,Y)
/*  swap       ： swap 2 data pointer (BMPSaveData and BMPData)
/*  **alloc_memory： dynamically allocate the 1D array data (sim. 2D)
/*  split_structure : split the original structure to fit SSE
/*  diff_in_millisecond : calculate the time of execution
/*********************************************************/
int readBMP( char *fileName);
int saveBMP( char *fileName);
RGBTRIPLE fetchloc(RGBTRIPLE *arr, int Y, int X);
RGBTRIPLE *alloc_memory( int Y, int X );
void swap(RGBTRIPLE **a, RGBTRIPLE **b);
void split_structure();
void merge_structure();
static double diff_in_millisecond(struct timespec t1, struct timespec t2);

int main(int argc,char *argv[])
{
    char *infileName = argv[1];
    char *outfileName = argv[2];
    int execution_times = atoi(argv[3]),threadcount;
    if(argc == 5)
        threadcount = atoi(argv[4]);
    struct timespec start, end;
    double cpu_time;
    // Load Data into BMPSaveData
    if ( readBMP( infileName) ) {
#if PERF
#else
        printf("Read file successfully\n");
#endif
    } else
        printf("Read file failed\n");

    // =================== Main Operation to BMP data ===================== //
#if FILTER(GAUSSIAN,1)
    color_r = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_g = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_b = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    split_structure();
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i=0; i<execution_times; i++) {
        naive_gaussian_blur_5(color_r,bmpInfo.biWidth,bmpInfo.biHeight);
        naive_gaussian_blur_5(color_g,bmpInfo.biWidth,bmpInfo.biHeight);
        naive_gaussian_blur_5(color_b,bmpInfo.biWidth,bmpInfo.biHeight);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    merge_structure();
#ifdef PERF
    printf("%f ",cpu_time);
#else
    printf("Gaussian blur[5x5][split structure], execution time : %f ms , with %d times Gaussian blur\n",cpu_time,execution_times);
#endif
#endif
#if FILTER(GAUSSIAN,2)
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i=0; i<execution_times; i++)
        naive_gaussian_blur_5_original(BMPSaveData,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
#ifdef PERF
    printf("%f ",cpu_time);
#else
    printf("Gaussian blur[5x5][original structure], execution time : %f ms , with %d times Gaussian blur\n",cpu_time,execution_times);
#endif
#endif
#if FILTER(GAUSSIAN,4)
    color_r = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_g = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_b = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    split_structure();
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i=0; i<execution_times; i++) {
        sse_gaussian_blur_5_tri(color_r,bmpInfo.biWidth,bmpInfo.biHeight);
        sse_gaussian_blur_5_tri(color_g,bmpInfo.biWidth,bmpInfo.biHeight);
        sse_gaussian_blur_5_tri(color_b,bmpInfo.biWidth,bmpInfo.biHeight);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    merge_structure();
#ifdef PERF
    printf("%f ",cpu_time);
#else
    printf("Gaussian blur[5x5][sse split structure], execution time : %f ms , with %d times Gaussian blur\n",cpu_time,execution_times);
#endif
#endif
#if FILTER(GAUSSIAN,8)
    color_r = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_g = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_b = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    split_structure();
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i=0; i<execution_times; i++) {
        unroll_gaussian_blur_5_tri(color_r,bmpInfo.biWidth,bmpInfo.biHeight);
        unroll_gaussian_blur_5_tri(color_g,bmpInfo.biWidth,bmpInfo.biHeight);
        unroll_gaussian_blur_5_tri(color_b,bmpInfo.biWidth,bmpInfo.biHeight);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    merge_structure();
#ifdef PERF
    printf("%f ",cpu_time);
#else
    printf("Gaussian blur[5x5][unroll split structure], execution time : %f ms , with %d times Gaussian blur\n",cpu_time,execution_times);
#endif
#endif
#if FILTER(GAUSSIAN,16)
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i=0; i<execution_times; i++)
        unroll_gaussian_blur_5_ori(BMPSaveData,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
#ifdef PERF
    printf("%f ",cpu_time);
#else
    printf("Gaussian blur[5x5][unroll original structure], execution time : %f ms , with %d times Gaussian blur\n",cpu_time,execution_times);
#endif
#endif
#if FILTER(GAUSSIAN,32)
    color_r = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_g = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_b = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    split_structure();
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i=0; i<execution_times; i++) {
        pt_gaussian_blur_5_tri(color_r,threadcount,bmpInfo.biWidth,bmpInfo.biHeight);
        pt_gaussian_blur_5_tri(color_g,threadcount,bmpInfo.biWidth,bmpInfo.biHeight);
        pt_gaussian_blur_5_tri(color_b,threadcount,bmpInfo.biWidth,bmpInfo.biHeight);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    merge_structure();
#ifdef PERF
    printf("%f ",cpu_time);
#else
    printf("Gaussian blur[5x5][pthread unroll split structure], execution time : %f ms , with %d times Gaussian blur\n",cpu_time,execution_times);
#endif
#endif
#if FILTER(GAUSSIAN,64)
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i=0; i<execution_times; i++)
        sse_gaussian_blur_5_ori(BMPSaveData,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
#ifdef PERF
    printf("%f ",cpu_time);
#else
    printf("Gaussian blur[5x5][sse original structure], execution time : %f ms , with %d times Gaussian blur\n",cpu_time,execution_times);
#endif
#endif
#if FILTER(GAUSSIAN,128)
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i=0; i<execution_times; i++)
        sse_gaussian_blur_5_prefetch_ori(BMPSaveData,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
#ifdef PERF
    printf("%f ",cpu_time);
#else
    printf("Gaussian blur[5x5][prefetch sse original structure], execution time : %f ms , with %d times Gaussian blur\n",cpu_time,execution_times);
#endif
#endif
#if FILTER(GAUSSIAN,256)
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i=0; i<execution_times; i++)
        pt_sse_gaussian_blur_5_ori(BMPSaveData,threadcount,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
#ifdef PERF
    printf("%f ",cpu_time);
#else
    printf("Gaussian blur[5x5][sse pthread original structure], execution time : %f ms , with %d times Gaussian blur\n",cpu_time,execution_times);
#endif
#endif
    printf("\n");

#if FILTER(MIRROR,1)
    color_r = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_g = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    color_b = (unsigned char*)malloc(bmpInfo.biWidth*bmpInfo.biHeight*sizeof(unsigned char));
    split_structure();
    clock_gettime(CLOCK_REALTIME, &start);
    naive_flip_vertical_ori(BMPSaveData,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("flip vertical ori, execution time : %f ms\n", cpu_time);
    clock_gettime(CLOCK_REALTIME, &start);
    naive_flip_vertical_tri(color_r,bmpInfo.biWidth,bmpInfo.biHeight);
    naive_flip_vertical_tri(color_g,bmpInfo.biWidth,bmpInfo.biHeight);
    naive_flip_vertical_tri(color_b,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("flip vertical tri, execution time : %f ms\n", cpu_time);
    clock_gettime(CLOCK_REALTIME, &start);
    omp_flip_vertical_tri(color_r,bmpInfo.biWidth,bmpInfo.biHeight);
    omp_flip_vertical_tri(color_g,bmpInfo.biWidth,bmpInfo.biHeight);
    omp_flip_vertical_tri(color_b,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("omp flip vertical tri, execution time : %f ms\n", cpu_time);
    clock_gettime(CLOCK_REALTIME, &start);
    sse_flip_vertical_tri(color_r,bmpInfo.biWidth,bmpInfo.biHeight);
    sse_flip_vertical_tri(color_g,bmpInfo.biWidth,bmpInfo.biHeight);
    sse_flip_vertical_tri(color_b,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("sse flip vertical tri, execution time : %f ms\n", cpu_time);
    clock_gettime(CLOCK_REALTIME, &start);
    naive_flip_horizontal_ori(BMPSaveData,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("naive flip horizontal ori, execution time : %f ms\n", cpu_time);
    clock_gettime(CLOCK_REALTIME, &start);
    naive_flip_horizontal_tri(color_r,bmpInfo.biWidth,bmpInfo.biHeight);
    naive_flip_horizontal_tri(color_g,bmpInfo.biWidth,bmpInfo.biHeight);
    naive_flip_horizontal_tri(color_b,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("naive flip horizontal tri, execution time : %f ms\n", cpu_time);
    clock_gettime(CLOCK_REALTIME, &start);
    sse_flip_horizontal_tri(color_r,bmpInfo.biWidth,bmpInfo.biHeight);
    sse_flip_horizontal_tri(color_g,bmpInfo.biWidth,bmpInfo.biHeight);
    sse_flip_horizontal_tri(color_b,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("sse flip horizontal tri using, execution time : %f ms\n", cpu_time);
    clock_gettime(CLOCK_REALTIME, &start);
    omp_flip_horizontal_tri(color_r,bmpInfo.biWidth,bmpInfo.biHeight);
    omp_flip_horizontal_tri(color_g,bmpInfo.biWidth,bmpInfo.biHeight);
    omp_flip_horizontal_tri(color_b,bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("omp flip horizontal tri using, execution time : %f ms\n", cpu_time);
    merge_structure();
#endif
#if FILTER(HSV,1)
    clock_gettime(CLOCK_REALTIME, &start);
    change_brightness(BMPSaveData, 1, bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("change brightness: %f ms\n", cpu_time);
    clock_gettime(CLOCK_REALTIME, &start);
    change_saturation(BMPSaveData, 0.5, bmpInfo.biWidth,bmpInfo.biHeight);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time = diff_in_millisecond(start, end);
    printf("change saturation: %f ms\n", cpu_time);
#endif
    // =================== Main Operation to BMP data ===================== //

    // Save Data into output file from BMPSaveData
    if ( saveBMP( outfileName ) ) {
#if PERF
#else
        printf("Save file successfully\n");
#endif
    } else
        printf("Save file failed\n");

    return 0;
}

/*********************************************************/
/* split the original structure                          */
/*********************************************************/
void split_structure()
{
    for(int i=0; i<bmpInfo.biHeight; i++) {
        for(int j=0; j<bmpInfo.biWidth; j++) {
            color_r[i*bmpInfo.biWidth+j] = BMPSaveData[i*bmpInfo.biWidth+j].rgbRed;
            color_g[i*bmpInfo.biWidth+j] = BMPSaveData[i*bmpInfo.biWidth+j].rgbGreen;
            color_b[i*bmpInfo.biWidth+j] = BMPSaveData[i*bmpInfo.biWidth+j].rgbBlue;
        }
    }
}

/*********************************************************/
/* merge the original structure (with choosing filter)   */
/*********************************************************/
void merge_structure()
{
    for(int i=0; i<bmpInfo.biHeight; i++) {
        for(int j=0; j<bmpInfo.biWidth; j++) {
            BMPSaveData[i*bmpInfo.biWidth+j].rgbRed = color_r[i*bmpInfo.biWidth+j];
            BMPSaveData[i*bmpInfo.biWidth+j].rgbGreen = color_g[i*bmpInfo.biWidth+j];
            BMPSaveData[i*bmpInfo.biWidth+j].rgbBlue = color_b[i*bmpInfo.biWidth+j];
        }
    }
}

/*********************************************************/
/* Read BMP                                              */
/*********************************************************/
int readBMP(char *fileName)
{
    // Open BMP File
    FILE *bmpFile = fopen(fileName,"rb");
    // Check the file
    if (!bmpFile) {
        printf("It can't open file!!\n");
        return 0;
    }
    // Read header information into bmpHeader
    fread( &bmpHeader, sizeof( BMPHEADER ) , 1 , bmpFile );
    // Check this image's type
    if( bmpHeader.bfType != 0x4d42 ) {
        printf("This file is not .BMP\n");
        return 0;
    }
    // Read file information into bmpinfo
    fread( &bmpInfo , sizeof( BMPINFO ) , 1 , bmpFile);
#if PERF
#else
    printf("Picture size of picture is width: %d , height %d\n",bmpInfo.biWidth,bmpInfo.biHeight);
#endif

    // check the bit depth is 24 bits or not (bpp)
    if ( bmpInfo.biBitCount != 24 ) {
        printf("This is not 24 bits!!\n");
        return 0;
    }

    // Revise the picture's width size to 4's times
    while( bmpInfo.biWidth % 4 != 0 )
        bmpInfo.biWidth++;

    // Allocate memory to BMPSaveData
    BMPSaveData = alloc_memory( bmpInfo.biHeight, bmpInfo.biWidth);

    // Read the real picture information into BMPSaveData
    fread( BMPSaveData , sizeof(RGBTRIPLE) , bmpInfo.biWidth*bmpInfo.biHeight , bmpFile);

    // close file
    fclose(bmpFile);
    return 1;
}
/*********************************************************/
/* Save BMP                                              */
/*********************************************************/
int saveBMP( char *fileName)
{
    if( bmpHeader.bfType != 0x4d42 ) {
        printf("This file is not .BMP!!\n");
        return 0;
    }

    FILE *newFile = fopen(fileName,"wb");

    if ( !newFile ) {
        printf("The File can't create!!\n");
        return 0;
    }

    // Write header first
    fwrite(&bmpHeader,sizeof(BMPHEADER),1,newFile);
    // Write information
    fwrite(&bmpInfo,sizeof(BMPINFO),1,newFile);
    // Write the picture information
    fwrite(BMPSaveData,sizeof(RGBTRIPLE),bmpInfo.biWidth*bmpInfo.biHeight,newFile);

    // close file
    fclose(newFile);
    return 1;
}
/*********************************************************/
/*  Retrive location                                     */
/*********************************************************/
RGBTRIPLE fetchloc(RGBTRIPLE *arr, int Y, int X)
{
    return arr[bmpInfo.biWidth * Y + X ];
}
/*********************************************************/
/* allocate 1D array memory                              */
/*********************************************************/
RGBTRIPLE *alloc_memory(int Y, int X )
{
    // 2D -> 1D (column number=X , row number=Y)
    RGBTRIPLE *temp = (RGBTRIPLE *)malloc(Y*X*sizeof(RGBTRIPLE));//new RGBTRIPLE [ Y * X ];
    memset( temp, 0, sizeof( RGBTRIPLE ) * Y * X);
    return temp;
}
/*********************************************************/
/* swap data pointer                                     */
/*********************************************************/
void swap(RGBTRIPLE **a, RGBTRIPLE **b)
{
    RGBTRIPLE *temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
/*********************************************************/
/* calculate execution time                              */
/*********************************************************/
static double diff_in_millisecond(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000.0);
}
