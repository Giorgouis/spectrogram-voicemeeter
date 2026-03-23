#include<stdio.h>
#include<stdlib.h>
#include<portaudio.h>
#include<string.h>

// STREAM CONST
#define SAMPLE_RATE 48000
#define FRAMES_PER_BUFFER 512
#define NUM_CHANNELS 2

int initStream(PaStream *stream){
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
                printf("Found Voicemeeter at #%d: %s\n", i, info->name);

                // printf("Max input channels: %d\n", info->maxInputChannels);
                // printf("Default sample rate: %.2f\n", info->defaultSampleRate);
                // printf("Max output channels: %d\n", info->maxOutputChannels);
                // printf("Default low input latency: %f sec\n", info->defaultLowInputLatency);
                // printf("Default high input latency: %f sec\n", info->defaultHighInputLatency);
                // break;
            }
        }
    }

    if (index == -1) {
        printf("VoiceMeeter device not found\n");
        return -1;
    }

    // Open Stream
    err = Pa_OpenDefaultStream(&stream,
                            NUM_CHANNELS,
                            0,
                            paFloat32,
                            SAMPLE_RATE,
                            FRAMES_PER_BUFFER,
                            NULL,
                            NULL);
    
    // check if stream opened
    if (err !=paNoError){
        printf("Error starting stream: %s", Pa_GetErrorText(err));
        return -1;
    }

    printf("Voicemeeter stream initialized");
    return 0;
}

int read_stream(PaStream *stream, float *buffer, unsigned long long frames){
    if (!stream) return -1;
    // read data
    PaError err = Pa_ReadStream(stream, buffer, frames);

    // check data
    if (err !=paNoError){
        if (err == paInputOverflow){
            printf("Input overflow\n");
            return -1;
        }
        else{
            printf("Error reading stream: %s\n", Pa_GetErrorText(err));
            return -1;

        }
    }
    return 0;
}
