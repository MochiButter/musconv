#ifndef MUSCONV_H
#define MUSCONV_H

#include <string>
#include <map>

#include "selector.h"
#include "option.h"

using namespace std;

void find_and_replace(string &, const string &, const string &);
string get_output_path(const string &, const string &, WRITESEL, const map<string, string> &);

bool music_convert(char *, musconv_opts *);
#endif
