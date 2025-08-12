#include "fileutil.hpp"

#include <stdbool.h>
#include <stdio.h>

#include <string>
#include <filesystem>
#include <map>
#include <vector>

/*
#include "reader.hpp"
#include "writer.hpp"
#include "selector.hpp"
#include "option.hpp"
*/

using namespace std;

void find_and_replace(string *base, const string find, const string replace){
  size_t ind = 0;
  while((ind = base->find(find, ind)) != string::npos){
    base->replace(ind, find.size(), replace);
    ind += replace.size();
  }
}

string get_fext(const string *path){
  filesystem::path in_path(*path);
  return in_path.extension().string().erase(0,1);
}

string get_output_path(const string *out_template, const string *in_path, enum writesel encoder){
  filesystem::path in_p(*in_path);
  string fstem = in_p.stem().string();

  string out = *out_template;
  find_and_replace(&out, "%(fn)", fstem);
  string fext = "";
  switch(encoder){
    case WRITER_OPUS:
      fext = "opus";
      break;
    case WRITER_FLAC:
      fext = "flac";
      break;
    default:
      // bad args
      break;
  }
  find_and_replace(&out, "%(ext)", fext);
  return out;
}

bool path_validate(string path){
  filesystem::path file_path(path);
  filesystem::file_status file_stat = filesystem::status(file_path);
  filesystem::perms file_perms = file_stat.permissions();

  if ((file_perms &  filesystem::perms::owner_write) != filesystem::perms::owner_write) {
    fprintf(stderr, "Can't write to %s\n", path.c_str());
    return 1;
  }

  if (filesystem::exists(file_stat) && !filesystem::is_regular_file(file_stat)){
    fprintf(stderr, "%s is not a regular file\n", path.c_str());
    return 1;
  }
  return 0;
}

bool file_exists(string path){
  filesystem::path file_path(path);
  return filesystem::exists(file_path);
}

bool create_parent(string path){
  filesystem::path outpath(path);
  if (outpath.has_parent_path()){
    filesystem::create_directories(outpath.parent_path());
  }
  return 0;
}
