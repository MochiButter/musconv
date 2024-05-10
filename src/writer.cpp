#include "writer.h"

#include <iostream>

using namespace std;

Writer::Writer(){
}

Writer::~Writer() {
}

void Writer::set_samplerate(const int32_t _samplerate){
  samplerate = _samplerate;
}

