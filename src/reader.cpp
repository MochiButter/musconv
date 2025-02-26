#include "reader.hpp"

#include <iostream>
#include <string>

#include "option.hpp"

using namespace std;

Reader::Reader(musconv_opts *opt){
  samplerate = opt->samplerate;
  bufsize = opt->bufsize;
  repeat_count = opt->repeat_count;
  fade_seconds = opt->fade_seconds;
  play_seconds = opt->play_seconds;
  channels = opt->channels;
}
Reader::~Reader(){
}
