#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"kiss_fft.h"
#include<portaudio.h>
#include<string.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// STREAM CONST
#define SAMPLE_RATE 48000
#define BUFFER_SIZE 512
#define NUM_CHANNELS 2

int initStream(PaStream **stream){
    PaError err = Pa_Initialize();
    if (err != paNoError) printf("Portaudio initialization error: %s", Pa_GetErrorText(err));

    int devices = Pa_GetDeviceCount();
    int index = -1;

    // find Voicemeter B1
    for (int i =0; i<devices;i++){
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        if (info && info->maxInputChannels>0){
            if (strstr(info->name, "VoiceMeeter Output")){ // VoiceMeeterOutput = B1
                index = i;
                printf("Voicemeeter at #%d: %s\n", i, info->name);

                // printf("Max input channels: %d\n", info->maxInputChannels);
                // printf("Default sample rate: %.2f\n", info->defaultSampleRate);
                // printf("Max output channels: %d\n", info->maxOutputChannels);
                // printf("Default low input latency: %f sec\n", info->defaultLowInputLatency);
                // printf("Default high input latency: %f sec\n", info->defaultHighInputLatency);
                break;
            }
        }
    }

    if (index == -1) {
        printf("VoiceMeeter device not found\n");
        return -1;
    }
    //test for Pa_OpenStream
    PaStreamParameters inputParams;
    inputParams.device = index;                     // the device you found in the loop
    inputParams.channelCount = NUM_CHANNELS;       // stereo
    inputParams.sampleFormat = paFloat32;          // float samples
    inputParams.suggestedLatency =
        Pa_GetDeviceInfo(index)->defaultLowInputLatency; // choose low latency
inputParams.hostApiSpecificStreamInfo = NULL;  // usually NULL

    // Open Stream
    err = Pa_OpenStream(
    stream,          // pointer to your PaStream*
    &inputParams,     // input parameters
    NULL,             // no output
    SAMPLE_RATE,      // 48000
    BUFFER_SIZE,// buffer size
    paClipOff,        // flags (no clipping)
    NULL,             // no callback (blocking mode)
    NULL              // userData
);
    printf("Reached here\n");
    // check if stream opened
    if (err !=paNoError){
        printf("Error starting stream: %s\n", Pa_GetErrorText(err));
        return -1;
    }

    printf("Voicemeeter stream initialized");
    return 0;
}

int readStream(PaStream *stream, float *buffer){
    static unsigned int times = 0;
    // if (!stream) return -1;
    // read data
    PaError err = Pa_ReadStream(stream, buffer, BUFFER_SIZE);
    // printf("haha %s", Pa_GetErrorText(err));

    // check data
    if (err !=paNoError){
        if (err == paInputOverflow){
            printf("Input overflow\n");
            return -1;
        }
        else{
            printf("Error reading stream: %s %d\n", Pa_GetErrorText(err), ++times);
            return -1;

        }
    }
    // printf("Read stream\n");
    return 0;
}

   
