#ifndef STREAM_G_H
#define STREAM_G_H

#include<stdio.h>
#include<stdlib.h>
#include<portaudio.h>

#define SAMPLE_RATE 48000
#define FRAMES_PER_BUFFER 512
#define NUM_CHANNELS 2

void initStream(PaStream *stream);

#endif