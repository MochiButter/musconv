#include "selector.h"

#include <map>
#include <string>

#include "reader.h"
#include "writer.h"
#include "mpt.h"
#include "ope.h"
#include "musconv.h"
#include "option.h"

using namespace std;

/* Decoder type selector.
 * Selects which Reader subclass to use to decode given file, or none if unsupported. 
 */
Reader *select_reader(string path, string ext, musconv_opts *opt){
  Reader *ret = nullptr;
  if(Mpt::is_supported(ext)){
    try{
      ret = new Mpt(path, opt);
    }
    catch(exception &e){
      ret = nullptr;
    }
  }
  else{
    ret = nullptr;
  }
  return ret;
}

/* Encoder type selector.
 * Selects encoder based on user input.
 */
Writer *select_writer(string path, WRITESEL encoder, const map<string, string> &comments){
  Writer *ret = nullptr;
  switch(encoder){
    case WRITER_OPUS:
      try{
        ret = new Opus(path, comments);
      }
      catch(exception &e){
        ret = nullptr;
      }
      break;
    default:
      ret = nullptr;
  }
  return ret;
}
