/**
 * Compatibility layer: All public data structures and functions that are
 * needed by the adpcm implementation and are usually provided by the ffmpeg
 * infrastructure.
 */
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file compat_public.h
 * public data structures
 */

typedef struct ADPCMChannelStatus {
  int predictor;
  int16_t step_index;
  int step;
  /* for encoding */
  int prev_sample;

  /* MS version */
  int sample1;
  int sample2;
  int coeff1;
  int coeff2;
  int idelta;
} ADPCMChannelStatus;

/**
 * @brief This structure stores compressed data. It is typically exported by
 * demuxers and then passed as input to decoders, or received as output from
 * encoders and then passed to muxers.
 *
 */
typedef struct AVPacket {
  uint8_t *data;
  int size;
} AVPacket;

/**
 * @brief This structure provides the uncompressed PCM data
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
  /**
   * number of audio samples (per channel) described by this frame
   */

  int nb_samples;
  /**
   * pointers to the data planes/channels.
   *
   * For video, this should simply point to data[].
   *
   * For planar audio, each channel has a separate data pointer, and
   * linesize[0] contains the size of each channel buffer.
   * For packed audio, there is just one data pointer, and linesize[0]
   * contains the total size of the buffer for all channels.
   *
   * Note: Both data and extended_data should always be set in a valid frame,
   * but for planar audio with more channels that can fit in data,
   * extended_data must be used in order to access all channels.
   */

  int16_t **extended_data;
} AVFrame;

/**
 * @brief Supported Codec IDs
 * @enum Supported Codec IDs
 */
enum AVCodecID {
  /* various ADPCM codecs */
  AV_CODEC_ID_ADPCM_IMA_QT = 0x11000, ///< IMA_QT
  AV_CODEC_ID_ADPCM_IMA_WAV, ///< IMA_WAV
  AV_CODEC_ID_ADPCM_IMA_DK3, ///< DK3
  AV_CODEC_ID_ADPCM_IMA_DK4, ///< DK4
  AV_CODEC_ID_ADPCM_IMA_WS, ///< WS
  AV_CODEC_ID_ADPCM_IMA_SMJPEG, ///< SMJPEG 
  AV_CODEC_ID_ADPCM_MS, ///< MS
  AV_CODEC_ID_ADPCM_4XM, ///< 4XM
  AV_CODEC_ID_ADPCM_XA, ///< XA
  AV_CODEC_ID_ADPCM_ADX, ///< ADX
  AV_CODEC_ID_ADPCM_EA, ///< EA
  AV_CODEC_ID_ADPCM_G726, ///< G726
  AV_CODEC_ID_ADPCM_CT, ///< CT
  AV_CODEC_ID_ADPCM_SWF, ///< SWF
  AV_CODEC_ID_ADPCM_YAMAHA, ///< YAHAMA
  AV_CODEC_ID_ADPCM_SBPRO_4, ///< SBRO 4 
  AV_CODEC_ID_ADPCM_SBPRO_3, ///< SBRO 3
  AV_CODEC_ID_ADPCM_SBPRO_2, ///< SBRO 2 
  AV_CODEC_ID_ADPCM_THP, ///< THP
  AV_CODEC_ID_ADPCM_IMA_AMV, ///< AMV
  AV_CODEC_ID_ADPCM_EA_R1, ///< R1
  AV_CODEC_ID_ADPCM_EA_R3, ///< R3
  AV_CODEC_ID_ADPCM_EA_R2, ///< R2
  AV_CODEC_ID_ADPCM_IMA_EA_SEAD, ///< SEAD
  AV_CODEC_ID_ADPCM_IMA_EA_EACS, ///< EACS 
  AV_CODEC_ID_ADPCM_EA_XAS, ///< XAS
  AV_CODEC_ID_ADPCM_EA_MAXIS_XA, ///< MAXIS_XA 
  AV_CODEC_ID_ADPCM_IMA_ISS, ///< ISS
  AV_CODEC_ID_ADPCM_G722, ///< G722
  AV_CODEC_ID_ADPCM_IMA_APC, ///< APC
  AV_CODEC_ID_ADPCM_VIMA, ///< VIMA
  AV_CODEC_ID_ADPCM_AFC, ///< AFC
  AV_CODEC_ID_ADPCM_IMA_OKI, ///< OKI 
  AV_CODEC_ID_ADPCM_DTK, ///< DTK
  AV_CODEC_ID_ADPCM_IMA_RAD, ///< IMA_RAD
  AV_CODEC_ID_ADPCM_G726LE, ///< G726
  AV_CODEC_ID_ADPCM_THP_LE, ///< THP_LE
  AV_CODEC_ID_ADPCM_PSX, ///< PSX
  AV_CODEC_ID_ADPCM_AICA, ///< AICA
  AV_CODEC_ID_ADPCM_IMA_DAT4, ///< DAT4
  AV_CODEC_ID_ADPCM_MTAF, ///< MTAF
  AV_CODEC_ID_ADPCM_AGM, ///< AGM
  AV_CODEC_ID_ADPCM_ARGO, ///< ARGO
  AV_CODEC_ID_ADPCM_IMA_SSI, ///< SSI
  AV_CODEC_ID_ADPCM_ZORK, ///< ZORK
  AV_CODEC_ID_ADPCM_IMA_APM, ///< APM
  AV_CODEC_ID_ADPCM_IMA_ALP, ///< ALP
  AV_CODEC_ID_ADPCM_IMA_MTF, ///< MTF
  AV_CODEC_ID_ADPCM_IMA_CUNNING, ///< CUNNING
  AV_CODEC_ID_ADPCM_IMA_MOFLEX, ///< MOFLX
  AV_CODEC_ID_ADPCM_IMA_ACORN, ///< ACORN 
  AV_CODEC_ID_ADPCM_XMD, ///< XMD
};

