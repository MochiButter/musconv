#include "vgm.hpp"

#include <stdio.h>
#include <stdbool.h>

#include <fstream>
#include <string>
#include <vector>

#include <player/playera.hpp>

#include "reader.h"
#include "option.h"

using namespace std;

Vgm::Vgm(string path, musconv_opts *opt) : Reader(opt){
  (void) opt;
  printf("File to read: %s\n", path.c_str());
  printf("Not reading anything at the moment!!\n");
}

Vgm::~Vgm() {
}

// read_count is the frames to render per channel
size_t Vgm::read_file(int16_t *buf, size_t read_count){
  (void) buf;
  (void) read_count;
  if(read_count > Reader::bufsize){
    fprintf(stderr, "Error in Vgm(): bad read count given\n");
    return 0;
  }
  size_t count = 0;
  return count;
}

ssize_t Vgm::get_comments(map<string,string> *comments) const{
  (void) comments;
  ssize_t count = 0;
  return count;
}

// Wrapper for openmpt supported files. No dot in ext
bool Vgm::is_supported(string ext){
  bool supported = false;
  supported |= ext == "dro";
  supported |= ext == "gym";
  supported |= ext == "s98";
  supported |= ext == "vgz";
  supported |= ext == "vgz";
  return supported;
}
