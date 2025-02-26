#ifndef VGM_H
#define VGM_H

#include <stdbool.h>

#include <string>
#include <map>

#include <player/playera.hpp>
#include <utils/DataLoader.h>

#include "reader.h"
#include "option.h"

using namespace std;

class Vgm: public Reader{
  private:
    DATA_LOADER *data_loader;
    size_t vgm_frames;
    PlayerA player;
    PlayerBase *player_emu;
    bool finished_main_loop;
  public:
    Vgm(string, musconv_opts *);
    virtual ~Vgm();

    virtual size_t read_file(int16_t *, size_t) override;
    
    virtual ssize_t get_comments(map<string,string> *) const override;

    static bool is_supported(string);
};
#endif
