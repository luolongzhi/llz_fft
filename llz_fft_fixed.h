/*
  llzlab - luolongzhi algorithm lab 
  Copyright (C) 2013 luolongzhi 罗龙智 (Chengdu, China)

  This program is part of llzlab, all copyrights are reserved by luolongzhi. 

  filename: llz_fft_fixed.h 
  time    : 2012/07/15 14:14 
  author  : luolongzhi ( luolongzhi@gmail.com )
*/

#ifndef _LLZ_FFT_FIXED_H
#define _LLZ_FFT_FIXED_H

#include <math.h>

#ifdef __cplusplus 
extern "C"
{ 
#endif  

#include <stdint.h>


#ifndef _LLZ_INTTYPES_H

#ifndef INT32_MAX
#define INT32_MAX 2147483647
#endif
#ifndef INT32_MIN
#define INT32_MIN (-2147483647 - 1)
#endif

#ifdef __GNUC__ 
#define int64_t long long
#else
#define int64_t __int64 
#endif

//typedef unsigned uintptr_t;

#endif

#ifndef _LLZ_MATHOPS_H
static inline double llz_roundf(double x)
{
    return (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}

static inline int llz_rintf(double x)
{
    double xx;

    xx = llz_roundf(x);

    if     (xx > INT32_MAX) return INT32_MAX;
    else if(xx < INT32_MIN) return INT32_MIN;
    else                    return (int)xx;
}

static inline int llz_clip_int(int a, int amin, int amax)
{
    if      (a < amin) return amin;
    else if (a > amax) return amax;
    else               return a;
}

#define LLZ_SCALE_FLOAT(a, bits)     llz_rintf((a) * (double)(1 << (bits)))
#define LLZ_FIX15(a)                 llz_clip_int(LLZ_SCALE_FLOAT(a, 15), -32767, 32767)
#define LLZ_FIXMUL_32X15(a,b)        ((int)(((int64_t)(a) *(int64_t)(b))>>(15)))

#endif

uintptr_t llz_fft_fixed_init(int size);
void llz_fft_fixed_uninit(uintptr_t handle);

void llz_fft_fixed(uintptr_t handle, int *data);
void llz_ifft_fixed(uintptr_t handle, int* data);

#ifdef __cplusplus 
}
#endif  



#endif
