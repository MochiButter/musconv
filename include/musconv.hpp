#ifndef MUSCONV_H
#define MUSCONV_H

#include <string>
#include <map>

#include "selector.hpp"
#include "option.hpp"

using namespace std;

class Musconv {
  private:
    Reader *r;
    Writer *w;
    musconv_opts *opt;
    map<string, string> comments;
    void init_reader(const string *, enum readsel);
    void init_writer(const string *);
  public:
    Musconv(const string *, const string *, musconv_opts *);
    ~Musconv(void);
    bool convert(void);
};
#endif
