#include<stdio.h>
#include<stdlib.h>
#include<portaudio.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include"stream.h"
#include"buffer_process.h"
#include"app_window.h"

#define NUM_BINS 48
#define MAX_HEIGHT 24

float prev[BUFFER_SIZE/2 +1] = {0};
float buffer[NUM_CHANNELS*BUFFER_SIZE];
PaStream *stream;
FFTProc proc;

bool timer600(){
    static time_t t_start = 0;
    
    if (t_start == 0) t_start = time(NULL);

    bool res = (time(NULL)-t_start) <= 600 ;

    if (!res) t_start = 0;
    return res;
}

float *SpectrometerLoopFunction(){
    readStream(stream,buffer, BUFFER_SIZE);

    processBuffer(buffer, &proc);


    UpdateGraph(prev, proc.mags);
    return prev;
}

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
    

    initStream(&stream);
    Pa_StartStream(stream);

    time_t t1 = time(NULL);
    proc = InitFFTProc();
    unsigned long long t=0;
    HWND window = createWindow(800, 600, "Spectrometer");

    if (!window)
        return -1;

    runWindowLoop(window, SpectrometerLoopFunction, timer600);

    DestroyWindow(window);

    return 0;
    while (time(NULL)-t1<900){

        

        readStream(stream,buffer, BUFFER_SIZE);

        processBuffer(buffer, &proc);


        UpdateGraph(prev, proc.mags);

        if (t++%2==0) // reduce input overflow
        DrawSpectroGraph(prev);
        printf("%d ", t/2);
        Pa_Sleep(8);

    }

    // while (Render_Running()){
    //     readStream(stream, buffer, BUFFER_SIZE);

    //     processBuffer(buffer, &proc);
    //     UpdateGraph(prev, proc.mags);

    //     Render_RequestFrame();
    //     Render_ProcessEvents(prev, BUFFER_SIZE/2 +1);

    //     Sleep(8);
    // }
    return 0;
    
}