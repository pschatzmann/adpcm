cmake_minimum_required(VERSION 3.16)

# set the project name
project(adpcm_ffmpeg)

# lots of warnings and all warnings as errors
# add_compile_options(-Wall -Wextra )

# define as library
add_library (adpcm_ffmpeg src/ffmpeg-adpcm/adpcm_data.c src/ffmpeg-adpcm/adpcm.c src/ffmpeg-adpcm/adpcmenc.c)

# define location for header files
target_include_directories(adpcm_ffmpeg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/ffmpeg ${CMAKE_CURRENT_SOURCE_DIR}/src/ )


