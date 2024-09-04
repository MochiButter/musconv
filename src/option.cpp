#include <stdio.h>

#include "option.h"

void init_opts(musconv_opts *opt){
  opt->samplerate = 48000;
  opt->channels = 2;
  // buffersize based on opus guidelines (20ms for 48kHz)
  // TODO find a good value for any output
  opt->bufsize = 960;
  opt->repeat_count = 0;
  //opt->interpolation = ;
  //opt->gain =;
  opt->fade_seconds = 0;
  //opt->channels = 2;
  opt->filename = NULL;
  opt->artist = NULL;
  opt->title = NULL;
  opt->date = NULL;
  opt->auto_comment = false;
  opt->out_template = NULL;
  opt->print_sub = false;
  opt->print_meta = false;
  opt-> dry_run = false;
  opt->quiet = false;
}

void print_opts(musconv_opts *opt){
  printf("Samplerate: %dHz\n", opt->samplerate);
  printf("Buffer size: %lu => %.2f ms\n", opt->bufsize, ((float)opt->bufsize / opt->samplerate) * 1000);
  printf("Loops: %d\n", opt->repeat_count);
  printf("Fade out: %d seconds\n", opt->fade_seconds);
  printf("Auto comment: %s\n", opt->auto_comment?"true":"false");
}
