#pragma once
#include "adpcm-ffmpeg/adpcm_public.h"
#include "adpcm-ffmpeg/compat_public.h"
#include "cstring"
#include "stddef.h"
#include "vector"

#define ADAPCM_DEFAULT_BLOCK_SIZE 128

#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wsign-compare"

namespace adpcm_ffmpeg {

/**
 * @brief Common ADPCM Functionality
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

class ADPCMCodec {
 public:
  ADPCMCodec() {
    memset(&avctx, 0, sizeof(avctx));
    memset(&enc_ctx, 0, sizeof(enc_ctx));
  }

  AVCodecContext &ctx() { return avctx; }

  void setCodecID(AVCodecID id) {
    this->id = id;
    avctx.codec_id = id;
  }

  void setBlockSize(int size) { enc_ctx.block_size = size; }

  int blockSize() { return enc_ctx.block_size; }

  int frameSize() { return avctx.frame_size; }

 protected:
  AVCodecContext avctx;
  AVCodecID id;
  ADPCMEncodeContext enc_ctx;
};

/**
 * @brief ADPCM Encoder
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class ADPCMEncoder : public ADPCMCodec {
 public:
  ADPCMEncoder() : ADPCMCodec() {
    setBlockSize(ADAPCM_DEFAULT_BLOCK_SIZE);
    setCodecID(AV_CODEC_ID_ADPCM_IMA_WAV);
    avctx.priv_data = (uint8_t *)&enc_ctx;
  }

  ADPCMEncoder(AVCodecID id, int blockSize = ADAPCM_DEFAULT_BLOCK_SIZE)
      : ADPCMCodec() {
    setCodecID(id);
    setBlockSize(blockSize);
    avctx.priv_data = (uint8_t *)&enc_ctx;
  }

  bool begin(int sampleRate, int channels) {
    avctx.sample_rate = sampleRate;
    avctx.nb_channels = channels;
    // avctx.sample_fmt = AV_SAMPLE_FMT_S16;
    bool rc = adpcm_encode_init(&avctx) == 0;
    printf("frame_size: %d", frameSize());
    return rc;
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
      // into separate  frame_extended_data_vector2 arrays
      frame_extended_data_vector1.resize(sampleCount / 2);
      frame_extended_data_vector2.resize(sampleCount / 2);
      extended_data[0] = &frame_extended_data_vector1[0];
      extended_data[1] = &frame_extended_data_vector2[0];
      for (int j = 0; j < sampleCount / 2; j++) {
        frame_extended_data_vector1[j] = data[(j * 2)];
        frame_extended_data_vector2[j] = data[(j * 2) + 1];
      }
    }
    int got_packet_ptr = 0;
    av_packet_data.resize(sampleCount);
    result.data = &av_packet_data[0];
    // std::fill(av_packet_data.begin(), av_packet_data.end(), 0);

    int rc = adpcm_encode_frame(&avctx, &result, &frame, &got_packet_ptr);
    if (rc != 0 || !got_packet_ptr) {
      result.size = 0;
    }
    return result;
  }

 protected:
  AVPacket result;
  AVFrame frame;
  int16_t *extended_data[2] = {0};
  std::vector<uint8_t> av_packet_data;
  std::vector<int16_t> frame_extended_data_vector1;
  std::vector<int16_t> frame_extended_data_vector2;
};

/**
 * @brief ADPCM Decoder
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

class ADPCMDecoder : public ADPCMCodec {
 public:
  ADPCMDecoder() : ADPCMCodec() {
    setBlockSize(ADAPCM_DEFAULT_BLOCK_SIZE);
    setCodecID(AV_CODEC_ID_ADPCM_IMA_WAV);
    avctx.bits_per_coded_sample = av_get_bits_per_sample(id);
    avctx.priv_data = (uint8_t *)&enc_ctx;
  }

  ADPCMDecoder(AVCodecID id, int blockSize = ADAPCM_DEFAULT_BLOCK_SIZE)
      : ADPCMCodec() {
    setCodecID(id);
    setBlockSize(blockSize);
    avctx.bits_per_coded_sample = av_get_bits_per_sample(id);
    avctx.priv_data = (uint8_t *)&enc_ctx;
  }

  bool begin(int sampleRate, int channels) {
    avctx.sample_rate = sampleRate;
    avctx.nb_channels = channels;
    data_source = Undefined;
    // determine frame size
    adpcm_encode_init(&avctx);
    int frame_size = frameSize();
    assert(frame_size != 0);
    // avctx.frame_size = frameSize;
    avctx.sample_fmt = AV_SAMPLE_FMT_S16;
    // setup result frame data
    frame_data_vector.resize(frame_size * channels);
    frame.data[0] = (uint8_t *)&frame_data_vector[0];
    // setup extra_data
    frame_extended_data_vector1.resize(frame_size);
    extended_data[0] = &frame_extended_data_vector1[0];
    if (channels == 2) {
      frame_extended_data_vector2.resize(frame_size);
      extended_data[1] = &frame_extended_data_vector2[0];
    }
    frame.extended_data = extended_data;
    // set result samples
    return adpcm_decode_init(&avctx) == 0;
  }

  void end() {
    adpcm_flush(&avctx);
    // release memory
    frame_data_vector.resize(0);
    frame_extended_data_vector1.resize(0);
    frame_extended_data_vector2.resize(0);
  }

  AVFrame &decode(uint8_t *data, size_t size) {
    packet.size = size;
    packet.data = (uint8_t *)data;
    return decode(packet);
  }

  AVFrame &decode(AVPacket &packet) {
    int got_packet_ptr = 0;

    // clear frame data result
    if (data_source == Undefined) {
      // just reset the data, for the subsequent source determination
      std::fill(frame_data_vector.begin(), frame_data_vector.end(), 0);
      std::fill(frame_extended_data_vector1.begin(),
                frame_extended_data_vector1.end(), 0);
      std::fill(frame_extended_data_vector2.begin(),
                frame_extended_data_vector2.end(), 0);
    }

    int rc = adpcm_decode_frame(&avctx, &frame, &got_packet_ptr, &packet);
    if (rc == 0 || !got_packet_ptr) {
      frame.nb_samples = 0;
    }

    /// determine data source: only once
    if (data_source == Undefined) {
      data_source = getDataSource((int16_t *)(frame.data[0]),
                                  frame.extended_data[0], frame.nb_samples);
    }

    // if data is in exended data, we copy it to the frame_data
    if (data_source == FromExtended) {
      int16_t *result16 = (int16_t *)frame.data[0];
      int pos = 0;
      for (int j = 0; j < frame.nb_samples; j++) {
        for (int ch = 0; ch < avctx.nb_channels; ch++) {
          result16[pos++] = frame.extended_data[ch][j];
        }
      }
    }

    return frame;
  }

 protected:
  enum DataSource { Undefined, FromFrame, FromExtended };
  AVPacket packet;
  AVFrame frame;
  DataSource data_source = Undefined;
  bool is_frame_data = true;
  std::vector<int16_t> frame_data_vector;
  std::vector<int16_t> frame_extended_data_vector1;
  std::vector<int16_t> frame_extended_data_vector2;
  int16_t *extended_data[2] = {NULL};
  uint8_t *data[AV_NUM_DATA_POINTERS] = {NULL};

  /// The result is not returned consistently: sometimes it is in the frame
  /// data, sometimes it is in the extra data. Here we check where it actually
  /// is!
  DataSource getDataSource(int16_t *frame_data, int16_t *ext_data, int len) {
    for (int j = 0; j < len; j++) {
      if (data[j] != 0) return FromFrame;
      if (ext_data[j] != 0) return FromExtended;
    }
    return FromFrame;
  }
};

}  // namespace adpcm_ffmpeg