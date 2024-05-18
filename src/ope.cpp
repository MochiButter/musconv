#include "ope.h"

#include <stdio.h>

#include <iostream>
#include <cstring>
#include <string>
#include <map>

#include <opusenc.h>

#include "writer.h"
#include "option.h"

using namespace std;

Opus::Opus(string path, const map<string, string> &comments, musconv_opts *opt) : Writer(opt){
  (void)opt;
  int error = OPE_OK;
  comm = ope_comments_create();
  if(error != OPE_OK){
    fprintf(stderr, "Error in Opus(): failed creating comments\n");
    comm = NULL;
    throw runtime_error("Opus comments failure");
  }
  ssize_t ret = add_comments(comments);
  if(ret < 0){
    ope_comments_destroy(comm);
    fprintf(stderr, "Error in Opus(): failed adding comments\n");
    comm = NULL;
    throw runtime_error("Opus comments failure");
  }

  error = OPE_OK;
  const char *p = path.c_str();
  // surround if 4 channels, mono/stereo if not
  int surround = (Writer::channels == 4)?1:0;
  printf("channels: %d\tsurround: %d\n", Writer::channels, surround);
  enc = ope_encoder_create_file
    (p, comm, (opus_int32)Writer::samplerate, Writer::channels, surround, &error);
  if(error != OPE_OK){
    fprintf(stderr, "Error in Opus(): %s\n", ope_strerror(error));
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

// Count should be the total number of frames rendered, not per channel
size_t Opus::write_file(const int16_t *buf, size_t count){
  int error = OPE_OK;
  //int frame_per_ch = count / Writer::channels;
  //error = ope_encoder_write(enc, buf, frame_per_ch);
  error = ope_encoder_write(enc, buf, count);
  if(error != OPE_OK){
    fprintf(stderr, "Failed writing opus data\n");
    return 1;
  }
  return 0;
}

ssize_t Opus::add_comments(const map<string, string> &comments){
  if(!comm){
    fprintf(stderr, "Opus::add_comments failure: comment object not created\n");
    return -1;
  }
  ssize_t count = 0;
  for(const auto &pair : comments){
    int error = OPE_OK;
    const char *key = pair.first.c_str();
    const char *value = pair.second.c_str();
    error = ope_comments_add(comm, key, value);
    if(error != OPE_OK){
      fprintf(stderr, "Opus::add_comments failure: failed to add a comment\n");
      return -1;
    }
    ++count;
  }
  return count;
}
