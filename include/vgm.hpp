#ifndef VGM_H
#define VGM_H

#include <string>
#include <map>

#include <player/playera.hpp>

#include "reader.h"
#include "option.h"

using namespace std;

class Vgm: public Reader{
  private:
    size_t tmp;
    PlayerA player;
  public:
    Vgm(string, musconv_opts *);
    virtual ~Vgm();

    virtual size_t read_file(int16_t *, size_t) override;
    
    virtual ssize_t get_comments(map<string,string> *) const override;

    static bool is_supported(string);
};
#endif
