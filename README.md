# ADPCM Library

I was looking for a complete ADPCM codec library that is suitable to be used on some microcontrollers and supports the most important implementations.

The most complete implementation that I could find is provided by [FFmpeg](https://ffmpeg.org/) but unfortunately FFMpeg itself is unsuited for microcontrollers.

Therefore I decided to extract the implementation and provide it as a stand alone libarary with a simple API.

# Usage

This library can be used directly with cmake or as an Arduino library. However I recommend to use it as [Codec](https://github.com/pschatzmann/arduino-audio-tools/wiki/Encoding-and-Decoding-of-Audio) with the [Arduino Audio Tools](https://github.com/pschatzmann/arduino-audio-tools). 


# Class Documentation

- [Encoder](https://pschatzmann.github.io/adpcm/docs/html/classadpcm__ffmpeg_1_1ADPCMEncoder.html)
- [Decoder](https://pschatzmann.github.io/adpcm/docs/html/classadpcm__ffmpeg_1_1ADPCMDecoder.html)


# Configuration

I started with all codec variants activated. This was working on the desktop but unfortunatly this was causing that microcontrollers like the ESP32 was running out of stack. 

I therfore have only activated the most important variants and let is up to you to select additional variants. For details see the [config-adpcm.h](https://github.com/pschatzmann/adpcm/blob/main/src/adpcm-ffmpeg/config-adpcm.h) file.


# Installation 

## Arduino

You can download the library as zip and call include Library -> zip library. Or you can git clone this project into the Arduino libraries folder e.g. with

```
cd  ~/Documents/Arduino/libraries
git clone https://github.com/pschatzmann/adpcm
```

I recommend to use git because you can easily update to the latest version just by executing the git pull command in the project folder. If you want to use the library in PlatformIO, you can find a detailed description in the Wiki.

## PlatformIO

Just add this library to your platformio.ini. You can also activate or deactivate individual encoders and decoders
```
lib_deps = https://github.com/pschatzmann/adpcm
build_flags = CONFIG_ADPCM_ARGO_DECODER=0
```



## CMake

You can install this library directly from gihub in your CMakeLists.txt

```
project(your-target)

# acivate FetchContent
include(FetchContent)

# install adpcm from github
FetchContent_Declare(adpcm_ffmpeg GIT_REPOSITORY "https://github.com/pschatzmann/adpcm" GIT_TAG main )
FetchContent_GetProperties(adpcm_ffmpeg)
if(NOT adpcm_ffmpeg)
    FetchContent_Populate(adpcm_ffmpeg)
    add_subdirectory(${adpcm_ffmpeg_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/adpcm)
endif()

# activate/deactivate individual encoders or decoders
target_compile_definitions(your-target PUBLIC -DCONFIG_ADPCM_ARGO_DECODER=0)

# specify libraries
target_link_libraries(your-target adpcm_ffmpeg)
...
```



