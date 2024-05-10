#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <map>

using namespace std;

class Writer{
  protected:
    int32_t samplerate = 48000;

  public:
    Writer();
    virtual ~Writer();

    virtual size_t write_file(const int16_t *, const size_t) = 0;
    virtual ssize_t add_comments(const map<string, string> &) = 0;

    void set_samplerate(const int32_t);
};
#endif
