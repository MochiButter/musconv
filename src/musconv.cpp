#include "musconv.h"

#include <string>
#include <filesystem>
#include <map>
#include <iostream>
#include <vector>

#include "reader.h"
#include "writer.h"
#include "selector.h"
#include "option.h"

using namespace std;

void find_and_replace(string &base, const string &find, const string &replace){
  size_t ind = 0;
  while((ind = base.find(find, ind)) != string::npos){
    base.replace(ind, find.size(), replace);
    ind += replace.size();
  }
}

string get_output_path(const char *outpath, const string &inpath_fstem, WRITESEL encoder, const map<string,string> &comments){
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

bool music_convert(char *path, musconv_opts *opt){
  bool success = true;
  const char *out_template = opt->out_template;
  if(out_template == NULL){
    out_template = "convert/%(fn).%(ext)";
  }
  printf("%s\n",out_template);
  int32_t samplerate = opt->samplerate;
  int32_t buffersize = opt->bufsize;
  int32_t fade_seconds = opt->fade_seconds;
  vector<int16_t> buffer(buffersize * 2);

  filesystem::path p(path);
  string fdir = p.parent_path().string();
  string fstem = p.stem().string();
  string fext = p.extension().string().erase(0,1);

  Reader *r = NULL;
  Writer *w = NULL;
  map<string, string> comments;
  filesystem::path outpath;
  string out;

  // TODO: error catch and throwing 
  r = select_reader(path, fext, opt);
  if(r == NULL){
    success = false;
    goto clean;
  }

  // Get comments from reader
  if(opt->auto_comment){
    r->get_comments(&comments);
  }

  // TODO overwrite [y/N] prompt
  out = get_output_path(out_template, fstem, WRITER_OPUS, comments);
  outpath = filesystem::path(out);
  printf("%s\n", outpath.string().data());
  // check if path is writeable, skip if not
  if((filesystem::perms::owner_write & filesystem::status(outpath).permissions()) 
    != filesystem::perms::owner_write){
    fprintf(stderr, "Not able to write to %s\n", out.data());
    success = false;
    goto clean;
  }
  // creates any directory in path that does not exixt
  if(outpath.has_parent_path()){
    filesystem::create_directories(outpath.parent_path());
  }

  w = select_writer(out, WRITER_OPUS, comments, opt);
  if(w == NULL){
    success = false;
    goto clean;
  }


  printf("Writing to %s\n", out.data());
  while(1){
    size_t c = 0;
    c = r->read_file(buffer.data(), buffersize);
    w->write_file(buffer.data(),c);
    if((int)c < buffersize){
      break;
    }
  }
  if(fade_seconds){
    int32_t fade_samples = fade_seconds * samplerate;
    int32_t samples_left = fade_samples;
    size_t wrsize = buffersize;
    float volume = 0;
    size_t bufpos = 0;
    while(samples_left >= buffersize){
      r->read_file(buffer.data(), buffersize);
      samples_left -= buffersize;
      if(samples_left < buffersize){
        wrsize = samples_left;
      }
      for(size_t i = 0; i < wrsize; i++){
        volume = (samples_left + buffersize - i) / (float)fade_samples;
        bufpos = 2 * i;
        buffer[bufpos] = (int16_t)(buffer[bufpos] * volume);
        buffer[bufpos + 1] = (int16_t)(buffer[bufpos] * volume);
      }
      w->write_file(buffer.data(), wrsize);
    }
  }
clean:
  if(r) delete(r);
  if(w) delete(w);
  return success;
}
