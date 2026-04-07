#include<stdio.h>
#include<stdint.h>
#include<math.h>
#include<stdbool.h>
#include"buffer_process.h"


void UpdateGraph(float *height, const float *buffer){
    
    for (int i =0; i<BUFFER_SIZE/2 +1; i++){
        if (buffer[i]> height[i]) {
            
            height[i] = buffer[i]* 1.9* i/BUFFER_SIZE;
        }
        else height[i] *=0.94f;

        if (height[i]<0.004f) height[i] = 0.0f;
    }
}
