#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include <filesystem>
#include <map>
#include <vector>
#include <iostream>

#include "reader.h"
#include "writer.h"
#include "selector.h"
#include "musconv.h"

using namespace std;

#define VERSION 0.2

/* Prints information on how to use options.
 * param name the name of the executable
 */
void usage(const char* name){
  printf("Usage:\n");
  printf("  %s <option(s)> <input filename>\n",name);
  printf("The output file will have the same name as the input file, with .opus file extension.\n");
  printf("\nOptions:\n");
  printf("  -h, --help         Shows this.\n");
  printf("  -V, --version      Prtints the version.\n");
  printf("  -v, --verbose      Prtints more info.\n");
  printf("  -q, --quiet        Runs without printing information.\n");
  printf("  -o <string>        Output file template. String must be in quatations.\n");
  printf("                     Supports the following:\n");
  printf("                       %%(fn) - The source file's filename without the extension.\n");
  printf("                       %%(ext) - The destination file's extension without the dot.\n");
  printf("                       %%(title) - The source file's title extracted from the reader.\n");
  printf("                       %%(artist) - The source file's artist extracted from the reader.\n");
  printf("                       ex. \"%%(artist) - %%(title) [%%(fn)].%%(ext)\"");
  printf("  --supported        Shows the list of supported file formats.\n");
  printf("\nRendering options:\n");
  printf("  --samplerate n     Set samplerate to n.\n");
  printf("  --repeat-count n   Repeat the song n times after playing once.\n");
  printf("  --dry-run          Run the program, skipping writing to file.\n");
}

