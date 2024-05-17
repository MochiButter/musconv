#ifndef OPTION
#define OPTION

#include <stddef.h>
#include <stdint.h>

typedef struct{
  /* for both reader and writer */
  int32_t samplerate;
  size_t bufsize;
  //int channels;

  /* for the reader */
  int32_t repeat_count;
  //int32_t interpolation;
  //int32_t gain;
  
  /* for main */
  int32_t fade_seconds;
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
  bool verbose;
} musconv_opts;

void init_opts(musconv_opts *);
void print_opts(musconv_opts *);

#endif
