#include<stdio.h>
#include<stdlib.h>
#include<portaudio.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include"stream.h"
#include"buffer_process.h"

#define NUM_BINS 32
#define MAX_HEIGHT 24
#define HISTORY 16

void DrawSpectroGraph(FFTProc *fftProc) {
    static float history[HISTORY][NUM_BINS] = {0};   // store past frames
    static int currentFrame = 0;
    int numBins = NUM_BINS;
    int maxHeight = MAX_HEIGHT;
    int binsPerBar = (BUFFER_SIZE/2 + 1) / numBins;
    int heights[NUM_BINS];

    // push current frame into history (average multiple FFT bins per display bar)
    for (int i = 0; i < numBins; i++) {
        float sum = 0.0f;
        for (int j = 0; j < binsPerBar; j++) {
            int idx = i * binsPerBar + j;
            if (idx > BUFFER_SIZE/2) break;
            sum += fftProc->mags[idx];
        }
        history[currentFrame % HISTORY][i] = sum / binsPerBar;
    }

    // compute heights with slow decrease
    for (int i = 0; i < numBins; i++) {
        // get current value from history
        float newVal = history[currentFrame % HISTORY][i];

        // get previous height (from last frame)
        float oldVal = (currentFrame == 0) ? 0.0f : history[(currentFrame-1+HISTORY) % HISTORY][i];

        // if decreasing sharply (> 0.05), average it with previous to slow down fall
        if (newVal < oldVal - 0.05f) 
            newVal = (newVal + oldVal) / 2.0f;

        // scale to display height
        heights[i] = (int)(newVal * maxHeight* 1.25);
        if (heights[i] > maxHeight) heights[i] = maxHeight;
    }

    currentFrame++;

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

    // optional bottom axis
    for (int i = 0; i < numBins; i++) printf("--");
    printf("\n");
}

int main(){
    PaStream *stream;

    initStream(&stream);
    Pa_StartStream(stream);

    time_t t1 = time(NULL);
    FFTProc proc = InitFFTProc();
    unsigned long long t=0;
    while (time(NULL)-t1<60){

        float buffer[NUM_CHANNELS*BUFFER_SIZE];

        readStream(stream,buffer, BUFFER_SIZE);

        processBuffer(buffer, &proc);
        if (t++%2==0)
        DrawSpectroGraph(&proc);

    }
    return 0;
    
}