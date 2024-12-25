#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "musconv.h"
#include "option.h"

using namespace std;

#define VERSION_MAJOR 0
#define VERSION_MINOR 4

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
  printf("  --supported        Prints a list of supported file formats.\n");
  printf("\nRendering options:\n");
  printf("  --encoder s        Sets the encoder to s, options \"opus\" or \"flac\" are valid.\n");
  printf("  --samplerate n     Set samplerate to n.\n");
  printf("  --repeat-count n   Repeat the song n times after playing once.\n");
  printf("  --channels n       Sets the channel to n, options 1, 2, or 4.\n");
  printf("  --dry-run          Run the program, skipping writing to file.\n");
  printf("  --time n           Specifies the time in seconds the song should play for. INCLUDES the fadeout time.\n");
  printf("  --fadeout n        After the song finishes playing, play for another n seconds and fade out.\n");
  printf("\nComment options:\n");
  printf("  --auto-comment     Automatically fills the tags of the output file from the source file.\n");
}

int main(int argc, char **argv){
  if(argc < 2){
    printf("Usage: %s <option(s)> <input filename>\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  musconv_opts opt;
  init_opts(&opt);

  // Process options.
  // TODO: support mono, surround
  int c = 0;
  int option_index = 0;
  const char* opname;
  static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {"quiet", no_argument, 0, 'q'},
    {"output", required_argument, 0, 'o'},
    {"encoder", required_argument, 0, 0},
    {"supported", no_argument, 0, 0},
    {"samplerate", required_argument, 0, 0},
    {"channels", required_argument, 0, 0},
    {"framesize", required_argument, 0, 0},
    {"auto-comment", no_argument, 0, 0},
    {"comment", required_argument, 0, 0},
    {"repeat-count", required_argument, 0, 0},
    {"interpolation", required_argument, 0, 0},
    {"gain", required_argument, 0, 0},
    {"fadeout", required_argument, 0, 0},
    {"time", required_argument, 0, 0},
    {"print-subsongs", no_argument, 0, 0},
    {"print-metadata", no_argument, 0, 0},
    {"dry-run", no_argument, 0, 0},
    {0, 0, 0, 0}
  };

  while(1){
    c = getopt_long(argc, argv, "hvVqo:", long_options, &option_index);
    if( c == -1)
      break;

    switch (c){
      case 0:
        opname = long_options[option_index].name;
        if(strcmp(opname,"encoder") == 0){
          if(strcmp(optarg,"opus") == 0){
            opt.encoder = WRITER_OPUS;
          }
          else if(strcmp(optarg,"flac") == 0){
            opt.encoder = WRITER_FLAC;
          }
          else{
            usage(argv[0]);
            exit(EXIT_FAILURE);
          }
        }
        else if(strcmp(opname,"samplerate") == 0){ // set sample rate 
          opt.samplerate = atoi(optarg);
          opt.bufsize = opt.samplerate * 0.02;
        }
        else if(strcmp(opname,"channels") == 0){ // set channels
          int32_t ch = atoi(optarg);
          switch(ch){
            case 1:
            case 2:
            case 4:
              opt.channels = ch;
              break;
            default:
              printf("Channel count must be 1, 2, or 4.\n");
              exit(EXIT_FAILURE);
          }
        }
        else if(strcmp(opname, "auto-comment") == 0){ // add comments from input file
          opt.auto_comment = true;
        }
        /*
        else if(strcmp(opname, "artist") == 0){ // set artist tag
          opt.artist = optarg;
        }
        else if(strcmp(opname, "title") == 0){ // set title tag
          opt.title = optarg;
        }
        else if(strcmp(opname, "date") == 0){ // set date tag
          opt.date = optarg;
        }
        */
        else if(strcmp(opname, "repeat-count") == 0){ // plays n + 1 times
          int32_t rc = atoi(optarg);
          if(rc < 0){
            printf("--repeat-count must be 0 (no loops) or n > 0 (play once and loop n times)\n");
            return 1;
          }
          opt.repeat_count = rc;
        }
        /*
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
        */
        else if(strcmp(opname, "fadeout") == 0){ // fades out for n seconds
          int32_t fade = atoi(optarg);
          if(fade < 0){
            printf("--fadeout must be 0 or above\n");
            return 1;
          }
          opt.fade_seconds = fade;
        }
        else if(strcmp(opname, "time") == 0){ // plays for n seconds, fadeout inclusive
          int32_t time = atoi(optarg);
          if(time < 1){
            printf("--time must be above 0 seconds\n");
            return 1;
          }
          opt.play_seconds = time;
        }
        else if(strcmp(opname, "print-subsongs") == 0){ // print list of subsongs and numbers
          opt.print_sub = true;
        }
        else if(strcmp(opname, "print-metadata") == 0){ // print song metadata
          opt.print_meta = true;
        }
        else if(strcmp(opname, "dry-run") == 0){ // skips encoding to file
          opt.dry_run = true;
          printf("======Doing a dry-run: no files will be written======\n");
        }
        else if(strcmp(opname, "supported") == 0){ // print list of supported files
          supported();
          exit(EXIT_SUCCESS);
        }
        break;
      case 'h': // print usage
        usage(argv[0]);
        exit(EXIT_SUCCESS);
        break;
      case 'V'://version
        printf("musconv version %d.%d\n", VERSION_MAJOR, VERSION_MINOR);
        exit(EXIT_SUCCESS);
        break;
      case 'q': // don't print
        opt.quiet = true;
        break;
      case 'o': // set output file location
        opt.out_template = optarg;
        break;
      case '?': // option not recognized
        usage(argv[0]);
        exit(EXIT_FAILURE);
        break;
    }
  }

  if(optind >= argc){
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  if((opt.play_seconds) && opt.fade_seconds >= opt.play_seconds){
    printf("Fadeout time must be at least 1 seconds less than playback time\n");
    exit(EXIT_FAILURE);
  }

  if(!opt.quiet){
    print_opts(&opt);
  }

  while(optind < argc){
    char *path = argv[optind++];
    if(!opt.dry_run){
      bool success = music_convert(path, &opt);
      if(!success){
        printf("Failed converting %s\n", path);
      }
      else {
        printf("Success\n");
      }
    }
  }
  exit(EXIT_SUCCESS);
}
