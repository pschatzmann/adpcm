/**
 * Just a simple test program where we encode a stereo sine tone and then
 * decode it again and display the result.
 */

#include <assert.h>

#include <iostream>
#include <vector>

#include "ADPCM.h"
#include "SineGenerator.h"

using namespace std;
using namespace adpcm_ffmpeg;

ADPCMDecoder decoder{AV_CODEC_ID_ADPCM_IMA_WAV};
ADPCMEncoder encoder{AV_CODEC_ID_ADPCM_IMA_WAV};
vector<int16_t> frame_vector;
SineWaveGenerator<int16_t> gen{30000.0};
int channels = 1;
int sample_rate = 44100;
int loop_count = 100;

void loadSamples(int frame_size) {
  // fill frame with data
  for (int j = 0; j < frame_size; j += channels) {
    int16_t sample = gen.nextSample();
    for (int ch = 0; ch < channels; ch++) {
      frame_vector[j + ch] = sample;
    }
  }
}

void displayPacket(AVPacket& packet) {
  for (int j = 0; j < packet.size; j++) {
    if (j % 16 == 0) cout << endl;
    printf("0x%x ", packet.data[j]);
  }
  cout << endl;
}

void displayResult(AVFrame& frame) {
  // print the result
  int16_t* data = (int16_t*) frame.data[0];
  size_t frames = frame.nb_samples / channels ;
  for (int j = 0; j < frames; j += channels) {
    for (int ch = 0; ch < channels; ch++) {
      cout << data[j + ch] << " ";
    }
    cout << endl;
  }
}

int checkFrameSize() {
  // determine frame size
  size_t frame_size = encoder.frameSize();
  assert(frame_size == decoder.frameSize());
  assert(frame_size > 0);
  return frame_size;
}

int main() {
  // start sine generator
  gen.begin(sample_rate, 220);

  // open codec
  if (!encoder.begin(sample_rate, channels)) {
    cout << "encoder not supported";
    return -1;
  }
  if (!decoder.begin(sample_rate, channels, encoder.frameSize())) {
    cout << "decoder not supported";
    return -1;
  }

  int frame_size = checkFrameSize();

  // setup data for frame
  frame_vector.resize(frame_size);

  for (int n = 0; n < loop_count; n++) {
    loadSamples(frame_size);
    // encode the data
    size_t samples = frame_size * channels;

    AVPacket& packet = encoder.encode(&frame_vector[0], samples);
    displayPacket(packet);

    // decode
    AVFrame& frame = decoder.decode(packet);
    displayResult(frame);
  }
  // close
  encoder.end();
  decoder.end();
  return 0;
}