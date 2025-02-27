#ifndef MPT_H
#define MPT_H

#include <string>
#include <map>

#include <libopenmpt/libopenmpt.hpp>

#include "reader.hpp"
#include "option.hpp"

using namespace std;

class Mpt : public Reader{
  private:
    openmpt::module *mod = NULL;
  public:
    Mpt(const string *, musconv_opts *);
    virtual ~Mpt();

    virtual size_t read_file(int16_t *, size_t) override;
    
    virtual ssize_t get_comments(map<string,string> *) const override;

    static bool is_supported(const string);
    static void print_supported(void);
};
#endif
