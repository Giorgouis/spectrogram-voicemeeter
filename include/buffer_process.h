#ifndef BUFFER_PROCESS_G_H
#define BUFFER_PROCESS_G_H
#include"kiss_fft.h"
#include "kiss_fftr.h"
#include"stream.h"
#include<math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct FFTProc{
    kiss_fftr_cfg fft_cfg;
    kiss_fft_scalar in[BUFFER_SIZE];
    kiss_fft_cpx out[BUFFER_SIZE/2 +1];
    float mags[BUFFER_SIZE/2 +1];
    float window[BUFFER_SIZE];
} FFTProc;

FFTProc InitFFTProc();

void processBuffer(float *buffer, FFTProc *proc);

void UpdateGraph(float *heights, const float *buffer);

#endif

