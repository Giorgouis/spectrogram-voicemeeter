# Spectogram Voicemeeter

A real-time(-ish) audio spectrometer using PortAudio and KissFFT.

It captures audio from Voicemeeter B1 output and visualizes a spectrogram of frequencies in a window

- Can be changed to suit any channel that portaudio can read the stream from.

(NOTE: making the window wider has a bug I could not find a way to fix)

---


## Dependencies

- PortAudio

- KissFFT



System requirements:

- Windows 10/11

- Visual Studio 2022 (MSVC toolchain)

- CMake ≥ 3.15

---

## Build Instructions

```bash
git clone --recursive https://github.com/Giorgouis/spectrometer-voicemeeter.git
cd spectrometer-voicemeeter
cmake -S . -B build
cmake --build build
```