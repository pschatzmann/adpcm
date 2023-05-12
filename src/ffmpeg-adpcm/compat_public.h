/**
 * Compatibility layer: All other public data structures and functions that are
 * needed by the adpcm implementation.
 */
#pragma once
#include <stdint.h>

/** 
 * @file compat_public.h
 * public data structures
 */


/**
 * This structure stores compressed data. It is typically exported by demuxers
 * and then passed as input to decoders, or received as output from encoders and
 * then passed to muxers.
 *
 */
typedef struct AVPacket {
  uint8_t *data;
  int size;
} AVPacket;

/**
 * This structure provides the uncompressed PCM data
 */
typedef struct AVFrame {
#define AV_NUM_DATA_POINTERS 8
  /**
   * pointer to the picture/channel planes.
   * This might be different from the first allocated byte
   *
   * Some decoders access areas outside 0,0 - width,height, please
   * see avcodec_align_dimensions2(). Some filters and swscale can read
   * up to 16 bytes beyond the planes, if these filters are to be used,
   * then 16 extra bytes must be allocated.
   *
   * NOTE: Except for hwaccel formats, pointers not needed by the format
   * MUST be set to NULL.
   */
  uint8_t *data[AV_NUM_DATA_POINTERS];
  int nb_samples;
  int16_t **extended_data;
} AVFrame;

/**
 * @brief Supported Codec IDs
 * @enum Supported Codec IDs
 */
enum AVCodecID {
  /* various ADPCM codecs */
  AV_CODEC_ID_ADPCM_IMA_QT = 0x11000,
  AV_CODEC_ID_ADPCM_IMA_WAV,
  AV_CODEC_ID_ADPCM_IMA_DK3,
  AV_CODEC_ID_ADPCM_IMA_DK4,
  AV_CODEC_ID_ADPCM_IMA_WS,
  AV_CODEC_ID_ADPCM_IMA_SMJPEG,
  AV_CODEC_ID_ADPCM_MS,
  AV_CODEC_ID_ADPCM_4XM,
  AV_CODEC_ID_ADPCM_XA,
  AV_CODEC_ID_ADPCM_ADX,
  AV_CODEC_ID_ADPCM_EA,
  AV_CODEC_ID_ADPCM_G726,
  AV_CODEC_ID_ADPCM_CT,
  AV_CODEC_ID_ADPCM_SWF,
  AV_CODEC_ID_ADPCM_YAMAHA,
  AV_CODEC_ID_ADPCM_SBPRO_4,
  AV_CODEC_ID_ADPCM_SBPRO_3,
  AV_CODEC_ID_ADPCM_SBPRO_2,
  AV_CODEC_ID_ADPCM_THP,
  AV_CODEC_ID_ADPCM_IMA_AMV,
  AV_CODEC_ID_ADPCM_EA_R1,
  AV_CODEC_ID_ADPCM_EA_R3,
  AV_CODEC_ID_ADPCM_EA_R2,
  AV_CODEC_ID_ADPCM_IMA_EA_SEAD,
  AV_CODEC_ID_ADPCM_IMA_EA_EACS,
  AV_CODEC_ID_ADPCM_EA_XAS,
  AV_CODEC_ID_ADPCM_EA_MAXIS_XA,
  AV_CODEC_ID_ADPCM_IMA_ISS,
  AV_CODEC_ID_ADPCM_G722,
  AV_CODEC_ID_ADPCM_IMA_APC,
  AV_CODEC_ID_ADPCM_VIMA,
  AV_CODEC_ID_ADPCM_AFC,
  AV_CODEC_ID_ADPCM_IMA_OKI,
  AV_CODEC_ID_ADPCM_DTK,
  AV_CODEC_ID_ADPCM_IMA_RAD,
  AV_CODEC_ID_ADPCM_G726LE,
  AV_CODEC_ID_ADPCM_THP_LE,
  AV_CODEC_ID_ADPCM_PSX,
  AV_CODEC_ID_ADPCM_AICA,
  AV_CODEC_ID_ADPCM_IMA_DAT4,
  AV_CODEC_ID_ADPCM_MTAF,
  AV_CODEC_ID_ADPCM_AGM,
  AV_CODEC_ID_ADPCM_ARGO,
  AV_CODEC_ID_ADPCM_IMA_SSI,
  AV_CODEC_ID_ADPCM_ZORK,
  AV_CODEC_ID_ADPCM_IMA_APM,
  AV_CODEC_ID_ADPCM_IMA_ALP,
  AV_CODEC_ID_ADPCM_IMA_MTF,
  AV_CODEC_ID_ADPCM_IMA_CUNNING,
  AV_CODEC_ID_ADPCM_IMA_MOFLEX,
  AV_CODEC_ID_ADPCM_IMA_ACORN,
  AV_CODEC_ID_ADPCM_XMD,
};

enum AVChannelLayout {
  AV_CHANNEL_LAYOUT_MONO,
  AV_CHANNEL_LAYOUT_STEREO,
};

typedef struct Codec {
  enum AVCodecID id;
} Codec;


typedef struct AVCodecContext {
  int trellis;
  int frame_size;
  int block_align;
  uint8_t *extradata;
  int extradata_size;
  Codec *codec;
  enum AVCodecID codec_id;
  void *priv_data;
  int nb_channels;
  int bits_per_coded_sample;
  int sample_rate;
  int sample_fmt;
} AVCodecContext;

enum AVSampleFormat {
  AV_SAMPLE_FMT_NONE = -1,
  AV_SAMPLE_FMT_U8,   ///< unsigned 8 bits
  AV_SAMPLE_FMT_S16,  ///< signed 16 bits
  AV_SAMPLE_FMT_S32,  ///< signed 32 bits
  AV_SAMPLE_FMT_FLT,  ///< float
  AV_SAMPLE_FMT_DBL,  ///< double

  AV_SAMPLE_FMT_U8P,   ///< unsigned 8 bits, planar
  AV_SAMPLE_FMT_S16P,  ///< signed 16 bits, planar
  AV_SAMPLE_FMT_S32P,  ///< signed 32 bits, planar
  AV_SAMPLE_FMT_FLTP,  ///< float, planar
  AV_SAMPLE_FMT_DBLP,  ///< double, planar
  AV_SAMPLE_FMT_S64,   ///< signed 64 bits
  AV_SAMPLE_FMT_S64P,  ///< signed 64 bits, planar

  AV_SAMPLE_FMT_NB  ///< Number of sample formats. DO NOT USE if linking
                    ///< dynamically
};
