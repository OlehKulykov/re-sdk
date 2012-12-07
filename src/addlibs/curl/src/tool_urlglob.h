#ifndef HEADER_CURL_TOOL_URLGLOB_H
#define HEADER_CURL_TOOL_URLGLOB_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#include "setup.h"

typedef enum {
  UPTSet = 1,
  UPTCharRange,
  UPTNumRange
} URLPatternType;

typedef struct {
  URLPatternType type;
  union {
    struct {
      char **elements;
      short size;
      short ptr_s;
    } Set;
    struct {
      char min_c;
      char max_c;
      char ptr_c;
      int step;
    } CharRange;
    struct {
      int min_n;
      int max_n;
      short padlength;
      int ptr_n;
      int step;
    } NumRange ;
  } content;
} URLPattern;

typedef struct {
  char *literal[10];
  URLPattern pattern[9];
  size_t size;
  size_t urllen;
  char *glob_buffer;
  char beenhere;
  char errormsg[80]; /* error message buffer */
} URLGlob;

int glob_url(URLGlob**, char*, int *, FILE *);
int glob_next_url(char **, URLGlob *);
int glob_match_url(char **, char*, URLGlob *);
void glob_cleanup(URLGlob* glob);

#endif /* HEADER_CURL_TOOL_URLGLOB_H */

