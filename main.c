#include<stdio.h>
#include<stdlib.h>
#include<portaudio.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include"stream.h"
#include"buffer_process.h"
#include"graph.h"

#define NUM_BINS 32
#define MAX_HEIGHT 22


void DrawSpectroGraph(float *heightmap) {
    int numBins = NUM_BINS;
    int maxHeight = MAX_HEIGHT;
    int binsPerBar = (BUFFER_SIZE / 2 + 1) / numBins;
    int heights[NUM_BINS];

    // compute heights directly from current FFT frame
    for (int i = 0; i < numBins; i++) {
        float sum = 0.0f;
        for (int j = 0; j < binsPerBar; j++) {
            int idx = i * binsPerBar + j;
            if (idx > BUFFER_SIZE / 2) break;
            sum += heightmap[idx];
        }
        float val = sum / binsPerBar;

        // scale to display height
        heights[i] = (int)(val * maxHeight* 2.0f);
        if (heights[i] > maxHeight) heights[i] = maxHeight;
    }

    // move cursor to top-left for live update
    printf("\033[H\n");

    // draw vertical bars
    for (int row = maxHeight; row > 0; row--) {
        for (int i = 0; i < numBins; i++) {
            if (heights[i] >= row) printf(" |");
            else printf("  ");
        }
        printf("\n");
    }

    for (int i = 0; i < numBins; i++) printf("--");
    printf("\n");
    fflush(stdout);
}

int main(){
    PaStream *stream;

    initStream(&stream);
    Pa_StartStream(stream);

    time_t t1 = time(NULL);
    FFTProc proc = InitFFTProc();
    unsigned long long t=0;
    float prev[BUFFER_SIZE/2 +1] = {0};
    while (time(NULL)-t1<300){

        float buffer[NUM_CHANNELS*BUFFER_SIZE];

        readStream(stream,buffer, BUFFER_SIZE);

        processBuffer(buffer, &proc);
        // float avg = 0;
        // for (int i = 0; i <= BUFFER_SIZE/2; i++) {
        //     avg+=proc.mags[i];
        // }
        // avg = avg/(BUFFER_SIZE/2);
        // printf("%f ", avg);
        // for (int i =0; i<10*avg;i++){
        //     printf("#");
        // }
        // printf("\n");

        UpdateGraph(prev, proc.mags);

        if (t++%2==0) // reduce input overflow
        DrawSpectroGraph(prev);
        printf("%d ", t/2);
        // Pa_Sleep(8);

    }
    return 0;
    
}