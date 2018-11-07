/*
  llzlab - luolongzhi algorithm lab 
  Copyright (C) 2013 luolongzhi 罗龙智 (Chengdu, China)

  This program is part of llzlab, all copyrights are reserved by luolongzhi. 

  filename: llz_fft.c 
  time    : 2012/07/15 14:14 
  author  : luolongzhi ( luolongzhi@gmail.com )
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "llz_fft.h"

#ifndef M_PI
#define  M_PI       3.14159265358979323846
#endif

typedef struct  _llz_fft_ctx_t {
    int   base;
    int   length;
    int   *bit_rvs;
    double *fft_work;
    double *cos_ang;
    double *sin_ang;
}llz_fft_ctx_t;


static int bit_reverse(int *buf_rvs, int size)
{
    int r  = 0;                  // counter
    int s  = 0;                  // bit-reversal of r/2
    int N2 = size << 1;          // N<<1 == N*2
    int i  = 0;

    do {
        buf_rvs[i++] = s; 
        r += 2;
        s ^= size - (size / (r&-r));
    }
    while (r < N2);

    return s;

}

/*
    decimation-in-freq radix-2 in-place butterfly
    data:   (array of double, order below)
    re(0),im(0),re(1),im(1),...,re(size-1),im(size-1)
    means size=array_length/2 

    useage:
    intput in normal order
    output in bit-reversed order
*/
static void dif_butterfly(double *data, long size, double *cos_ang, double *sin_ang)
{
    long  angle, astep, dl;
    double xr, yr, xi, yi, wr, wi, dr, di;
    double *l1, *l2, *end, *ol2;

    astep = 1;
    end   = data + size + size;
    for (dl = size; dl > 1; dl >>= 1, astep += astep) {
        l1 = data;
        l2 = data + dl;
        for (; l2 < end; l1 = l2, l2 = l2 + dl) {
            ol2 = l2;
            for (angle = 0; l1 < ol2; l1 += 2, l2 += 2) {
                wr = cos_ang[angle];
                wi = -sin_ang[angle];
                xr = *l1     + *l2;
                xi = *(l1+1) + *(l2+1);
                dr = *l1     - *l2;
                di = *(l1+1) - *(l2+1);
                yr = dr*wr   - di*wi;
                yi = dr*wi   + di*wr;
                *(l1) = xr; *(l1+1) = xi;
                *(l2) = yr; *(l2+1) = yi;
                angle += astep;
            }
        }
    }
}

/*
    decimation-in-time radix-2 in-place inverse butterfly
    data:   (array of double, below order)
    re(0),im(0),re(1),im(1),...,re(size-1),im(size-1)
    means size=array_length/2 

    useage:
    intput in bit-reversed order
    output in normal order
*/
static void inverse_dit_butterfly(double *data, long size, double *cos_ang, double *sin_ang)
{

    long  angle,astep,dl;
    double xr, yr, xi, yi, wr, wi, dr, di;
    double *l1, *l2, *end, *ol2;

    astep = size >> 1;
    end   = data + size + size;
    for (dl = 2; astep > 0; dl += dl, astep >>= 1) {
        l1 = data;
        l2 = data + dl;
        for (; l2 < end; l1 = l2, l2 = l2 + dl) {
            ol2 = l2;
            for (angle = 0; l1 < ol2; l1 += 2, l2 += 2) {
                wr = cos_ang[angle];
                wi = sin_ang[angle];
                xr = *l1;
                xi = *(l1+1);
                yr = *l2;
                yi = *(l2+1);
                dr = yr*wr - yi*wi;
                di = yr*wi + yi*wr;
                *(l1) = xr + dr; *(l1+1) = xi + di;
                *(l2) = xr - dr; *(l2+1) = xi - di;
                angle += astep;
            }
        }
    }
}



/*
    in-place Radix-2 FFT for complex values
    data:   (array of double, below order)
    re(0),im(0),re(1),im(1),...,re(size-1),im(size-1)
    means size=array_length/2 !!

    output is in similar order, normalized by array length
*/
void llz_fft(uintptr_t handle, double *data)
{
    int i, bit;
    llz_fft_ctx_t *f = (llz_fft_ctx_t *)handle;
    double *temp     = f->fft_work;
    int   size      = f->length;
    int   *bit_rvs  = f->bit_rvs;

    double *cos_ang  = f->cos_ang;
    double *sin_ang  = f->sin_ang;

    dif_butterfly(data, size, cos_ang, sin_ang);

    for (i = 0 ; i < size ; i++) {
        bit = bit_rvs[i];
        temp[i+i] = data[bit+bit]; temp[i+i+1] = data[bit+bit+1];
    }

    for (i = 0; i < size ; i++) {
        data[i+i] = temp[i+i];
        data[i+i+1] = temp[i+i+1];
    }

}


/*
    in-place Radix-2 inverse FFT for complex values
    data:   (array of double, below order)
    re(0),im(0),re(1),im(1),...,re(size-1),im(size-1)
    means size=array_length/2 !!

    output is in similar order, NOT normalized by array length
*/
void llz_ifft(uintptr_t handle, double* data)
{
    int i, bit;
    llz_fft_ctx_t *f = (llz_fft_ctx_t *)handle;
    double *temp     = f->fft_work;
    int   size      = f->length;
    int   *bit_rvs  = f->bit_rvs;

    double *cos_ang  = f->cos_ang;
    double *sin_ang  = f->sin_ang;

    for (i = 0 ; i < size ; i++) {
        bit = bit_rvs[i];
        temp[i+i] = data[bit+bit]; temp[i+i+1] = data[bit+bit+1];
    }

    for (i = 0; i < size ; i++) {
        data[i+i] = temp[i+i]/size;
        data[i+i+1] = temp[i+i+1]/size;
    }

    inverse_dit_butterfly(data, size, cos_ang, sin_ang);
}



uintptr_t llz_fft_init(int size)
{
    int i;
    double ang;
    llz_fft_ctx_t *f = NULL;

    f = (llz_fft_ctx_t *)malloc(sizeof(llz_fft_ctx_t));
    memset(f, 0, sizeof(llz_fft_ctx_t));

    f->length = size;
    f->base   = (int)(log(size)/log(2));

    if ((1<<f->base) < size)
        f->base += 1;

    f->bit_rvs  = (int *)malloc(sizeof(int)*size);
    f->fft_work = (double *)malloc(sizeof(double)*size*2);
    f->cos_ang  = (double *)malloc(sizeof(double)*size);
    f->sin_ang  = (double *)malloc(sizeof(double)*size);

    bit_reverse(f->bit_rvs,size);

    for (i = 0 ; i < size ; i++){
        ang = (double)(2*M_PI*i)/size;
        f->cos_ang[i] = (double)cos(ang);
        f->sin_ang[i] = (double)sin(ang);
    }

    return (uintptr_t)f;
}

void llz_fft_uninit(uintptr_t handle)
{
    llz_fft_ctx_t *f = (llz_fft_ctx_t *)handle;

    free(f->bit_rvs);
    free(f->fft_work);
    free(f->cos_ang);
    free(f->sin_ang);

    f->bit_rvs  = NULL;
    f->fft_work = NULL;
    f->cos_ang  = NULL;
    f->sin_ang  = NULL;

    free(f);
    f = NULL;
}



