#include"kiss_fft.h"
#include "kiss_fftr.h"
#include"stream.h"
#include<math.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


typedef struct  {
    kiss_fftr_cfg fft_cfg;
    kiss_fft_scalar in[BUFFER_SIZE];
    kiss_fft_cpx out[BUFFER_SIZE/2 +1];
    float mags[BUFFER_SIZE/2 +1];
    float window[BUFFER_SIZE];
} FFTProc;

FFTProc InitFFTProc(){
    FFTProc proc;
    proc.fft_cfg = kiss_fftr_alloc(BUFFER_SIZE,0,NULL,NULL);

    for (int n = 0; n < BUFFER_SIZE; n++)
        proc.window[n] = 0.5f * (1 - cosf(2 * M_PI * n / (BUFFER_SIZE - 1)));

    return proc;
}

void processBuffer(float *buffer, FFTProc *proc){
    // mono
    for (int i = 0; i < BUFFER_SIZE; i++)
        proc->in[i] = 0.5f * (buffer[i*2] + buffer[i*2 + 1]) * (proc->window[i]);

    kiss_fftr(proc->fft_cfg, proc->in, proc->out);

    // |fft|
    for (int i = 0; i <= BUFFER_SIZE/2; i++)
        proc->mags[i] = sqrtf(proc->out[i].r*proc->out[i].r +
                               proc->out[i].i*proc->out[i].i);
}

void UpdateGraph(float *heights, const float *buffer){
    
    for (int i =0; i<BUFFER_SIZE/2 +1; i++){
        if (buffer[i]> heights[i]) {
            
            heights[i] = buffer[i]* (1.8* i/BUFFER_SIZE);
        }
        else heights[i] *=0.955f; // steady decrease

        if (heights[i]<0.007f) heights[i] = 0.0f; // reduce noise
    }
}