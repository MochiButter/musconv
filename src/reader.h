#ifndef READER_H
#define READER_H

#include <string>
#include <map>

#include "option.h"

using namespace std;

class Reader{
  protected:
    int32_t samplerate = 48000;
    size_t bufsize = 960;
    int32_t repeat_count = 0;
    int32_t fade_seconds = 0;
    int32_t channels = 2;

  public:
    Reader(musconv_opts *);
    virtual ~Reader();

    virtual size_t read_file(int16_t *, size_t) = 0;

    virtual ssize_t get_comments(map<string,string> *) const = 0;

};
#endif
