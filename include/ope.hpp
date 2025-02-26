#ifndef OPE_H
#define OPE_H

#include <string>
#include <map>

#include <opusenc.h>

#include "writer.hpp"
#include "option.hpp"

using namespace std;

class Opus : public Writer{
  private: 
    OggOpusEnc *enc;
    OggOpusComments *comm;

  public:
    Opus(string, const map<string, string> &, musconv_opts *);
    virtual ~Opus();

    virtual size_t write_file(const int16_t *, size_t) override;
 
    virtual ssize_t add_comments(const map<string, string> &) override;
};
#endif
