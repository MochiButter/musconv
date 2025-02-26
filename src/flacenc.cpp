#include "flacenc.hpp"

#include <stdio.h>

#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <vector>

#include <FLAC/format.h>
#include <FLAC/metadata.h>
#include <FLAC/stream_encoder.h>

#include "writer.hpp"
#include "option.hpp"

using namespace std;

Flac::Flac(string path, const map<string, string> &comments, musconv_opts *opt) : Writer(opt){
  FLAC__bool error = true;
  FLAC__StreamEncoderInitStatus init_ret;

  enc = FLAC__stream_encoder_new();
  if(enc == NULL){
    fprintf(stderr, "Error in Flac(): failed creating encoder\n");
    throw runtime_error("Flac create error");
  }
  
  error &= FLAC__stream_encoder_set_verify(enc, true);
  error &= FLAC__stream_encoder_set_compression_level(enc, 6);
  error &= FLAC__stream_encoder_set_channels(enc, Writer::channels);
  error &= FLAC__stream_encoder_set_sample_rate(enc, Writer::samplerate);
  error &= FLAC__stream_encoder_set_bits_per_sample(enc, 16);
  if(!error){
    fprintf(stderr, "Error in Flac(): failed setting stream info\n");
    throw runtime_error("Flac file setting error");
  }

  metadata[0] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT);
  metadata[1] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_PADDING);
  
  ssize_t ret = add_comments(comments);
  if(ret < 0){
    fprintf(stderr, "Error in Flac(): failed adding comments\n");
    throw runtime_error("Flac comments failure");
  }

  const char *p = path.c_str();
  init_ret = FLAC__stream_encoder_init_file(enc, p, NULL, NULL);
  if(init_ret != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
    fprintf(stderr, "Error in Flac(): %s\n", FLAC__StreamEncoderInitStatusString[init_ret]);
    throw runtime_error("Flac file init error");
  }
}

Flac::~Flac() {
  if(enc != NULL){
    FLAC__stream_encoder_finish(enc);
    FLAC__stream_encoder_delete(enc);
  }
  if(metadata[0] != NULL){
    FLAC__metadata_object_delete(metadata[0]);
  }
  if(metadata[1] != NULL){
    FLAC__metadata_object_delete(metadata[1]);
  }
}

// Count should be the total number of frames rendered, not per channel
size_t Flac::write_file(const int16_t *buf, size_t count){
  FLAC__bool error = true;
  vector<FLAC__int32> tmpbuf(Writer::channels * count);
  for(size_t i = 0; i < (Writer::channels * count); i ++){
    tmpbuf[i] = (FLAC__int32)buf[i];
  }

  error = FLAC__stream_encoder_process_interleaved(enc, tmpbuf.data(), count);
  if(!error){
    fprintf(stderr, "Failed writing flac data: %s\n", FLAC__stream_encoder_get_resolved_state_string(enc));
    return 1;
  }
  return 0;
}

ssize_t Flac::add_comments(const map<string, string> &comments){
  FLAC__StreamMetadata_VorbisComment_Entry comm;

  if(enc == NULL){
    fprintf(stderr, "Flac::add_comments failure: encoder not created\n");
    return -1;
  }
  if(metadata[0] == NULL || metadata[1] == NULL){
    fprintf(stderr, "Flac::add_comments failure: metadata object not created\n");
    return -1;
  }

  ssize_t count = 0;
  FLAC__bool error = true;
  for(const auto &pair : comments){
    const char *key = pair.first.c_str();
    const char *value = pair.second.c_str();
    error &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&comm, key, value);
    error &= FLAC__metadata_object_vorbiscomment_append_comment(metadata[0], comm, true);
    if(!error){
      fprintf(stderr, "Flac::add_comments failure: failed to add a comment\n");
      return -1;
    }
    ++count;
  }

  // set padding to 8192
  metadata[1]->length = 8192;

  error = FLAC__stream_encoder_set_metadata(enc, metadata, 2);
  if(!error){
    fprintf(stderr, "Flac:add_comments failure: failed to write metadata to encoder\n");
    return -1;
  }
  return count;
}
