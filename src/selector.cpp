#include "selector.hpp"

#include <map>
#include <string>

#include "reader.hpp"
#include "writer.hpp"
#include "mpt.hpp"
#include "vgm.hpp"
#include "gme.hpp"
#include "ope.hpp"
#include "flacenc.hpp"
#include "option.hpp"

using namespace std;

/* Decoder type selector.
 * Selects which Reader subclass to use to decode given file, or none if unsupported. 
 */
Reader *select_reader(enum readsel dec, const string *path, musconv_opts *opt){
  Reader *ret = NULL;
  switch(dec){
    case READER_MPT:
      try{
        ret = new Mpt(path, opt);
      }
      catch(exception &e){
        ret = NULL;
      }
      break;
    case READER_VGM:
      try{
        ret = new Vgm(path, opt);
      }
      catch(exception &e){
        ret = NULL;
      }
      break;
    case READER_GME:
      try{
        ret = new Gme(path, opt);
      }
      catch(exception &e){
        ret = NULL;
      }
      break;
    default:
      ret = NULL;
      break;
  }
  return ret;
}

enum readsel get_reader(const string *ext, musconv_opts *opt) {
  enum readsel ret;
  if (Mpt::is_supported(*ext)){
    ret = READER_MPT;
  } else if (Vgm::is_supported(*ext)) {
    ret = READER_VGM;
    // vgm only supports 2 channel outputs
    if (opt->channels != 2) {
      opt->channels = 2;
      printf("Warning: libvgm supports 2 channel rendering only!\n");
    }
  } else if (Gme::is_supported(*ext)) {
    ret = READER_GME;
  }
  return ret;
}

void supported(void){
  Mpt::print_supported();
  Vgm::print_supported();
  Gme::print_supported();
}

/* Encoder type selector.
 * Selects encoder based on user input.
 */
Writer *select_writer(const string *path, const map<string, string> &comments, musconv_opts *opt){
  Writer *ret = NULL;
  // TODO remove the constructor from here.
  // TODO select based on file extension
  switch(opt->encoder){
    case WRITER_OPUS:
      try{
        ret = new Opus(path, comments, opt);
      }
      catch(exception &e){
        ret = NULL;
      }
      break;
    case WRITER_FLAC:
      try{
        ret = new Flac(path, comments, opt);
      }
      catch(exception &e){
        ret = NULL;
      }
      break;
    default:
      ret = NULL;
  }
  return ret;
}

