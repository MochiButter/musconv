#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <map>
#include "option.hpp"

using namespace std;

class Writer{
  protected:
    int32_t samplerate = 48000;
    int32_t channels = 2;

  public:
    Writer(musconv_opts *);
    virtual ~Writer();

    virtual size_t write_file(const int16_t *, size_t) = 0;
    virtual ssize_t add_comments(const map<string, string> &) = 0;
};
#endif