int main(int argc, char **argv){
  if(argc < 2){
    printf("Usage: %s <option(s)> <input filename>\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  string out_template = "convert/%(artist) - %(title) [%(fn)].%(ext)";
  int32_t samplerate = 48000;
  // 20ms buffer
  int32_t buffersize = (int32_t)samplerate * 0.02;
  int32_t fade_seconds = 0;

  // Process options.
  // TODO: support mono, surround
  int c = 0;
  while(1){
    int option_index = 0;
    const char* opname;
    static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},
      {"version", no_argument, 0, 'V'},
      {"verbose", no_argument, 0, 'v'},
      {"quiet", no_argument, 0, 'q'},
      {"output", required_argument, 0, 'o'},
      {"supported", no_argument, 0, 0},
      {"samplerate", required_argument, 0, 0},
      {"framesize", required_argument, 0, 0},
      {"auto-comment", no_argument, 0, 0},
      {"comment", required_argument, 0, 0},
      {"repeat-count", required_argument, 0, 0},
      {"interpolation", required_argument, 0, 0},
      {"gain", required_argument, 0, 0},
      {"print-subsongs", no_argument, 0, 0},
      {"print-metadata", no_argument, 0, 0},
      {"dry-run", no_argument, 0, 0},
      {0, 0, 0, 0}
    };
    c = getopt_long(argc, argv, "hvVqo:", long_options, &option_index);
    if( c == -1)
      break;

    (void)opname;
    switch (c){
      case 0:
        opname = long_options[option_index].name;
        /*
        if(strcmp(opname,"samplerate") == 0){ // set sample rate 
          opt.samplerate = atoi(optarg);
        }
        else if(strcmp(opname,"framesize") == 0){ // set frame size
          if(strcmp(optarg,"2.5") == 0) 
            opt.framesize = OPUS_FRAMESIZE_2_5_MS;
          else if(strcmp(optarg,"5") == 0) 
            opt.framesize = OPUS_FRAMESIZE_5_MS;
          else if(strcmp(optarg,"10") == 0) 
            opt.framesize = OPUS_FRAMESIZE_10_MS;
          else if(strcmp(optarg,"20") == 0)
            opt.framesize = OPUS_FRAMESIZE_20_MS;
          else if(strcmp(optarg,"40") == 0)
            opt.framesize = OPUS_FRAMESIZE_40_MS;
          else if(strcmp(optarg,"60") == 0)
            opt.framesize = OPUS_FRAMESIZE_60_MS;
          else{
            printf("--framesize must be one of the following: [2.5, 5, 10, 20, 40, 60].\n");
            return 1;
          }
        }
        else if(strcmp(opname, "auto-comment") == 0){ // add comments from input file
          opt.auto_comment = true;
        }
        else if(strcmp(opname, "artist") == 0){ // set artist tag
          opt.artist = optarg;
        }
        else if(strcmp(opname, "title") == 0){ // set title tag
          opt.title = optarg;
        }
        else if(strcmp(opname, "date") == 0){ // set date tag
          opt.date = optarg;
        }
        else if(strcmp(opname, "repeat-count") == 0){ // plays n + 1 times
          int32_t rc = atoi(optarg);
          if(rc < 0){
            printf("--repeat-count must be 0 (no loops) or n > 0 (play once and loop n times)\n");
            return 1;
          }
          opt.repeat_count = rc;
        }
        else if(strcmp(opname, "gain") == 0){ // set gain in mB
          opt.gain = atoi(optarg);
        }
        else if(strcmp(opname, "interpolation") == 0){ // set interpolation filter preset
          int32_t ifl = atoi(optarg);
          if(ifl < 0){
            printf("Interpolation value must be grater than 0, see --help for more\n");
            return 1;
          }
          opt.interpolation = ifl;
        }
        else if(strcmp(opname, "print-subsongs") == 0){ // print list of subsongs and numbers
          opt.print_sub = true;
        }
        else if(strcmp(opname, "print-metadata") == 0){ // print song metadata
          opt.print_meta = true;
        }
        else if(strcmp(opname, "dry-run") == 0){ // skips encoding to file
          opt.dry_run = true;
        }
        else if(strcmp(opname, "supported") == 0){ // print list of supported files
          supported();
          return 0;
        }
        */
        break;
      case 'h': // print usage
        usage(argv[0]);
        exit(EXIT_SUCCESS);
        break;
      case 'V'://version
        printf("%s %f\n",argv[0], VERSION);
        exit(EXIT_SUCCESS);
        break;
      case 'v'://verbose
        break;
      case 'q': // don't print
        //opt.quiet = true;
        break;
      case 'o': // set output file location
        out_template = optarg;
        break;
      case '?': // option not recognized
        usage(argv[0]);
        exit(EXIT_FAILURE);
        break;
    }
  }

  /*
   * Main loop for conversion
   * For each file given, loads the correct reader and writer to convert
   * files. Loads options and creates objects to follow the user's input.
   */
  while(optind < argc){
    char *path = argv[optind++];
    filesystem::path p(path);
    string fdir = p.parent_path().string();
    string fstem = p.stem().string();
    string fext = p.extension().string().erase(0,1);

    Reader *r = nullptr;
    Writer *w = nullptr;
    map<string, string> comments;
    filesystem::path outpath;
    string out;

    // TODO: error catch and throwing 
    r = select_reader(path, fext);
    if(r == nullptr){
      goto clean;
    }

    // Get comments from reader
    r->get_comments(&comments);

    out = get_output_path(out_template, fstem, WRITER_OPUS, comments);
    outpath = filesystem::path(out);
    // check if path is writeable, skip if not
    if((filesystem::perms::owner_write & filesystem::status(outpath).permissions()) 
        != filesystem::perms::owner_write){
      cerr << "Not able to write to " << out << endl;
      goto clean;
    }
    // creates any directory in path that does not exixt
    filesystem::create_directories(outpath.parent_path());
    
    w = select_writer(out, WRITER_OPUS, comments);
    if(w == nullptr){
      goto clean;
    }

    (void)fade_seconds;
    if(r != nullptr && w != nullptr){
      cout << "Writing to " << out << endl;
      vector<int16_t> buffer(buffersize * 2);
      while(1){
        size_t c = 0;
        c = r->read_file(buffer.data(), buffersize);
        w->write_file(buffer.data(),c);
        if((int)c < buffersize){
          cout << "Success" << endl;
          break;
        }
      }
      if(fade_seconds){
        int32_t fade_samples = fade_seconds * samplerate;
        int32_t samples_left = fade_samples;
        size_t wrsize = buffersize;
        while(samples_left >= buffersize){
          // TODO does read_file need the buffersize input?
          r->read_file(buffer.data(), buffersize);
          samples_left -= buffersize;
          if(samples_left < buffersize){
            wrsize = samples_left;
          }
          for(size_t i = 0; i < wrsize; i++){
            float volume = (samples_left + buffersize - i) / (float)fade_samples;
            size_t bufpos = 2 * i;
            buffer[bufpos] = (int16_t)(buffer[bufpos] * volume);
            buffer[bufpos + 1] = (int16_t)(buffer[bufpos] * volume);
          }
          w->write_file(buffer.data(), wrsize);
        }
        cout << "fade success" << endl;
      }
    }
clean:
    if(r) {delete(r);}
    if(w) {delete(w);}
  }
  exit(EXIT_SUCCESS);
}
