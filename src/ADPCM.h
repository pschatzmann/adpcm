#pragma once
#include "adpcm-ffmpeg/adpcm_public.h"
#include "adpcm-ffmpeg/compat_public.h"
#include "stddef.h"


namespace adpcm_ffmpeg {

/**
 * @brief Common ADPCM Functionality
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

class ADPCMCodec {
 public:
  int frameSize() { return avctx.frame_size; }
  AVCodecContext &ctx() { return avctx; }

 protected:
  AVCodecContext avctx;
};

/**
 * @brief ADPCM Encoder
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class ADPCMEncoder : public ADPCMCodec {
 public:
  ADPCMEncoder(AVCodecID id, int blockSize = 256) {
    this->id = id;
    avctx.codec_id = id;
    avctx.priv_data = (uint8_t *)&enc_ctx;
    enc_ctx.block_size = blockSize;
  }

  bool begin(int sampleRate, int channels) {
    avctx.sample_rate = sampleRate;
    avctx.nb_channels = channels;
    //avctx.sample_fmt = AV_SAMPLE_FMT_S16;
    return adpcm_encode_init(&avctx) == 0;
  }

  void end() { adpcm_encode_close(&avctx); }

  AVPacket &encode(int16_t *data, size_t sampleCount) {
    frame.nb_samples = sampleCount;
    // fill data
    frame.data[0] = (uint8_t *)data;

    // fill extended_data
    frame.extended_data = extended_data;
    if (avctx.nb_channels == 1) {
      extended_data[0] = data;
    } else if (avctx.nb_channels == 2) {
      // if channels is 2 we need to split up the stereo data
      // into separate  frame_extra_data2 arrays
      frame_extra_data1.resize(sampleCount / 2);
      frame_extra_data2.resize(sampleCount / 2);
      extended_data[0] = &frame_extra_data1[0];
      extended_data[1] = &frame_extra_data2[0];
      for (int j = 0; j < sampleCount/2; j++) {
        frame_extra_data1[j] = data[(j*2)];
        frame_extra_data2[j] = data[(j*2) + 1];
      }
    } 
    int got_packet_ptr = 0;
    av_packet_data.resize(sampleCount);
    result.data = &av_packet_data[0];
    std::fill(av_packet_data.begin(), av_packet_data.end(), 0);

    int rc = adpcm_encode_frame(&avctx, &result, &frame, &got_packet_ptr);
    if (rc != 0 || !got_packet_ptr) {
      result.size = 0;
    }
    return result;
  }

 protected:
  AVCodecID id;
  AVPacket result;
  AVFrame frame;
  ADPCMEncodeContext enc_ctx;
  int16_t *extended_data[2] = {0};
  std::vector<uint8_t> av_packet_data;
  std::vector<int16_t> frame_extra_data1;
  std::vector<int16_t> frame_extra_data2;
};

/**
 * @brief ADPCM Decoder
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

class ADPCMDecoder : public ADPCMCodec {
 public:
  ADPCMDecoder(AVCodecID id, int blockSize = 256) {
    this->id = id;
    avctx.codec_id = id;
    avctx.bits_per_coded_sample = av_get_bits_per_sample(id);
    avctx.priv_data = (uint8_t *)&enc_ctx;
    enc_ctx.block_size = blockSize;
  }

  bool begin(int sampleRate, int channels, int frameSize) {
    avctx.sample_rate = sampleRate;
    avctx.nb_channels = channels;
    avctx.frame_size = frameSize;
    avctx.sample_fmt = AV_SAMPLE_FMT_S16;
    // setup result frame data
    frame_data.resize(frameSize * channels);
    frame.data[0] = (uint8_t *)&frame_data[0];
    // setup extra_data
    frame_extra_data1.resize(frameSize);
    ext_data[0] = &frame_extra_data1[0];
    if (channels == 2) {
      frame_extra_data2.resize(frameSize);
      ext_data[1] = &frame_extra_data2[0];
    }
    frame.extended_data = ext_data;
    // set result samples
    return adpcm_decode_init(&avctx) == 0;
  }

  void end() { adpcm_flush(&avctx); }

  AVFrame &decode(int8_t *data, size_t size) {
    packet.size = size;
    packet.data = (uint8_t *)data;
    return decode(packet);
  }

  AVFrame &decode(AVPacket &packet) {
    int got_packet_ptr = 0;
    //frame.nb_samples = avctx.frame_size;

    // clear frame data result
    std::fill(frame_data.begin(), frame_data.end(), 0);
    std::fill(frame_extra_data1.begin(), frame_extra_data1.end(), 0);
    std::fill(frame_extra_data2.begin(), frame_extra_data2.end(), 0);

    int rc = adpcm_decode_frame(&avctx, &frame, &got_packet_ptr, &packet);
    if (rc == 0 || !got_packet_ptr) {
      frame.nb_samples = 0;
    }

    int16_t *result16 = (int16_t *)frame.data[0];
    int pos = 0;
    for (int j = 0; j < frame.nb_samples; j += avctx.nb_channels) {
      for (int ch = 0; ch < avctx.nb_channels; ch++) {
        result16[pos++] = frame.extended_data[ch][j];
      }
    }

    return frame;
  }

 protected:
  AVCodecID id;
  AVPacket packet;
  AVFrame frame;
  ADPCMEncodeContext enc_ctx;
  std::vector<int16_t> frame_data;
  std::vector<int16_t> frame_extra_data1;
  std::vector<int16_t> frame_extra_data2;
  int16_t *ext_data[2];
  uint8_t *data[AV_NUM_DATA_POINTERS] = {NULL};
};

}  // namespace adpcm_ffmpeg