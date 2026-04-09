#ifndef APP_WINDOW_G_H
#define APP_WINDOW_G_H


#include <windows.h>
#include <stdbool.h>


// Function pointer types
typedef float* (*loopfunction)();       
typedef bool (*conditionfunction)();    

// Window creation / management
HWND createWindow(int width, int height, const char* title);
void runWindowLoop(HWND window, loopfunction f, conditionfunction condition);



#endif // RENDER_H