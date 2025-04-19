#include "musconv.hpp"

#include <string>
#include <filesystem>
#include <map>
#include <vector>

#include "reader.hpp"
#include "writer.hpp"
#include "selector.hpp"
#include "option.hpp"
#include "fileutil.hpp"

using namespace std;

Musconv::Musconv(const string *source_path, const string *dest_path, musconv_opts *option){
  opt = option;
  const string fext = get_fext(source_path);
  enum readsel dec = get_reader(&fext,opt);
  init_reader(source_path, dec);
  init_writer(dest_path);
}

Musconv::~Musconv(void){
  if (r) delete(r);
  if (w) delete(w);
}

void Musconv::init_reader(const string *path, enum readsel dec){
  const string fext = get_fext(path);
  r = select_reader(dec, path, opt);
  if (!r){
    fprintf(stderr, "Reader fail\n");
    throw runtime_error("Failed creating reader");
  }
  if(opt->auto_comment){
    r->get_comments(&comments);
  }
}

void Musconv::init_writer(const string *path){
  w = select_writer(path, comments, opt);
  if (!w){
    fprintf(stderr, "Writer fail\n");
    throw runtime_error("Failed creating writer");
  }
}

bool Musconv::convert(void){
  int32_t samplerate = opt->samplerate;
  int32_t buffersize = opt->bufsize;
  int32_t fade_seconds = opt->fade_seconds;
  // TODO rename buffer, sample, and frames to be consistent
  // TODO limit vgm channels to two
  vector<int16_t> buffer(buffersize * opt->channels);

  size_t time_count = 0;
  size_t time_max = 0;
  if(opt->play_seconds){
    // play for n samples, which is the seconds specified, minus the fadeout time.
    time_max = opt->samplerate * (opt->play_seconds - opt->fade_seconds);
    printf("Playing for %d seconds => %lu samples\n", opt->play_seconds, time_max);
  }

  while(1){
    size_t c = 0;
    c = r->read_file(buffer.data(), buffersize);
    time_count += c;
    if(time_max && time_count > time_max){
      size_t write_diff;
      write_diff = time_count - time_max;
      w->write_file(buffer.data(),write_diff);
      break;
    }
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
    size_t i, j;
    int32_t channels = opt->channels;
    while(samples_left >= buffersize){
      r->read_file(buffer.data(), buffersize);
      samples_left -= buffersize;
      if(samples_left < buffersize){
        wrsize = samples_left;
      }
      for(i = 0; i < wrsize; i++){
        volume = (samples_left + buffersize - i) / (float)fade_samples;
        // scale volume for each interleaved frame
        for(j = 0; j < (size_t)channels; j++){
          buffer[channels * i + j] = (int16_t)(buffer[channels * i + j] * volume);
        }
      }
      w->write_file(buffer.data(), wrsize);
    }
  }
  return true;
}
