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
#include "config-adpcm.h"
#include "compat_public.h"


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

// Some define used by logging
#define AV_LOG_ERROR 16
#define AV_LOG_WARNING 24
#define AV_LOG_INFO 32


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
