/**
 * Compatibility layer: All other data structures and functions that are needed
 * by the adpcm implementation which are usually provided by the ffmpeg
 * infrastructure.
 */

#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compat_public.h"

#define DEBUG 0

#define AV_HAVE_BIGENDIAN 0
#define CACHED_BITSTREAM_READER 0
#define BITSTREAM_READER_LE 1
#define BITSTREAM_WRITER_LE 1

// define some qulifiers used by the code base
#define av_cold
#define av_always_inline inline
#define av_const const
#define av_unused
#define av_alias

#define av_assert(X) assert(X)

#define FFABS(a) ((a) >= 0 ? (a) : (-(a)))
#define FFSIGN(a) ((a) > 0 ? 1 : -1)
#define FFMAX(a, b) ((a) > (b) ? (a) : (b))
#define FFMIN(a, b) ((a) > (b) ? (b) : (a))
#define FFSWAP(type, a, b) \
  do {                     \
    type SWAP_tmp = b;     \
    b = a;                 \
    a = SWAP_tmp;          \
  } while (0)
#define FFALIGN(x, a) (((x) + (a)-1) & ~((a)-1))

#define AVERROR(X) X
enum av_errors {
  AV_OK = 0,
  AVERROR_INVALIDDATA,
  EINVAL,
  AVERROR_PATCHWELCOME,
  ENOMEM
};

/**
 * @ingroup lavc_decoding
 * Required number of additionally allocated bytes at the end of the input
 * bitstream for decoding. This is mainly needed because some optimized
 * bitstream readers read 32 or 64 bit at once and could read over the end.<br>
 * Note: If the first 23 bits of the additional bytes are not 0, then damaged
 * MPEG bitstreams could cause overread and segfault.
 */
#define AV_INPUT_BUFFER_PADDING_SIZE 64

// Some define used by logging
#define AV_LOG_ERROR 16
#define AV_LOG_WARNING 24
#define AV_LOG_INFO 32


