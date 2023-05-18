# ADPCM Library

I was looking for a complete ADPCM codec library that is suitable to be used on some microcontrollers and supports the most important implementations.

The most complete implementation that I could find is provided by [FFmpeg](https://ffmpeg.org/) but FFMpeg itself is unsuited for microcontrollers.

Therefore I decided to extract the FFmpeg implementation and provide it as a stand alone libarary with a simple API.


# Class Documentation

- [Encoder](https://pschatzmann.github.io/adpcm/docs/html/classadpcm__ffmpeg_1_1ADPCMEncoder.html)
- [Decoder](https://pschatzmann.github.io/adpcm/docs/html/classadpcm__ffmpeg_1_1ADPCMDecoder.html)


# Configuration

I started with all codec variants activated. Unfortunatly this was causing that the ESP32 was running out of stack. I therfore have only activated the most important variants. For details see the config-adpcm.h file.

# Usage

This library can be used directly with cmake or as an Arduino library. However I recommend to use it as [Codec](https://github.com/pschatzmann/arduino-audio-tools/wiki/Encoding-and-Decoding-of-Audio) with the [Arduino Audio Tools](https://github.com/pschatzmann/arduino-audio-tools). 
