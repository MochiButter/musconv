#include "writer.h"

#include "option.h"

using namespace std;

Writer::Writer(musconv_opts *opt){
  samplerate = opt->samplerate;
  channels = opt->channels;
}

Writer::~Writer() {
}
