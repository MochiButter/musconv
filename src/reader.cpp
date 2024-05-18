#include "reader.h"

#include <iostream>
#include <string>

#include "option.h"

using namespace std;

Reader::Reader(musconv_opts *opt){
  samplerate = opt->samplerate;
  bufsize = opt->bufsize;
  repeat_count = opt->repeat_count;
  fade_seconds = opt->fade_seconds;
}
Reader::~Reader(){
}