enum AVChannelLayout {
  AV_CHANNEL_LAYOUT_MONO, ///< mono: 1 channel
  AV_CHANNEL_LAYOUT_STEREO, ///< stereo: 2 channels
};

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


typedef struct TrellisPath {
  int nibble;
  int prev;
} TrellisPath;

typedef struct TrellisNode {
  uint32_t ssd;
  int path;
  int sample1;
  int sample2;
  int step;
} TrellisNode;

// typedef struct AVClass {
//   int sample_rate;
// } AVClass;

typedef struct ADPCMEncodeContext {
  // AVClass *class;
  int block_size;

  ADPCMChannelStatus status[6];
  TrellisPath *paths;
  TrellisNode *node_buf;
  TrellisNode **nodep_buf;
  uint8_t *trellis_hash;
} ADPCMEncodeContext;

/**
 * @brief C API context for encoder and decoder
*/

typedef struct AVCodecContext {
  int trellis;
  int frame_size;
  int block_align;
  uint8_t *extradata;
  int extradata_size;
//  Codec *codec;
  enum AVCodecID codec_id;
  void *priv_data;
  int nb_channels;
  int bits_per_coded_sample;
  int sample_rate;
  int sample_fmt;
} AVCodecContext;


static int av_get_bits_per_sample(enum AVCodecID codec_id) {
  switch (codec_id) {
    case AV_CODEC_ID_ADPCM_SBPRO_2:
      return 2;
    case AV_CODEC_ID_ADPCM_SBPRO_3:
      return 3;
    case AV_CODEC_ID_ADPCM_SBPRO_4:
    case AV_CODEC_ID_ADPCM_IMA_WAV:
    case AV_CODEC_ID_ADPCM_IMA_QT:
    case AV_CODEC_ID_ADPCM_SWF:
    case AV_CODEC_ID_ADPCM_MS:
    case AV_CODEC_ID_ADPCM_ARGO:
    case AV_CODEC_ID_ADPCM_CT:
    case AV_CODEC_ID_ADPCM_IMA_ALP:
    case AV_CODEC_ID_ADPCM_IMA_AMV:
    case AV_CODEC_ID_ADPCM_IMA_APC:
    case AV_CODEC_ID_ADPCM_IMA_APM:
    case AV_CODEC_ID_ADPCM_IMA_EA_SEAD:
    case AV_CODEC_ID_ADPCM_IMA_OKI:
    case AV_CODEC_ID_ADPCM_IMA_WS:
    case AV_CODEC_ID_ADPCM_IMA_SSI:
    case AV_CODEC_ID_ADPCM_G722:
    case AV_CODEC_ID_ADPCM_YAMAHA:
    case AV_CODEC_ID_ADPCM_AICA:
      return 4;
    default:
      return 0;
  }
}

#ifdef __cplusplus
}
#endif
