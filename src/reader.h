#ifndef READER_H
#define READER_H

#include <string>
#include <map>

using namespace std;

class Reader{
  protected:
    size_t bufsize = 960;
    int32_t repeat_count = 0;
    int32_t samplerate = 48000;
    int32_t fade_seconds = 0;

  public:
    Reader();
    virtual ~Reader();

    virtual size_t read_file(int16_t *, size_t) = 0;
    virtual void play_forever() = 0;

    virtual ssize_t get_comments(map<string,string> *) const = 0;

    void set_bufsize(size_t);
    void set_repeat_count(int32_t);
    void set_samplerate(int32_t);
    void set_fadeout(int32_t);
};
#endif
