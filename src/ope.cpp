#include "ope.h"

#include <cstring>
#include <iostream>
#include <string>
#include <map>

#include <opusenc.h>

#include "writer.h"

using namespace std;

Opus::Opus(string path, const map<string, string> &comments){
  int error = OPE_OK;
  comm = ope_comments_create();
  if(error != OPE_OK){
    cerr << "Failed creating opus comments" << endl;
    comm = NULL;
    throw runtime_error("Opus comments failure");
  }
  ssize_t ret = add_comments(comments);
  if(ret < 0){
    ope_comments_destroy(comm);
    comm = NULL;
    throw runtime_error("Opus comments failure");
  }

  error = OPE_OK;
  const char *p = path.c_str();
  enc = ope_encoder_create_file
    (p, comm, (opus_int32)Writer::samplerate, channels, 0, &error);
  if(error != OPE_OK){
    cerr << "Opus error: " << ope_strerror(error) << endl;
    enc = NULL;
    throw runtime_error("Opus encoder failure");
  }
}

Opus::~Opus() {
  if(enc != NULL){
    ope_encoder_drain(enc);
    ope_encoder_destroy(enc);
  }
  if(comm != NULL)
    ope_comments_destroy(comm);
}

size_t Opus::write_file(const int16_t *buf, const size_t count){
  int error = OPE_OK;
  error = ope_encoder_write(enc, buf, (int)count);
  if(error != OPE_OK){
    cerr << "Failed writing opus data" << endl;
    return 1;
  }
  return 0;
}

ssize_t Opus::add_comments(const map<string, string> &comments){
  if(!comm){
    cerr << "Opus::add_comments failure: comment object not created." << endl;
    return -1;
  }
  ssize_t count = 0;
  for(const auto &pair : comments){
    int error = OPE_OK;
    const char *key = pair.first.c_str();
    const char *value = pair.second.c_str();
    error = ope_comments_add(comm, key, value);
    if(error != OPE_OK){
      cerr << "Opus::add_comments failure: failed to add a comment." << endl;
      return -1;
    }
    ++count;
  }
  return count;
}
