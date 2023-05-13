
# build executable
add_executable (test test.cpp)

# find SinGenerator.h
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test )
target_compile_options    (test PUBLIC "-g3")

# add library
target_link_libraries(test PUBLIC adpcm_ffmpeg)
