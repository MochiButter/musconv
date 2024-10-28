#ifndef SELECTOR_H
#define SELECTOR_H

#include <string>
#include <map>

#include "reader.h"
#include "writer.h"
#include "option.h"

using namespace std;

enum WRITESEL{
  WRITER_OPUS = 1
};

Reader *select_reader(string, string, musconv_opts *);
Writer *select_writer(string, WRITESEL, const map<string, string> &, musconv_opts *);
#endif