// activate the encoders & decoders
#define CONFIG_ADPCM_4XM_DECODER 1
#define CONFIG_ADPCM_ADX_DECODER 1
#define CONFIG_ADPCM_ADX_ENCODER 1
#define CONFIG_ADPCM_AFC_DECODER 1
#define CONFIG_ADPCM_AGM_DECODER 1
#define CONFIG_ADPCM_AICA_DECODER 1
#define CONFIG_ADPCM_ARGO_DECODER 1
#define CONFIG_ADPCM_ARGO_ENCODER 1
#define CONFIG_ADPCM_CT_DECODER 1
#define CONFIG_ADPCM_DTK_DECODER 1
#define CONFIG_ADPCM_EA_DECODER 1
#define CONFIG_ADPCM_EA_MAXIS_XA_DECODER 1
#define CONFIG_ADPCM_EA_R1_DECODER 1
#define CONFIG_ADPCM_EA_R2_DECODER 1
#define CONFIG_ADPCM_EA_R3_DECODER 1
#define CONFIG_ADPCM_EA_XAS_DECODER 1
#define CONFIG_ADPCM_G722_DECODER 1
#define CONFIG_ADPCM_G722_ENCODER 1
#define CONFIG_ADPCM_G726_DECODER 1
#define CONFIG_ADPCM_G726_ENCODER 1
#define CONFIG_ADPCM_G726LE_DECODER 1
#define CONFIG_ADPCM_G726LE_ENCODER 1
#define CONFIG_ADPCM_IMA_ACORN_DECODER 1
#define CONFIG_ADPCM_IMA_AMV_DECODER 1
#define CONFIG_ADPCM_IMA_AMV_ENCODER 1
#define CONFIG_ADPCM_IMA_ALP_DECODER 1
#define CONFIG_ADPCM_IMA_ALP_ENCODER 1
#define CONFIG_ADPCM_IMA_APC_DECODER 1
#define CONFIG_ADPCM_IMA_APM_DECODER 1
#define CONFIG_ADPCM_IMA_APM_ENCODER 1
#define CONFIG_ADPCM_IMA_CUNNING_DECODER 1
#define CONFIG_ADPCM_IMA_DAT4_DECODER 1
#define CONFIG_ADPCM_IMA_DK3_DECODER 1
#define CONFIG_ADPCM_IMA_DK4_DECODER 1
#define CONFIG_ADPCM_IMA_EA_EACS_DECODER 1
#define CONFIG_ADPCM_IMA_EA_SEAD_DECODER 1
#define CONFIG_ADPCM_IMA_ISS_DECODER 1
#define CONFIG_ADPCM_IMA_MOFLEX_DECODER 1
#define CONFIG_ADPCM_IMA_MTF_DECODER 1
#define CONFIG_ADPCM_IMA_OKI_DECODER 1
#define CONFIG_ADPCM_IMA_QT_DECODER 1
#define CONFIG_ADPCM_IMA_QT_ENCODER 1
#define CONFIG_ADPCM_IMA_RAD_DECODER 1
#define CONFIG_ADPCM_IMA_SSI_DECODER 1
#define CONFIG_ADPCM_IMA_SSI_ENCODER 1
#define CONFIG_ADPCM_IMA_SMJPEG_DECODER 1
#define CONFIG_ADPCM_IMA_WAV_DECODER 1
#define CONFIG_ADPCM_IMA_WAV_ENCODER 1
#define CONFIG_ADPCM_IMA_WS_DECODER 1
#define CONFIG_ADPCM_IMA_WS_ENCODER 1
#define CONFIG_ADPCM_MS_DECODER 1
#define CONFIG_ADPCM_MS_ENCODER 1
#define CONFIG_ADPCM_MTAF_DECODER 1
#define CONFIG_ADPCM_PSX_DECODER 1
#define CONFIG_ADPCM_SBPRO_2_DECODER 1
#define CONFIG_ADPCM_SBPRO_3_DECODER 1
#define CONFIG_ADPCM_SBPRO_4_DECODER 1
#define CONFIG_ADPCM_SWF_DECODER 1
#define CONFIG_ADPCM_SWF_ENCODER 1
#define CONFIG_ADPCM_THP_DECODER 1
#define CONFIG_ADPCM_THP_LE_DECODER 1
#define CONFIG_ADPCM_VIMA_DECODER 1
#define CONFIG_ADPCM_XA_DECODER 1
#define CONFIG_ADPCM_XMD_DECODER 1
#define CONFIG_ADPCM_YAMAHA_DECODER 1
#define CONFIG_ADPCM_YAMAHA_ENCODER 1
#define CONFIG_ADPCM_ZORK_DECODER 1

static av_always_inline av_const int av_clip(int a, int amin, int amax) {
#if defined(HAVE_AV_CONFIG_H) && defined(ASSERT_LEVEL) && ASSERT_LEVEL >= 2
  if (amin > amax) abort();
#endif
  if (a < amin)
    return amin;
  else if (a > amax)
    return amax;
  else
    return a;
}

static void *av_malloc(size_t size) { return malloc(size); }

static void av_free(void *ptr) {
#if HAVE_ALIGNED_MALLOC
  _aligned_free(ptr);
#else
  free(ptr);
#endif
}

static void av_freep(void *arg) {
  void *val;

  memcpy(&val, arg, sizeof(val));
  memcpy(arg, &(void *){NULL}, sizeof(val));
  av_free(val);
}

static void *av_mallocz(size_t size) {
  void *ptr = av_malloc(size);
  if (ptr) memset(ptr, 0, size);
  return ptr;
}

static int size_mult(size_t a, size_t b, size_t *r) {
  size_t t;

  t = a * b;
  /* Hack inspired from glibc: don't try the division if nelem and elsize
   * are both less than sqrt(SIZE_MAX). */
  if ((a | b) >= ((size_t)1 << (sizeof(size_t) * 4)) && a && t / a != b)
    return AVERROR(EINVAL);
  *r = t;
  return 0;
}

