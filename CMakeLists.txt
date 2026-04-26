cmake_minimum_required(VERSION 3.15)
project(spectrometer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

# --- KissFFT (minimal manual build)
add_library(kissfft STATIC
    external/kissfft/kiss_fft.c
    external/kissfft/kiss_fftr.c
    )

# --- PortAudio (trimmed build)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(PA_BUILD_SHARED ON CACHE BOOL "" FORCE)
set(PA_BUILD_STATIC OFF CACHE BOOL "" FORCE)

add_subdirectory(external/portaudio)

# --- Your app
add_executable(spectrometer
    src/main.c
    src/stream.c
    src/app_window.c
    src/buffer_process.c
)

set_target_properties(spectrometer PROPERTIES WIN32_EXECUTABLE TRUE)

target_include_directories(spectrometer PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/kissfft
)



target_link_libraries(spectrometer PRIVATE
    kissfft
    portaudio
    gdi32
)