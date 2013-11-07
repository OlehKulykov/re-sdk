// Copyright 2012 Google Inc. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the COPYING file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS. All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
// -----------------------------------------------------------------------------
//
//  simple tool to convert animated GIFs to WebP
//
// Authors: Skal (pascal.massimino@gmail.com)
//          Urvang (urvang@google.com)

#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WEBP_HAVE_GIF

#include <gif_lib.h>
#include "webp/encode.h"
#include "webp/mux.h"
#include "./example_util.h"
#include "./gif2webp_util.h"

#define GIF_TRANSPARENT_MASK 0x01
#define GIF_DISPOSE_MASK     0x07
#define GIF_DISPOSE_SHIFT    2
#define WHITE_COLOR          0xffffffff
#define MAX_CACHE_SIZE       30

//------------------------------------------------------------------------------

// Global variables gathered in a struct.
static int transparent_index = -1;  // Index of transparent color in the map.
static int is_first_frame = 1;     // Whether we are processing the first frame.

static void SanitizeKeyFrameIntervals(size_t* const kmin_ptr,
                                      size_t* const kmax_ptr) {
  size_t kmin = *kmin_ptr;
  size_t kmax = *kmax_ptr;

  if (kmin == 0) kmin = ~0;  // Disable keyframe insertion.
  if (kmax == 0) kmax = ~0;

  if (kmin >= kmax) {
    kmin = kmax - 1;
    fprintf(stderr,
            "WARNING: Setting kmin = %zu, so that kmin < kmax.\n", kmin);
  } else if (kmin < (kmax / 2 + 1)) {
    // This ensures that cache.keyframe + kmin >= kmax is always true. So, we
    // can flush all the frames in the ‘count_since_key_frame == kmax’ case.
    kmin = (kmax / 2 + 1);
    fprintf(stderr,
            "WARNING: Setting kmin = %zu, so that kmin >= kmax / 2 + 1.\n",
            kmin);
  }
  // Limit the max number of frames that are allocated.
  if (kmax - kmin > MAX_CACHE_SIZE) {
    fprintf(stderr,
            "WARNING: Setting kmin = %zu, so that kmax - kmin <= 30.\n", kmin);
    kmin = kmax - MAX_CACHE_SIZE;
  }
  *kmin_ptr = kmin;
  *kmax_ptr = kmax;
}

static void Remap(const uint8_t* const src, const GifFileType* const gif,
                  uint32_t* dst, int len) {
  int i;
  const GifColorType* colors;
  const ColorMapObject* const cmap =
      gif->Image.ColorMap ? gif->Image.ColorMap : gif->SColorMap;
  if (cmap == NULL) return;
  colors = cmap->Colors;

  for (i = 0; i < len; ++i) {
    const GifColorType c = colors[src[i]];
    dst[i] = (src[i] == transparent_index) ? TRANSPARENT_COLOR
           : c.Blue | (c.Green << 8) | (c.Red << 16) | (0xff << 24);
  }
}

// Read the GIF image frame.
static int ReadFrame(GifFileType* const gif, WebPFrameRect* const gif_rect,
                     WebPPicture* const sub_image, WebPPicture* const curr) {
  const GifImageDesc image_desc = gif->Image;
  uint32_t* dst = NULL;
  uint8_t* tmp = NULL;
  int ok = 0;
  WebPFrameRect rect = {
      image_desc.Left, image_desc.Top, image_desc.Width, image_desc.Height
  };
  *gif_rect = rect;

  // Use a view for the sub-picture:
  if (!WebPPictureView(curr, rect.x_offset, rect.y_offset,
                       rect.width, rect.height, sub_image)) {
    fprintf(stderr, "Sub-image %dx%d at position %d,%d is invalid!\n",
            rect.width, rect.height, rect.x_offset, rect.y_offset);
    goto End;
  }
  dst = sub_image->argb;

  tmp = (uint8_t*)malloc(rect.width * sizeof(*tmp));
  if (tmp == NULL) goto End;

  if (image_desc.Interlace) {  // Interlaced image.
    // We need 4 passes, with the following offsets and jumps.
    const int interlace_offsets[] = { 0, 4, 2, 1 };
    const int interlace_jumps[]   = { 8, 8, 4, 2 };
    int pass;
    for (pass = 0; pass < 4; ++pass) {
      int y;
      for (y = interlace_offsets[pass]; y < rect.height;
           y += interlace_jumps[pass]) {
        if (DGifGetLine(gif, tmp, rect.width) == GIF_ERROR) goto End;
        Remap(tmp, gif, dst + y * sub_image->argb_stride, rect.width);
      }
    }
  } else {  // Non-interlaced image.
    int y;
    for (y = 0; y < rect.height; ++y) {
      if (DGifGetLine(gif, tmp, rect.width) == GIF_ERROR) goto End;
      Remap(tmp, gif, dst + y * sub_image->argb_stride, rect.width);
    }
  }
  ok = 1;

 End:
  free(tmp);
  return ok;
}