static void *av_calloc(size_t nmemb, size_t size) {
  size_t result;
  if (size_mult(nmemb, size, &result) < 0) return NULL;
  return av_mallocz(result);
}

static void *av_malloc_array(size_t nmemb, size_t size) {
  size_t result;
  if (size_mult(nmemb, size, &result) < 0) return NULL;
  return av_malloc(result);
}

#define FF_ALLOC_TYPED_ARRAY(p, nelem) (p = av_malloc_array(nelem, sizeof(*p)))
#define FF_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

/**
 * Clip a signed integer value into the 0-65535 range.
 * @param a value to clip
 * @return clipped value
 */
static av_always_inline av_const uint16_t av_clip_uint16(int a) {
  if (a & (~0xFFFF))
    return (~a) >> 31;
  else
    return a;
}

/**
 * Clip a signed integer value into the -32768,32767 range.
 * @param a value to clip
 * @return clipped value
 */
static av_always_inline av_const int16_t av_clip_int16(int a) {
  if ((a + 0x8000U) & ~0xFFFF)
    return (a >> 31) ^ 0x7FFF;
  else
    return a;
}

/**
 * Clip a signed integer into the -(2^p),(2^p-1) range.
 * @param  a value to clip
 * @param  p bit position to clip at
 * @return clipped value
 */
static av_always_inline av_const int av_clip_intp2(int a, int p) {
  if (((unsigned)a + (1 << p)) & ~((2 << p) - 1))
    return (a >> 31) ^ ((1 << p) - 1);
  else
    return a;
}

/**
 * Clip a signed integer to an unsigned power of two range.
 * @param  a value to clip
 * @param  p bit position to clip at
 * @return clipped value
 */
static av_always_inline av_const unsigned av_clip_uintp2(int a, int p) {
  if (a & ~((1 << p) - 1))
    return (~a) >> 31 & ((1 << p) - 1);
  else
    return a;
}

/**
 * Clear high bits from an unsigned integer starting with specific bit position
 * @param  a value to clip
 * @param  p bit position to clip at
 * @return clipped value
 */
static av_always_inline av_const unsigned av_mod_uintp2(unsigned a,
                                                        unsigned p) {
  return a & ((1U << p) - 1);
}

#ifndef sign_extend
static inline av_const int sign_extend(int val, unsigned bits) {
  unsigned shift = 8 * sizeof(int) - bits;
  union {
    unsigned u;
    int s;
  } v = {(unsigned)val << shift};
  return v.s >> shift;
}
#endif

#ifndef sign_extend64
static inline av_const int64_t sign_extend64(int64_t val, unsigned bits) {
  unsigned shift = 8 * sizeof(int64_t) - bits;
  union {
    uint64_t u;
    int64_t s;
  } v = {(uint64_t)val << shift};
  return v.s >> shift;
}
#endif

#ifndef zero_extend
static inline av_const unsigned zero_extend(unsigned val, unsigned bits) {
  return (val << ((8 * sizeof(int)) - bits)) >> ((8 * sizeof(int)) - bits);
}
#endif

static int ff_get_encode_buffer(AVCodecContext *avctx, AVPacket *avpkt, int64_t size,
                         int flags) {
  avpkt->size = size;
  return 0;
}

static int ff_get_buffer(AVCodecContext *avctx, AVFrame *frame, int flags) {
  return 0;
}

//  error message about missing feature
static void avpriv_request_sample(void *avc, const char *msg, ...) {
  printf("%s", msg);
}

#define av_log(A, B, ...) printf(__VA_ARGS__);

static int av_get_exact_bits_per_sample(enum AVCodecID codec_id) {
  switch (codec_id) {}
}


#define NEG_USR32 NEG_USR32
static inline uint32_t NEG_USR32(uint32_t a, int8_t s) {
  __asm__("shrl %1, %0\n\t" : "+r"(a) : "ic"((uint8_t)(-s)));
  return a;
}
