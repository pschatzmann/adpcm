#pragma once

#include "compat_public.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @file adpcm_public.h
 * public methods
 */

// pschatzmann: added public methods
// decoder
/// @brief Init decoder
int adpcm_decode_init(AVCodecContext *avctx);
/// @brief Flush decoder
void adpcm_flush(AVCodecContext *avctx);
/// @brief Decode a pcm frame
int adpcm_decode_frame(AVCodecContext *avctx, AVFrame *frame,
                              int *got_frame_ptr, AVPacket *avpkt);

/// Init encoder
int adpcm_encode_init(AVCodecContext *avctx);
/// @brief Close encoder
int adpcm_encode_close(AVCodecContext *avctx);
/// @brief Encode a single frame
int adpcm_encode_frame(AVCodecContext *avctx, AVPacket *avpkt,
                              const AVFrame *frame, int *got_packet_ptr);

#ifdef __cplusplus
}
#endif
