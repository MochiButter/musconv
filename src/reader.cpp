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
void Reader::set_bufsize(size_t _bufsize){
  bufsize = _bufsize;
}
void Reader::set_repeat_count(int32_t _repeat_count){
  repeat_count = _repeat_count;
}
void Reader::set_samplerate(int32_t _samplerate){
  samplerate = _samplerate;
}
void Reader::set_fadeout(int32_t _fade_seconds){
  fade_seconds = _fade_seconds;
}
