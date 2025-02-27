#ifndef FLACENC_H
#define FLACENC_H

#include <string>
#include <map>

#include <FLAC/format.h>
#include <FLAC/stream_encoder.h>

#include "writer.hpp"
#include "option.hpp"

using namespace std;

class Flac : public Writer{
  private: 
    FLAC__StreamEncoder *enc;
    FLAC__StreamMetadata *metadata[2];

  public:
    Flac(const string *, const map<string, string> &, musconv_opts *);
    virtual ~Flac();

    virtual size_t write_file(const int16_t *, size_t) override;
 
    virtual ssize_t add_comments(const map<string, string> &) override;
};
#endif
