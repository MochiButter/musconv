#ifndef MUSCONV_H
#define MUSCONV_H

#include <string>
#include <map>

#include "selector.h"

using namespace std;

/*
typedef struct{
  int32_t framesize;
  int32_t samplerate;
  size_t buffersize;
  int32_t repeat_count;
  int32_t interpolation;
  int32_t gain;
  int channels;
  char *filename; 
  char *artist;
  char *title;
  char *date;
  bool auto_comment;
  bool print_sub;
  bool print_meta;
  bool dry_run;
  bool quiet;
}modopus_settings;
*/

void find_and_replace(string &, const string &, const string &);
string get_output_path(const string &, const string &, WRITESEL, const map<string, string> &);
#endif
