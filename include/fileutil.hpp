#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <string>
#include <map>

#include "option.hpp"

using namespace std;

string get_fext(const string *);
string get_output_path(const string *, const string *, enum writesel);

bool path_validate(string);
bool file_exists(string);
bool create_parent(string);

#endif
