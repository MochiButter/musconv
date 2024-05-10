#include "musconv.h"

#include <string>
#include <map>

#include "selector.h"

using namespace std;

void find_and_replace(string &base, const string &find, const string &replace){
  size_t ind = 0;
  while((ind = base.find(find, ind)) != string::npos){
    base.replace(ind, find.size(), replace);
    ind += replace.size();
  }
}

string get_output_path(const string &outpath, const string &inpath_fstem, WRITESEL encoder, const map<string,string> &comments){
  string out = outpath;
  find_and_replace(out, "%(fn)", inpath_fstem);
  string fext = "";
  switch(encoder){
    case WRITER_OPUS:
      fext = "opus";
      break;
    default:
      // bad args 
      break;
  }
  find_and_replace(out, "%(ext)", fext);
  string buf = "";
  if(comments.find("title") != comments.end()){
    buf = comments.at("title");
  }
  find_and_replace(out, "%(title)", buf);
  buf = "";
  if(comments.find("artist") != comments.end()){
    buf = comments.at("artist");
  }
  find_and_replace(out, "%(artist)", buf);
  return out;
}
