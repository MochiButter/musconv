#include "mpt.hpp"

#include <stdio.h>

#include <fstream>
#include <string>

#include <libopenmpt/libopenmpt.hpp>

#include "reader.hpp"
#include "option.hpp"

using namespace std;

Mpt::Mpt(const string *path, musconv_opts *opt) : Reader(opt){
  try{
    ifstream file(*path, ios::binary);
    mod = new openmpt::module(file);
    file.close();
  } 
  catch(const exception &e){
    fprintf(stderr, "Error in Mpt(): %s\n", e.what());
    throw e;
  }
  mod->set_repeat_count(Reader::repeat_count);
  if(Reader::fade_seconds){
    mod->ctl_set_text("play.at_end", "continue");
  }
  if(Reader::play_seconds){
    mod->set_repeat_count(-1);
  }
}

Mpt::~Mpt() {
  if(mod != NULL)
    delete(mod);
}

// read_count is the frames to render per channel
size_t Mpt::read_file(int16_t *buf, size_t read_count){
  if(read_count > Reader::bufsize){
    fprintf(stderr, "Error in Mpt(): bad read count given\n");
    return 0;
  }
  size_t count = 0;
  switch(Reader::channels){
    case 1:
      count = mod->read(Reader::samplerate, read_count, buf);
      break;
    case 2:
      count = mod->read_interleaved_stereo(Reader::samplerate, read_count, buf);
      break;
    case 4:
      count = mod->read_interleaved_quad(Reader::samplerate, read_count, buf);
      break;
    default:
      //error
      count = 0;
  }
  return count;
}

ssize_t Mpt::get_comments(map<string,string> *comments) const{
  vector<string> keys = mod->get_metadata_keys();
  ssize_t count = 0;
  string comment = "";
  for(size_t i = 0; i < keys.size(); i ++){
    comment = mod->get_metadata(keys[i]);
    if(comment != ""){
      comments->insert_or_assign(keys[i], comment);
      ++count;
    }
  }
  return count;
}

// Wrapper for openmpt supported files. No dot in ext
bool Mpt::is_supported(const string ext){
  return openmpt::is_extension_supported2(ext);
}

void Mpt::print_supported(void){
  printf("Reader \"libopenmpt\" supports:\n\t");
  vector<string> exts = openmpt::get_supported_extensions();
  for(size_t i = 0; i < exts.size(); i ++){
    printf("%s", exts[i].c_str());
    if (i != exts.size() - 1) {
      printf(" ");
    }
  }
  printf("\n");
}
