#ifndef OPTION_H
#define OPTION_H

#include <stddef.h>
#include <stdint.h>

enum writesel{
  WRITER_OPUS,
  WRITER_FLAC
};

enum readsel{
  READER_MPT,
  READER_VGM
};

typedef struct{
  /* for both reader and writer */
  int32_t samplerate;
  int32_t channels;

  /* for the reader */
  size_t bufsize;
  int32_t repeat_count;
  enum readsel decoder;

  /* for the writer */
  enum writesel encoder;
  
  /* for main */
  int32_t fade_seconds;
  int32_t play_seconds;
  char *out_template;
  bool auto_comment;
  bool dry_run;
  bool quiet;
  bool overwrite;
} musconv_opts;

void init_opts(musconv_opts *);
void print_opts(musconv_opts *);

#endif
