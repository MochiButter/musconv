# musconv
Music conversion utility, currently supports various trackers to opus.

A re-write in c++ for 
[modopus](https://github.com/MochiButter/modopus)
, to be more modular and allow easier expansion.

## Building
run `make`, libopenmpt, libopusenc, and libFLAC are required. 
The binary will be in the build directory called `musconv`

## Usage
```
Usage:
  build/musconv <option(s)> <input filename>
The output file will have the same name as the input file, with .opus file extension.

Options:
  -h, --help         Shows this.
  -V, --version      Prints the version.
  -v, --verbose      Prints more info.
  -q, --quiet        Runs without printing information.
  -o <string>        Output file template. String must be in quotations.
                     Supports the following:
                       %(fn) - The source file's filename without the extension.
                       %(ext) - The destination file's extension without the dot.
                       %(title) - The source file's title extracted from the reader.
                       %(artist) - The source file's artist extracted from the reader.
                       ex. "%(artist) - %(title) [%(fn)].%(ext)"  
  -y                 Automatically overwrites a file when asked.
  --supported        Prints a list of supported file formats.

Rendering options:
  --encoder s        Sets the encoder to s, options "opus" or "flac" are valid.
  --samplerate n     Set samplerate to n.
  --repeat-count n   Repeat the song n times after playing once.
  --channels n       Sets the channel to n, options 1, 2, or 4.
  --dry-run          Run the program, skipping writing to file.
  --time n           Specifies the time in seconds the song should play for. INCLUDES the fadeout time.
  --fadeout n        After the song finishes playing, play for another n seconds and fade out.

Comment options:
  --auto-comment     Automatically fills the tags of the output file from the source file.
```
