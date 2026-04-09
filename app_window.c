#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

typedef float* (*loopfunction)();   // returns FFT array
typedef bool (*conditionfunction)(); 

// ----- Window procedure -----
#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // --- Debug print every message ---

    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CLOSE:
            break;

    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ----- Create window -----
HWND createWindow(int width, int height, const char* title)
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "SpectroWindow";

    RegisterClass(&wc);

    HWND window = CreateWindowEx(
        0,
        "SpectroWindow",
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL,
        wc.hInstance,
        NULL
    );

    ShowWindow(window, SW_SHOW);
    return window;
}

// ----- Render FFT array as bars -----
void renderFFT(HWND hwnd, float* prev){
    // static float lowFactor = 0.7f;
    // static float highFactor = 1.2f;
    // for (int k = 0; k < BUFFER_SIZE/2 +1; k++) {
    // float weight = lowFactor + (highFactor - lowFactor) * ((float)k / (BUFFER_SIZE/2));
    

    // prev[k] *= weight;

    // }
    RECT rect;
    GetClientRect(hwnd, &rect);
    int width = rect.right;
    int height = rect.bottom;

    int numBins = BUFFER_SIZE / 2 + 1;

    // Decide how many bars to render (e.g., one per 2 pixels)
    int barPixelWidth = 3;                  // width of each bar in pixels
    int numBars = width / barPixelWidth;    // total bars to render
    if (numBars > numBins) numBars = numBins; // cannot exceed FFT size
    int binsPerBar = numBins / numBars;     // how many FFT bins per bar

    HDC hdcWindow = GetDC(hwnd);

    // --- double buffering ---
    HDC hdcMem = CreateCompatibleDC(hdcWindow);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcWindow, width, height);
    SelectObject(hdcMem, hBitmap);

    // clear background
    HBRUSH brushBG = CreateSolidBrush(RGB(0,0,0));
    FillRect(hdcMem, &rect, brushBG);
    DeleteObject(brushBG);

    // draw FFT bars
    HBRUSH brushBar = CreateSolidBrush(RGB(0,255,0));
    for (int i = 0; i < numBars; i++)
    {
        float sum = 0.0f;
        for (int j = 0; j < binsPerBar; j++)
        {
            int idx = i * binsPerBar + j;
            if (idx >= numBins) break; // safety check
            sum += prev[idx];
        }
        // sum*=4*i/BUFFER_SIZE;

        float magnitude = sum / binsPerBar;

        if (magnitude > 1.0f) magnitude = 1.0f;
        if (magnitude < 0.0f) magnitude = 0.0f;

        int barHeight = (int)(magnitude * height);

        RECT bar = {
            (LONG)(i * barPixelWidth),
            (height - barHeight),
            (LONG)((i + 1) * barPixelWidth),
            height
        };
        FillRect(hdcMem, &bar, brushBar);
    }
    DeleteObject(brushBar);

    // copy to window in one operation
    BitBlt(hdcWindow, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

    // cleanup
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdcWindow);
}

// ----- Run window loop -----
void runWindowLoop(HWND window, loopfunction f, conditionfunction condition)
{
    MSG msg;
    while (condition())
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (IsIconic(window)){
            Sleep(10); continue;
        }
        float* prev = f();        // get FFT array
        renderFFT(window, prev);  // render to window

        InvalidateRect(window, NULL, TRUE); // force repaint
    }
}