// Optimize the image frame for WebP and encode it.
static int OptimizeAndEncodeFrame(
    const WebPConfig* const config, const WebPFrameRect* const gif_rect,
    WebPPicture* const curr, WebPPicture* const prev_canvas,
    WebPPicture* const curr_canvas, WebPPicture* const sub_image,
    WebPMuxFrameInfo* const info, WebPFrameCache* const cache) {
  WebPFrameRect rect = *gif_rect;

  // Snap to even offsets (and adjust dimensions if needed).
  rect.width += (rect.x_offset & 1);
  rect.height += (rect.y_offset & 1);
  rect.x_offset &= ~1;
  rect.y_offset &= ~1;

  if (!WebPPictureView(curr, rect.x_offset, rect.y_offset,
                       rect.width, rect.height, sub_image)) {
    return 0;
  }
  info->x_offset = rect.x_offset;
  info->y_offset = rect.y_offset;

  if (is_first_frame || WebPUtilIsKeyFrame(curr, &rect, prev_canvas)) {
    // Add this as a key frame.
    if (!WebPFrameCacheAddFrame(cache, config, NULL, NULL, info, sub_image)) {
      return 0;
    }
    // Update prev_canvas by simply copying from 'curr'.
    WebPUtilCopyPixels(curr, prev_canvas);
  } else {
    if (!config->lossless) {
      // For lossy compression, it's better to replace transparent pixels of
      // 'curr' with actual RGB values, whenever possible.
      WebPUtilReduceTransparency(prev_canvas, &rect, curr);
    }
    if (!WebPFrameCacheShouldTryKeyFrame(cache)) {
      // Add this as a frame rectangle.
      if (!WebPFrameCacheAddFrame(cache, config, info, sub_image, NULL, NULL)) {
        return 0;
      }
      // Update prev_canvas by blending 'curr' into it.
      WebPUtilBlendPixels(curr, gif_rect, prev_canvas);
    } else {
      WebPPicture full_image;
      WebPMuxFrameInfo full_image_info;
      int ok;

      // Convert to a key frame.
      WebPUtilCopyPixels(curr, curr_canvas);
      WebPUtilConvertToKeyFrame(prev_canvas, &rect, curr_canvas);
      if (!WebPPictureView(curr_canvas, rect.x_offset, rect.y_offset,
                           rect.width, rect.height, &full_image)) {
        return 0;
      }
      full_image_info = *info;
      full_image_info.x_offset = rect.x_offset;
      full_image_info.y_offset = rect.y_offset;

      // Add both variants to cache: frame rectangle and key frame.
      ok = WebPFrameCacheAddFrame(cache, config, info, sub_image,
                                  &full_image_info, &full_image);
      WebPPictureFree(&full_image);
      if (!ok) return 0;

      // Update prev_canvas by simply copying from 'curr_canvas'.
      WebPUtilCopyPixels(curr_canvas, prev_canvas);
    }
  }
  return 1;
}

static void DisposeFrame(WebPMuxAnimDispose dispose_method,
                         const WebPFrameRect* const gif_rect,
                         WebPPicture* const frame, WebPPicture* const canvas) {
  if (dispose_method == WEBP_MUX_DISPOSE_BACKGROUND) {
    WebPUtilClearPic(frame, NULL);
    WebPUtilClearPic(canvas, gif_rect);
  }
}

