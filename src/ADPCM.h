#pragma once
#include "adpcm-ffmpeg/adpcm_public.h"
#include "adpcm-ffmpeg/compat_public.h"
#include "stddef.h"

namespace adpcm_ffmpeg {

/**
 * @brief ADPCM Encoder
 */
class ADPCMEncoder {
 public:
  ADPCMEncoder(AVCodecID id, int blockSize = 256) {
    this->id = id;
    codec.id = id;
    avctx.codec_id = id;
    avctx.codec = &codec;
    avctx.priv_data = (uint8_t *)&enc_ctx;
    enc_ctx.block_size = blockSize;
  }

  bool begin(int sampleRate, int channels) {
    avctx.sample_rate = sampleRate;
    avctx.nb_channels = channels;
    avctx.sample_fmt = AV_SAMPLE_FMT_S16;
    return adpcm_encode_init(&avctx) == 0;
  }

  int frameSize() { return avctx.frame_size; }

  void end() { adpcm_encode_close(&avctx); }

  AVPacket &encode(int16_t *data, size_t sampleCount) {
    frame.nb_samples = sampleCount;
    frame.data[0] = (uint8_t *)data;
    frame.extended_data = &data;
    int got_packet_ptr = 0;
    av_packet_data.resize(sampleCount);
    result.data = &av_packet_data[0];
    int rc = adpcm_encode_frame(&avctx, &result, &frame, &got_packet_ptr);
    if (rc != 0 || !got_packet_ptr) {
      result.size = 0;
    }
    return result;
  }

  AVCodecContext &ctx() { return avctx; }

 protected:
  AVCodecID id;
  AVCodecContext avctx;
  AVPacket result;
  AVFrame frame;
  Codec codec;
  ADPCMEncodeContext enc_ctx;
  std::vector<uint8_t> av_packet_data;
};

/**
 * @brief ADPCM Decoder
 */

class ADPCMDecoder {
 public:
  ADPCMDecoder(AVCodecID id) {
    this->id = id;
    codec.id = id;
    avctx.codec_id = id;
    avctx.codec = &codec;
    avctx.bits_per_coded_sample = 4;
    avctx.priv_data = (uint8_t *)&enc_ctx;
    enc_ctx.block_size = 256;
  }

  bool begin(int sampleRate, int channels, int frameSize) {
    avctx.sample_rate = sampleRate;
    avctx.nb_channels = channels;
    avctx.frame_size = frameSize;
    avctx.sample_fmt = AV_SAMPLE_FMT_S16;
    // setup result frame data
    frame_data.resize(frameSize * channels);
    result.data[0] = (uint8_t *)&frame_data[0];
    // setup extra_data
    frame_extra_data1.resize(frameSize);
    ext_data[0] = &frame_extra_data1[0];
    if (channels == 2) {
      frame_extra_data2.resize(frameSize);
      ext_data[1] = &frame_extra_data2[0];
    }
    result.extended_data = ext_data;
    // set result samples
    return adpcm_decode_init(&avctx) == 0;
  }

  int frameSize() { return avctx.frame_size; }

  void end() { adpcm_flush(&avctx); }

  AVFrame &decode(int8_t *data, size_t size) {
    packet.size = size;
    packet.data = (uint8_t *)data;
    return decode(packet);
  }

  AVFrame &decode(AVPacket &packet) {
    int got_packet_ptr = 0;
    result.nb_samples = avctx.frame_size;
    int rc = adpcm_decode_frame(&avctx, &result, &got_packet_ptr, &packet);
    if (rc == 0 || !got_packet_ptr) {
      result.nb_samples = 0;
    } else if (avctx.nb_channels == 2) {
      // int pos=0;
      // for (int j=0;j<result.nb_samples;j++){
      //   result.data[pos++]=result.extended_data[0][j];
      // }
    }
    return result;
  }

  AVCodecContext &ctx() { return avctx; }

 protected:
  AVCodecID id;
  AVCodecContext avctx;
  AVPacket packet;
  AVFrame result;
  Codec codec;
  ADPCMEncodeContext enc_ctx;
  std::vector<int16_t> frame_data;
  std::vector<int16_t> frame_extra_data1;
  std::vector<int16_t> frame_extra_data2;
  int16_t *ext_data[2];
  uint8_t *data[AV_NUM_DATA_POINTERS] = {NULL};
};

}  // namespace adpcm_ffmpeg