#include "gme.hpp"

#include "gme/gme.h"

#include <stdio.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "reader.hpp"
#include "option.hpp"
#include "fileutil.hpp"

using namespace std;

static string get_m3upath(const string *filepath) {
  string ext = get_fext(filepath);
  string ret = *filepath;
  find_and_replace(&ret, ext, "m3u");
  return ret;
}

Gme::Gme(const string *path, musconv_opts *opt) : Reader(opt) {
  //gme_type_t ftype = gme_identify_extension(path->c_str());
  gme_err_t err;
  err = gme_open_file(path->c_str(), &emu, Reader::samplerate);

  if (err) {
    fprintf(stderr, "Error in Gme(): failed to open file. %s\n", err);
    throw runtime_error("Gme create error");
  }

  if (opt->use_m3u) {
    string m3upath = get_m3upath(path);
    gme_load_m3u(emu, m3upath.c_str());
  }

  // TODO make this an option ... also useful for mpt
  int tracknum = 0;

  if (opt->play_seconds == 0) {
    gme_info_t* info;
		err = gme_track_info(emu, &info, tracknum);
    if (err) {
      fprintf(stderr, "Error in Gme(): failed load info. %s\n", err);
      throw runtime_error("Gme create error");
    }

    if (opt->use_m3u) {
      Reader::play_seconds = info->length / 1000 + opt->fade_seconds;
    } else {
      Reader::play_seconds = 180;
    }
    gme_free_info(info);
  }

  err = gme_start_track(emu, tracknum);
  if (err){
    fprintf(stderr, "Error in Gme(): failed to start track.\n");
    throw runtime_error("Gme create error");
  }
  printf("playing for %d seconds\n", Reader::play_seconds);
  opt->play_seconds = Reader::play_seconds;
}

Gme::~Gme() {
  gme_delete(emu);
}

size_t Gme::read_file(int16_t *buf, size_t read_count) {
  if(read_count > Reader::bufsize){
    fprintf(stderr, "Error in Gme::read_file(): bad read count given\n");
    return 0;
  }

  // TODO make this work for mono
  gme_err_t err;
  err = gme_play(emu, Reader::bufsize * Reader::channels, buf);
  if (err) {
    fprintf(stderr, "Error in Gme::read_file(): could not read %lu samples\n", Reader::bufsize);
    return 0;
  }
  return Reader::bufsize;
}

ssize_t Gme::get_comments(map<string, string> *comments) const {
  gme_info_t* info;
  gme_err_t err;
  err = gme_track_info(emu, &info, 0);
  if (err) {
    printf("Failed getting comments: %s\n", err);
    return 0;
  }

  ssize_t count = 0;
  if (info->song) {
    comments->insert_or_assign("title", info->song);
    count ++;
  }
  if (info->author) {
    comments->insert_or_assign("artist", info->author);
    count ++;
  }
  if (info->game) {
    comments->insert_or_assign("album", info->game);
    count ++;
  }
  if (info->copyright) {
    comments->insert_or_assign("copyright", info->copyright);
    count ++;
  }
  if (info->system) {
    comments->insert_or_assign("system", info->system);
    count ++;
  }
  if (info->comment) {
    comments->insert_or_assign("comment", info->comment);
    count ++;
  }
  if (info->dumper) {
    comments->insert_or_assign("dumper", info->dumper);
    count ++;
  }

  gme_free_info( info );
  return 0;
}

bool Gme::is_supported(const string ext) {
  bool supported = false;
  supported |= ext == "ay";
  supported |= ext == "gbs";
  supported |= ext == "gym";
  supported |= ext == "hes";
  supported |= ext == "kss";
  supported |= ext == "nsf";
  supported |= ext == "nsfe";
  supported |= ext == "sap";
  supported |= ext == "spc";
  supported |= ext == "vgm";
  supported |= ext == "vgz";
  return supported;
}

void Gme::print_supported(void) {
  printf("Reader \"libgme\" supportd:\n");
  printf("\tay gbs hes kss nsf nsfe sap spc vgm vgz");
}
