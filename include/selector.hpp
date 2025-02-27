#ifndef SELECTOR_H
#define SELECTOR_H

#include <string>
#include <map>

#include "reader.hpp"
#include "writer.hpp"
#include "option.hpp"

using namespace std;

void supported(void);
Reader *select_reader(const string *, const string *, musconv_opts *);
Writer *select_writer(const string *, const map<string, string> &, musconv_opts *);
#endif
