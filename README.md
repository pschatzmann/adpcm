# ADPCM Library

ADPCM (Adaptive Differential Pulse Code Modulation) is a __lossy codec__ with a good compression ratio and very low requirements in terms of memory and cpu for both the decoding and encoding. It is also purely based on integer arithemtic. It has lost it's imporance on the desktop, but it is still one of the best codec to be used on microcontrollers.

Unfortunately it is not standardized but there are many alternative implementations which are incompatible between each other.

I was looking for a __complete__ ADPCM codec library that is suitable to be used on some microcontrollers and supports the most important implementations.

The best implementation that I could find is provided by [FFmpeg](https://ffmpeg.org/) but unfortunately FFmpeg itself is unsuited for microcontrollers.

Therefore I decided to extract the implementation and provide it as a stand alone libarary with a simple API.

# Usage

This library can be used directly with cmake or as an Arduino library. However I recommend to use it as [Codec](https://github.com/pschatzmann/arduino-audio-tools/wiki/Encoding-and-Decoding-of-Audio) with the [Arduino Audio Tools](https://github.com/pschatzmann/arduino-audio-tools). 


# Class Documentation

- [Encoder](https://pschatzmann.github.io/adpcm/docs/html/classadpcm__ffmpeg_1_1ADPCMEncoder.html)
- [Decoder](https://pschatzmann.github.io/adpcm/docs/html/classadpcm__ffmpeg_1_1ADPCMDecoder.html)


# Configuration

I started with all [codec variants](https://pschatzmann.github.io/adpcm/docs/html/compat__public_8h.html#aadca229ad2c20e060a14fec08a5cc7ce) activated. This was working on the desktop but unfortunatly this was causing that microcontrollers like the ESP32 was running out of stack. 

I therfore left only the most important variants activated and let it up to you to select additional variants. For details see the [config-adpcm.h](https://github.com/pschatzmann/adpcm/blob/main/src/adpcm-ffmpeg/config-adpcm.h) file.

# Examples

The following examples are encoding and decoding a sine tone:

- [Arduino Audio Tools](https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/tests/codecs/test-codec-adpcm/test-codec-adpcm.ino) 
- [CMake](https://github.com/pschatzmann/arduino-audio-tools/tree/main/tests-cmake/codec/adpcm)


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



