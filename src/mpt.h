#ifndef MPT_H
#define MPT_H

#include <string>
#include <map>

#include <libopenmpt/libopenmpt.hpp>

#include "reader.h"

using namespace std;

class Mpt : public Reader{
  private:
    openmpt::module *mod = nullptr;
  public:
    Mpt(string);
    virtual ~Mpt();

    virtual size_t read_file(int16_t *, size_t) override;
    virtual void play_forever() override;
    
    virtual ssize_t get_comments(map<string,string> *) const override;

    static bool is_supported(string);
};
#endif
