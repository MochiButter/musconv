#ifndef MSUCONV_GME_H
#define MSUCONV_GME_H

#include <stdbool.h>

#include <string>
#include <map>

#include "gme/gme.h"

#include "reader.hpp"
#include "option.hpp"

using namespace std;

class Gme: public Reader {
  private:
    Music_Emu *emu;
  public:
    Gme(const string *, musconv_opts *);
    virtual ~Gme();

    virtual size_t read_file(int16_t *, size_t) override;
    virtual ssize_t get_comments(map<string,string> *) const override;

    static bool is_supported(const string);
    static void print_supported(void);
};
#endif
