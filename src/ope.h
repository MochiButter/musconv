#ifndef OPE_H
#define OPE_H

#include <string>
#include <map>

#include <opusenc.h>

#include "writer.h"

using namespace std;

class Opus : public Writer{
  private: 
    OggOpusEnc *enc;
    OggOpusComments *comm;
    int32_t channels = 2;

  public:
    Opus(string, const map<string, string> &);
    virtual ~Opus();

    virtual size_t write_file(const int16_t *, const size_t) override;
 
    virtual ssize_t add_comments(const map<string, string> &) override;
};
#endif
