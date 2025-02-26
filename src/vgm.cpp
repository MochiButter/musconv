#include "vgm.hpp"

#include <stdio.h>
#include <stdbool.h>

#include <stdexcept>
#include <string>
#include <vector>

#include <player/playera.hpp>
#include <player/vgmplayer.hpp>
#include <player/s98player.hpp>
#include <player/droplayer.hpp>
#include <player/gymplayer.hpp>
#include <utils/DataLoader.h>
#include <utils/FileLoader.h>

#include "reader.h"
#include "option.h"

using namespace std;
static DATA_LOADER* file_req_cb(void *, PlayerBase *, const char *);

Vgm::Vgm(string path, musconv_opts *opt) : Reader(opt){
  data_loader = FileLoader_Init(path.c_str());
  if (!data_loader){
    fprintf(stderr, "Error in Vgm(): failed to open source file.\n");
    throw runtime_error("Vgm create error");
  }

  player.RegisterPlayerEngine(new VGMPlayer);
  player.RegisterPlayerEngine(new S98Player);
  player.RegisterPlayerEngine(new DROPlayer);
  player.RegisterPlayerEngine(new GYMPlayer);
  player.SetFileReqCallback(file_req_cb, NULL);

  // TODO add bit depth option
  // TODO limit channels to 2 for libvgm conversions
  uint8_t res;
  res = player.SetOutputSettings(
      Reader::samplerate, Reader::channels, 16, /* bit depth */ Reader::bufsize);
  if (res){
    fprintf(stderr, "Error in Vgm(): failed to create vgm player.\n");
    throw runtime_error("Vgm create error");
  }

  PlayerA::Config vgm_opts = player.GetConfiguration();
  // set loops to infinite when play-time is dictated by the converter
  size_t conf_loops = (Reader::fade_seconds || Reader::play_seconds) ? 0 : Reader::repeat_count + 1;
  vgm_opts.loopCount = conf_loops;

  player.SetConfiguration(vgm_opts);

  DataLoader_SetPreloadBytes(data_loader,255);
  res = DataLoader_Load(data_loader);
  if (res){
    fprintf(stderr, "Error in Vgm(): failed to load file header.\n");
    throw runtime_error("Vgm create error");
  }

  res = player.LoadFile(data_loader);
  if (res){
    fprintf(stderr, "Error in Vgm(): failed to load the rest of the file\n");
    throw runtime_error("Vgm create error");
  }

  player_emu = player.GetPlayer();

  if (player_emu->GetPlayerType() == FCC_VGM){
    VGMPlayer* vgmplay = (VGMPlayer*) player_emu;
    player.SetLoopCount(vgmplay->GetModifiedLoopCount(conf_loops));
  }

  player.Start();

  // get the frames needed to render at least the desired amount of loops.
  vgm_frames = (size_t) player_emu->Tick2Sample
    (player_emu->GetTotalPlayTicks(Reader::repeat_count + 1));
  finished_main_loop = false;
}

Vgm::~Vgm() {
  player.Stop();
  player.UnloadFile();
  player.UnregisterAllPlayers();
  DataLoader_Deinit(data_loader);
}

size_t Vgm::read_file(int16_t *buf, size_t read_count){
  if(read_count > Reader::bufsize){
    fprintf(stderr, "Error in Vgm(): bad read count given\n");
    return 0;
  }
  int frame_factor = 16 / 8;

  size_t frames_to_render = (finished_main_loop || read_count < vgm_frames)
    ? read_count : vgm_frames;
  size_t samples_to_render = frames_to_render * frame_factor * Reader::channels;
  vector<uint8_t> buf_8bit(samples_to_render);
  player.Render(samples_to_render, buf_8bit.data());

  for (size_t frame = 0; frame < frames_to_render; frame ++){
    for (size_t chan = 0; chan < (size_t) Reader::channels; chan ++){
      size_t buf_16bit_ind = (Reader::channels * frame) + chan;
      size_t buf_8bit_ind = buf_16bit_ind * frame_factor;
      // TODO make a function for different bit depths
      buf[buf_16bit_ind] = (buf_8bit[buf_8bit_ind + 1] << 8) | (buf_8bit[buf_8bit_ind]);
    }
  }

  if (finished_main_loop || Reader::play_seconds){
    return read_count;
  }
  vgm_frames -= frames_to_render;
  if (frames_to_render < read_count) {
    finished_main_loop = true;
    return frames_to_render;
  }
  return read_count;
}

ssize_t Vgm::get_comments(map<string,string> *comments) const{
  if (!player_emu){
    fprintf(stderr, "Error in Vgm::get_comments(): player_emuwas not initialized\n");
    return -1;
  }

  ssize_t count = 0;
  const char *const *tags;
  tags = player_emu->GetTags();
  while(*tags) {
    comments->insert_or_assign(tags[0], tags[1]);
    tags += 2;
    ++count;
  }
  return count;
}

bool Vgm::is_supported(string ext){
  bool supported = false;
  supported |= ext == "dro";
  supported |= ext == "gym";
  supported |= ext == "s98";
  supported |= ext == "vgz";
  supported |= ext == "vgm";
  return supported;
}

static DATA_LOADER* file_req_cb(void *user_param, PlayerBase *player, const char *path){
  (void) user_param;
  (void) player;

  DATA_LOADER* rom_loader = FileLoader_Init(path);
  uint8_t res = DataLoader_Load(rom_loader);
  if (res){
    DataLoader_Deinit(rom_loader);
    return NULL;
  }
  return rom_loader;
}
