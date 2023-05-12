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
  ADPCMEncoder(AVCodecID id) {
    this->id = id;
    codec.id = id;
    avctx.codec_id = id;
    avctx.codec = &codec;
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
    int got_packet_ptr = 0;
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
  }

  bool begin(int sampleRate, int channels) {
    avctx.sample_rate = sampleRate;
    avctx.nb_channels = channels;
    avctx.sample_fmt = AV_SAMPLE_FMT_S16;
    return adpcm_decode_init(&avctx) == 0;
  }

  int frameSize() { return avctx.frame_size; }

  void end() {}

  AVFrame &decode(int8_t *data, size_t size) {
    packet.size = size;
    packet.data = (uint8_t *)data;
    return decode(packet);
  }

  AVFrame &decode(AVPacket packet) {
    int got_packet_ptr = 0;
    int rc = adpcm_decode_frame(&avctx, &result, &got_packet_ptr, &packet);
    if (rc != 0 || !got_packet_ptr) {
      result.nb_samples = 0;
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
};

}  // namespace adpcm_ffmpeg