static int GetBackgroundColor(const ColorMapObject* const color_map,
                              GifWord bgcolor_idx, uint32_t* const bgcolor) {
  if (transparent_index != -1 && bgcolor_idx == transparent_index) {
    *bgcolor = TRANSPARENT_COLOR;  // Special case.
    return 1;
  } else if (color_map == NULL || color_map->Colors == NULL
             || bgcolor_idx >= color_map->ColorCount) {
    return 0;  // Invalid color map or index.
  } else {
    const GifColorType color = color_map->Colors[bgcolor_idx];
    *bgcolor = (0xff        << 24)
             | (color.Red   << 16)
             | (color.Green <<  8)
             | (color.Blue  <<  0);
    return 1;
  }
}

static void DisplayGifError(const GifFileType* const gif, int gif_error) {
  // GIFLIB_MAJOR is only defined in libgif >= 4.2.0.
  // libgif 4.2.0 has retired PrintGifError() and added GifErrorString().
#if defined(GIFLIB_MAJOR) && defined(GIFLIB_MINOR) && \
        ((GIFLIB_MAJOR == 4 && GIFLIB_MINOR >= 2) || GIFLIB_MAJOR > 4)
#if GIFLIB_MAJOR >= 5
  // Static string actually, hence the const char* cast.
  const char* error_str = (const char*)GifErrorString(
      (gif == NULL) ? gif_error : gif->Error);
#else
  const char* error_str = (const char*)GifErrorString();
  (void)gif;
#endif
  if (error_str == NULL) error_str = "Unknown error";
  fprintf(stderr, "GIFLib Error %d: %s\n", gif_error, error_str);
#else
  (void)gif;
  fprintf(stderr, "GIFLib Error %d: ", gif_error);
  PrintGifError();
  fprintf(stderr, "\n");
#endif
}

static const char* const kErrorMessages[] = {
  "WEBP_MUX_NOT_FOUND", "WEBP_MUX_INVALID_ARGUMENT", "WEBP_MUX_BAD_DATA",
  "WEBP_MUX_MEMORY_ERROR", "WEBP_MUX_NOT_ENOUGH_DATA"
};

static const char* ErrorString(WebPMuxError err) {
  assert(err <= WEBP_MUX_NOT_FOUND && err >= WEBP_MUX_NOT_ENOUGH_DATA);
  return kErrorMessages[-err];
}

//------------------------------------------------------------------------------

static void Help(void) {
  printf("Usage:\n");
  printf(" gif2webp [options] gif_file -o webp_file\n");
  printf("options:\n");
  printf("  -h / -help  ............ this help\n");
  printf("  -lossy ................. Encode image using lossy compression.\n");
  printf("  -q <float> ............. quality factor (0:small..100:big)\n");
  printf("  -m <int> ............... compression method (0=fast, 6=slowest)\n");
  printf("  -kmin <int> ............ Min distance between key frames\n");
  printf("  -kmax <int> ............ Max distance between key frames\n");
  printf("  -f <int> ............... filter strength (0=off..100)\n");
  printf("\n");
  printf("  -version ............... print version number and exit.\n");
  printf("  -v ..................... verbose.\n");
  printf("  -quiet ................. don't print anything.\n");
  printf("\n");
}

//------------------------------------------------------------------------------

