#include "selector.hpp"

#include <map>
#include <string>

#include "reader.hpp"
#include "writer.hpp"
#include "mpt.hpp"
#include "vgm.hpp"
#include "ope.hpp"
#include "flacenc.hpp"
#include "option.hpp"

using namespace std;

/* Decoder type selector.
 * Selects which Reader subclass to use to decode given file, or none if unsupported. 
 */
Reader *select_reader(const string *path, const string *ext, musconv_opts *opt){
  Reader *ret = NULL;
  if(Mpt::is_supported(*ext)){
    try{
      ret = new Mpt(path, opt);
    }
    catch(exception &e){
      ret = NULL;
    }
  }
  else if(Vgm::is_supported(*ext)){
    try{
      ret = new Vgm(path, opt);
    }
    catch(exception &e){
      ret = NULL;
    }
  }
  else{
    ret = NULL;
  }
  return ret;
}

void supported(void){
  Mpt::print_supported();
  Vgm::print_supported();
}

/* Encoder type selector.
 * Selects encoder based on user input.
 */
Writer *select_writer(const string *path, const map<string, string> &comments, musconv_opts *opt){
  Writer *ret = NULL;
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
