# musconv
Music conversion utility, currently supports various trackers and video game music formats to opus and flac.

## Building
First, clone the libvgm submodule.
Then run cmake as normal.
```
git submodule update --init
cmake -B build
cmake --build ./build
```
libopenmpt, libopusenc, and FLAC are required. 
The binary will be in the build directory called `musconv`

(Tested on Debian 12 and Ubuntu Server 24.04.2 LTS)

## Usage
```
Usage:
  musconv <option(s)> <input filename>
The default output file name will have the same name as the input file, with the encoder's file extension.

Options:
  -h, --help         Shows this.
  -V, --version      Prints the version.
  -v, --verbose      Prints more info.
  -q, --quiet        Runs without printing information.
  -o <string>        Output file template. String must be in quotations.
                     Supports the following:
                       %(fn) - The source file's filename without the extension.
                       %(ext) - The destination file's extension without the dot.
                       ex. "convert/%(fn).%(ext)"
  -y                 Automatically overwrites a file when asked.
  --supported        Prints a list of supported file formats.

Rendering options:
  --encoder s        Sets the encoder to s, options "opus" or "flac" are valid.
  --samplerate n     Set samplerate to n.
  --repeat-count n   Repeat the song n times after playing once. Will not affect songs that do not have loops.
  --channels n       Sets the channel to n, options 1, 2, or 4.
  --dry-run          Run the program, skipping writing to file.
  --time n           Specifies the time in seconds the song should play for. INCLUDES the fadeout time.
  --fadeout n        After the song finishes playing, play for another n seconds and fade out.

Comment options:
  --auto-comment     Automatically fills the tags of the output file from the source file.
```

## supported file formats
```
Reader "libopenmpt" supports:
    mptm mod s3m xm it 669 amf ams c67 dbm digi dmf dsm dsym dtm far fmt
    imf ice j2b m15 mdl med mms mt2 mtm mus nst okt plm psm pt36 ptm sfx
    sfx2 st26 stk stm stx stp symmod ult wow gdm mo3 oxm umx xpk ppm mmcmp
Reader "libvgm" supports:
    dro gym s98 vgz vgm
```