int main(int argc, const char *argv[]) {
  int verbose = 0;
  int gif_error = GIF_ERROR;
  WebPMuxError err = WEBP_MUX_OK;
  int ok = 0;
  const char *in_file = NULL, *out_file = NULL;
  FILE* out = NULL;
  GifFileType* gif = NULL;
  WebPConfig config;
  WebPPicture curr_frame;
  WebPPicture prev_canvas;
  WebPPicture curr_canvas;
  WebPMuxFrameInfo frame;
  WebPMuxAnimParams anim = { WHITE_COLOR, 0 };
  WebPFrameCache* cache = NULL;

  int done;
  int c;
  int quiet = 0;
  WebPMux* mux = NULL;
  WebPData webp_data = { NULL, 0 };
  int stored_icc = 0;  // Whether we have already stored an ICC profile.
  int stored_xmp = 0;

  int default_kmin = 1;  // Whether to use default kmin value.
  int default_kmax = 1;
  size_t kmin = 0;
  size_t kmax = 0;

  memset(&frame, 0, sizeof(frame));
  frame.id = WEBP_CHUNK_ANMF;
  frame.dispose_method = WEBP_MUX_DISPOSE_BACKGROUND;
  frame.blend_method = WEBP_MUX_BLEND;

  if (!WebPConfigInit(&config) || !WebPPictureInit(&curr_frame) ||
      !WebPPictureInit(&prev_canvas) || !WebPPictureInit(&curr_canvas)) {
    fprintf(stderr, "Error! Version mismatch!\n");
    return -1;
  }
  config.lossless = 1;  // Use lossless compression by default.

  if (argc == 1) {
    Help();
    return 0;
  }

  for (c = 1; c < argc; ++c) {
    if (!strcmp(argv[c], "-h") || !strcmp(argv[c], "-help")) {
      Help();
      return 0;
    } else if (!strcmp(argv[c], "-o") && c < argc - 1) {
      out_file = argv[++c];
    } else if (!strcmp(argv[c], "-lossy")) {
      config.lossless = 0;
    } else if (!strcmp(argv[c], "-q") && c < argc - 1) {
      config.quality = (float)strtod(argv[++c], NULL);
    } else if (!strcmp(argv[c], "-m") && c < argc - 1) {
      config.method = strtol(argv[++c], NULL, 0);
    } else if (!strcmp(argv[c], "-kmax") && c < argc - 1) {
      kmax = strtoul(argv[++c], NULL, 0);
      default_kmax = 0;
    } else if (!strcmp(argv[c], "-kmin") && c < argc - 1) {
      kmin = strtoul(argv[++c], NULL, 0);
      default_kmin = 0;
    } else if (!strcmp(argv[c], "-f") && c < argc - 1) {
      config.filter_strength = strtol(argv[++c], NULL, 0);
    } else if (!strcmp(argv[c], "-version")) {
      const int enc_version = WebPGetEncoderVersion();
      const int mux_version = WebPGetMuxVersion();
      printf("WebP Encoder version: %d.%d.%d\nWebP Mux version: %d.%d.%d\n",
             (enc_version >> 16) & 0xff, (enc_version >> 8) & 0xff,
             enc_version & 0xff, (mux_version >> 16) & 0xff,
             (mux_version >> 8) & 0xff, mux_version & 0xff);
      return 0;
    } else if (!strcmp(argv[c], "-quiet")) {
      quiet = 1;
    } else if (!strcmp(argv[c], "-v")) {
      verbose = 1;
    } else if (argv[c][0] == '-') {
      fprintf(stderr, "Error! Unknown option '%s'\n", argv[c]);
      Help();
      return -1;
    } else {
      in_file = argv[c];
    }
  }

  // Appropriate default kmin, kmax values for lossy and lossless.
  if (default_kmin) {
    kmin = config.lossless ? 9 : 3;
  }
  if (default_kmax) {
    kmax = config.lossless ? 17 : 5;
  }
  SanitizeKeyFrameIntervals(&kmin, &kmax);

  cache = WebPFrameCacheNew(kmin, kmax);
  if (cache == NULL) goto End;

  if (!WebPValidateConfig(&config)) {
    fprintf(stderr, "Error! Invalid configuration.\n");
    goto End;
  }

  if (in_file == NULL) {
    fprintf(stderr, "No input file specified!\n");
    Help();
    goto End;
  }

  // Start the decoder object
#if defined(GIFLIB_MAJOR) && (GIFLIB_MAJOR >= 5)
  // There was an API change in version 5.0.0.
  gif = DGifOpenFileName(in_file, &gif_error);
#else
  gif = DGifOpenFileName(in_file);
#endif
  if (gif == NULL) goto End;

  // Allocate current buffer
  curr_frame.width = gif->SWidth;
  curr_frame.height = gif->SHeight;
  curr_frame.use_argb = 1;
  if (!WebPPictureAlloc(&curr_frame)) goto End;
  if (!WebPPictureCopy(&curr_frame, &prev_canvas)) goto End;
  if (!WebPPictureCopy(&curr_frame, &curr_canvas)) goto End;

  mux = WebPMuxNew();
  if (mux == NULL) {
    fprintf(stderr, "ERROR: could not create a mux object.\n");
    goto End;
  }

  // Loop over GIF images
  done = 0;
  do {
    GifRecordType type;
    if (DGifGetRecordType(gif, &type) == GIF_ERROR) goto End;

    switch (type) {
      case IMAGE_DESC_RECORD_TYPE: {
        WebPPicture sub_image;
        WebPFrameRect gif_rect;

        if (!DGifGetImageDesc(gif)) goto End;
        if (!ReadFrame(gif, &gif_rect, &sub_image, &curr_frame)) {
          goto End;
        }

        if (!OptimizeAndEncodeFrame(&config, &gif_rect, &curr_frame,
                                    &prev_canvas, &curr_canvas, &sub_image,
                                    &frame, cache)) {
          fprintf(stderr, "Error! Cannot encode frame as WebP\n");
          fprintf(stderr, "Error code: %d\n", sub_image.error_code);
        }
        WebPPictureFree(&sub_image);

        err = WebPFrameCacheFlush(cache, verbose, mux);
        if (err != WEBP_MUX_OK) {
          fprintf(stderr, "ERROR (%s): Could not add animation frame.\n",
                  ErrorString(err));
          goto End;
        }

        DisposeFrame(frame.dispose_method, &gif_rect,
                     &curr_frame, &prev_canvas);
        is_first_frame = 0;
        break;
      }
      case EXTENSION_RECORD_TYPE: {
        int extension;
        GifByteType *data = NULL;
        if (DGifGetExtension(gif, &extension, &data) == GIF_ERROR) {
          goto End;
        }
        switch (extension) {
          case COMMENT_EXT_FUNC_CODE: {
            break;  // Do nothing for now.
          }
          case GRAPHICS_EXT_FUNC_CODE: {
            const int flags = data[1];
            const int dispose = (flags >> GIF_DISPOSE_SHIFT) & GIF_DISPOSE_MASK;
            const int delay = data[2] | (data[3] << 8);  // In 10 ms units.
            if (data[0] != 4) goto End;
            frame.duration = delay * 10;  // Duration is in 1 ms units for WebP.
            if (dispose == 3) {
              fprintf(stderr, "WARNING: GIF_DISPOSE_RESTORE not supported.");
              // failsafe. TODO(urvang): emulate the correct behaviour by
              // recoding the whole frame.
              frame.dispose_method = WEBP_MUX_DISPOSE_BACKGROUND;
            } else {
              frame.dispose_method =
                  (dispose == 2) ? WEBP_MUX_DISPOSE_BACKGROUND
                                 : WEBP_MUX_DISPOSE_NONE;
            }
            transparent_index = (flags & GIF_TRANSPARENT_MASK) ? data[4] : -1;
            if (is_first_frame) {
              if (!GetBackgroundColor(gif->SColorMap, gif->SBackGroundColor,
                                      &anim.bgcolor)) {
                fprintf(stderr, "GIF decode warning: invalid background color "
                        "index. Assuming white background.\n");
              }
              WebPUtilClearPic(&curr_frame, NULL);
              WebPUtilClearPic(&prev_canvas, NULL);
            }
            break;
          }
          case PLAINTEXT_EXT_FUNC_CODE: {
            break;
          }
          case APPLICATION_EXT_FUNC_CODE: {
            if (data[0] != 11) break;    // Chunk is too short
            if (!memcmp(data + 1, "NETSCAPE2.0", 11)) {
              // Recognize and parse Netscape2.0 NAB extension for loop count.
              if (DGifGetExtensionNext(gif, &data) == GIF_ERROR) goto End;
              if (data == NULL) goto End;  // Loop count sub-block missing.
              if (data[0] != 3 && data[1] != 1) break;   // wrong size/marker
              anim.loop_count = data[2] | (data[3] << 8);
              if (verbose) printf("Loop count: %d\n", anim.loop_count);
            } else {  // An extension containing metadata.
              // We only store the first encountered chunk of each type.
              const int is_xmp =
                  !stored_xmp && !memcmp(data + 1, "XMP DataXMP", 11);
              const int is_icc =
                  !stored_icc && !memcmp(data + 1, "ICCRGBG1012", 11);
              if (is_xmp || is_icc) {
                const char* const fourccs[2] = { "XMP " , "ICCP" };
                const char* const features[2] = { "XMP" , "ICC" };
                WebPData metadata = { NULL, 0 };
                // Construct metadata from sub-blocks.
                // Usual case (including ICC profile): In each sub-block, the
                // first byte specifies its size in bytes (0 to 255) and the
                // rest of the bytes contain the data.
                // Special case for XMP data: In each sub-block, the first byte
                // is also part of the XMP payload. XMP in GIF also has a 257
                // byte padding data. See the XMP specification for details.
                while (1) {
                  WebPData prev_metadata = metadata;
                  WebPData subblock;
                  if (DGifGetExtensionNext(gif, &data) == GIF_ERROR) {
                    WebPDataClear(&metadata);
                    goto End;
                  }
                  if (data == NULL) break;  // Finished.
                  subblock.size = is_xmp ? data[0] + 1 : data[0];
                  assert(subblock.size > 0);
                  subblock.bytes = is_xmp ? data : data + 1;
                  metadata.bytes =
                      (uint8_t*)realloc((void*)metadata.bytes,
                                        prev_metadata.size + subblock.size);
                  if (metadata.bytes == NULL) {
                    WebPDataClear(&prev_metadata);
                    goto End;
                  }
                  metadata.size += subblock.size;
                  memcpy((void*)(metadata.bytes + prev_metadata.size),
                         subblock.bytes, subblock.size);
                }
                if (is_xmp) {
                  // XMP padding data is 0x01, 0xff, 0xfe ... 0x01, 0x00.
                  const size_t xmp_pading_size = 257;
                  if (metadata.size > xmp_pading_size) {
                    metadata.size -= xmp_pading_size;
                  }
                }

                // Add metadata chunk.
                err = WebPMuxSetChunk(mux, fourccs[is_icc], &metadata, 1);
                if (verbose) {
                  printf("%s size: %d\n", features[is_icc], (int)metadata.size);
                }
                WebPDataClear(&metadata);
                if (err != WEBP_MUX_OK) {
                  fprintf(stderr, "ERROR (%s): Could not set %s chunk.\n",
                          ErrorString(err), features[is_icc]);
                  goto End;
                }
                if (is_icc) {
                  stored_icc = 1;
                } else if (is_xmp) {
                  stored_xmp = 1;
                }
              }
            }
            break;
          }
          default: {
            break;  // skip
          }
        }
        while (data != NULL) {
          if (DGifGetExtensionNext(gif, &data) == GIF_ERROR) goto End;
        }
        break;
      }
      case TERMINATE_RECORD_TYPE: {
        done = 1;
        break;
      }
      default: {
        if (verbose) {
          fprintf(stderr, "Skipping over unknown record type %d\n", type);
        }
        break;
      }
    }
  } while (!done);

  // Flush any pending frames.
  err = WebPFrameCacheFlushAll(cache, verbose, mux);
  if (err != WEBP_MUX_OK) {
    fprintf(stderr, "ERROR (%s): Could not add animation frame.\n",
            ErrorString(err));
    goto End;
  }

  // Finish muxing
  err = WebPMuxSetAnimationParams(mux, &anim);
  if (err != WEBP_MUX_OK) {
    fprintf(stderr, "ERROR (%s): Could not set animation parameters.\n",
            ErrorString(err));
    goto End;
  }

  err = WebPMuxAssemble(mux, &webp_data);
  if (err != WEBP_MUX_OK) {
    fprintf(stderr, "ERROR (%s) assembling the WebP file.\n", ErrorString(err));
    goto End;
  }
  if (out_file != NULL) {
    if (!ExUtilWriteFile(out_file, webp_data.bytes, webp_data.size)) {
      fprintf(stderr, "Error writing output file: %s\n", out_file);
      goto End;
    }
    if (!quiet) {
      printf("Saved output file: %s\n", out_file);
    }
  } else {
    if (!quiet) {
      printf("Nothing written; use -o flag to save the result.\n");
    }
  }

  // All OK.
  ok = 1;
  gif_error = GIF_OK;

 End:
  WebPDataClear(&webp_data);
  WebPMuxDelete(mux);
  WebPPictureFree(&curr_frame);
  WebPPictureFree(&prev_canvas);
  WebPPictureFree(&curr_canvas);
  WebPFrameCacheDelete(cache);
  if (out != NULL && out_file != NULL) fclose(out);

  if (gif_error != GIF_OK) {
    DisplayGifError(gif, gif_error);
  }
  if (gif != NULL) {
    DGifCloseFile(gif);
  }

  return !ok;
}

#else  // !WEBP_HAVE_GIF

int main(int argc, const char *argv[]) {
  fprintf(stderr, "GIF support not enabled in %s.\n", argv[0]);
  (void)argc;
  return 0;
}

#endif

//------------------------------------------------------------------------------
