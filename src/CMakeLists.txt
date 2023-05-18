
# lots of warnings and all warnings as errors
# add_compile_options(-Wall -Wextra )

# define as library
add_library (adpcm_ffmpeg adpcm-ffmpeg/adpcm_data.c adpcm-ffmpeg/adpcm.c adpcm-ffmpeg/adpcmenc.c)
target_compile_options    (adpcm_ffmpeg PUBLIC "-g3")

# define location for header files
target_include_directories(adpcm_ffmpeg PUBLIC . adpcm-ffmpeg )

