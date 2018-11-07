/*
  llzlab - luolongzhi algorithm lab 
  Copyright (C) 2013 luolongzhi 罗龙智 (Chengdu, China)

  This program is part of llzlab, all copyrights are reserved by luolongzhi. 

  filename: main.c 
  time    : 2012/07/14 22:14 
  author  : luolongzhi ( luolongzhi@gmail.com )
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "llz_fft.h"
#include "llz_fft_fixed.h"

#define N 256 //32
/*#define LLZ_FFT_FIXED*/

int main(int argc, char *argv[])
{
	int i;

#ifdef		LLZ_FFT_FIXED
	int buf_in[N];
	int fft_inbuf[2*N];
	uintptr_t handle;
#else
	double buf_in[N];
	double fft_inbuf[2*N];
	uintptr_t handle;
#endif

	for (i = 0 ; i < N ; i++)
		/*buf_in[i] = (1<<25)+i; //i*i;*/
		buf_in[i] = i; //i*i;

	printf("\n");

	for (i = 0 ; i < N ; i++){
		fft_inbuf[2*i] = buf_in[i];
		fft_inbuf[2*i+1] = 0;
	}

#ifdef		LLZ_FFT_FIXED
	handle = llz_fft_fixed_init(N);
	llz_fft_fixed(handle, fft_inbuf);
	llz_ifft_fixed(handle, fft_inbuf);
#else
	handle = llz_fft_init(N);
	llz_fft(handle, fft_inbuf);
	for (i = 0 ; i < N ; i++)
		printf("%f\t  %f\n",fft_inbuf[2*i],fft_inbuf[2*i+1]);
	llz_ifft(handle, fft_inbuf);
#endif


	for (i = 0 ; i < N ; i++)
#ifdef		LLZ_FFT_FIXED
		printf("%d\t  %d\n",fft_inbuf[2*i],fft_inbuf[2*i+1]);
#else
		printf("%f\t  %f\n",fft_inbuf[2*i],fft_inbuf[2*i+1]);
#endif


#ifdef  LLZ_FFT_FIXED
	llz_fft_fixed_uninit(handle);
#else
	llz_fft_uninit(handle);
#endif

    return 0;
}
