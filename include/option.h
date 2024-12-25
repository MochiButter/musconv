#ifndef OPTION_H
#define OPTION_H

#include <stddef.h>
#include <stdint.h>

enum writesel{
  WRITER_OPUS,
  WRITER_FLAC
};

typedef struct{
  /* for both reader and writer */
  int32_t samplerate;
  int32_t channels;

  /* for the reader */
  size_t bufsize;
  int32_t repeat_count;
  //int32_t interpolation;
  //int32_t gain;

  /* for the writer */
  enum writesel encoder;
  
  /* for main */
  int32_t fade_seconds;
  int32_t play_seconds;
  char *filename; 
  char *artist;
  char *title;
  char *date;
  char *out_template;
  bool auto_comment;
  bool print_sub;
  bool print_meta;
  bool dry_run;
  bool quiet;
} musconv_opts;

void init_opts(musconv_opts *);
void print_opts(musconv_opts *);

#endif
