ckormanyos/xxd
==================

[![Build Status](https://github.com/ckormanyos/xxd/actions/workflows/xxd.yml/badge.svg)](https://github.com/ckormanyos/xxd/actions)

This is the well-known hex-dump-type utility `xxd` commonly distributed
as a part of the [vim-project](https://www.vim.org).
The version of `xxd` in this repository has been adapted
for MSVC/GCC/clang standalone build.

## Build

This version of the source code has been adapted for build on both `Win*`
as well as `*nix`.

### Build on `Win*` with MSVC

On `Win*` with MSVC simply:

  - Open the `xxd.sln` workspace.
  - Rebuild `Release` (i.e., for the `x64` project configuration).
  - After building, find the executable `xxd.exe` in the expected output directory.

### Build on `*nix` on the command line

On `*nix` it is straightforward to build on the command line.

  - Switch to the `xxd`directory.
  - Build (for instance with GCC) using an easy command, as shown below.
  - This compiles `src/xxd.c` to `xxd.exe`.

In other words,

```sh
cd xxd
g++ -x c -std=c99 -O3 -Wall -Wextra -pedantic -Wconversion -Wsign-conversion src/xxd.c -o xxd.exe
```

## Adaptions

The following adaptions have been undertaken.

  - Create an MSVC `*.sln` workspace and associated project configuration.
  - Disregard (i.e., delete and do not use) the configuration header `config.h.in`.
  - Replace the compiler switch `WIN32` with MSVC's standard `_WIN32`.
  - Run the `xxd.c`/`config.h` files through the [Artistic Style](http://astyle.sourceforge.net/astyle.html) automatic code formatter, using a version of _AStyle_ from somewhere around 2015.
  - Handle Level-3 warnings found in MSVC.
  - Handle GCC warnings from `-Wall`, `-Wextra`, `-Wpedantic`, `-Wconversion` and `-Wsign-conversion`.
  - Add CI consisting of MSVC/GCC/clang builds and a handful of straightforward test cases.
  - Upload build artifacts in CI for `xxd-x86_64-linux-gnu` and `xxd-win64-msvc`, see also discussion in [issue 11](https://github.com/ckormanyos/xxd/issues/11).

## Usage

The program manual at the [xxd(1) - Linux man page](https://linux.die.net/man/1/xxd)
states:

`xxd` - make a hexdump or do the reverse.

It can do nearly everything hexdump can and moreover perform
the reversal translation of hex-like text back to binary representation.
Like [uuencode](https://linux.die.net/man/1/uuencode)
and [uudecode](https://linux.die.net/man/1/uudecode)
it allows the transmission of binary data in a `mail-safe`
ASCII representation, but has the advantage of decoding to standard output.
Moreover, it can be used to perform binary file patching.

### Examples

Print everything but the first three lines (hex 0x30 bytes) of file.
```
% xxd -s 0x30 file
```

Print 3 lines (hex 0x30 bytes) from the end of file.
```
% xxd -s -0x30 file
```

Print 120 bytes as continuous hexdump with 20 octets per line.
```
% xxd -l 120 -ps -c 20 xxd.1
2e54482058584420312022417567757374203139
39362220224d616e75616c207061676520666f72
20787864220a2e5c220a2e5c222032317374204d
617920313939360a2e5c22204d616e2070616765
20617574686f723a0a2e5c2220202020546f6e79
204e7567656e74203c746f6e79407363746e7567
```

Hexdump the first 120 bytes of this man page with 12 octets per line.
```
% xxd -l 120 -c 12 xxd.1
0000000: 2e54 4820 5858 4420 3120 2241  .TH XXD 1 "A
000000c: 7567 7573 7420 3139 3936 2220  ugust 1996"
0000018: 224d 616e 7561 6c20 7061 6765  "Manual page
0000024: 2066 6f72 2078 7864 220a 2e5c   for xxd"..\
0000030: 220a 2e5c 2220 3231 7374 204d  "..\" 21st M
000003c: 6179 2031 3939 360a 2e5c 2220  ay 1996..\"
0000048: 4d61 6e20 7061 6765 2061 7574  Man page aut
0000054: 686f 723a 0a2e 5c22 2020 2020  hor:..\"
0000060: 546f 6e79 204e 7567 656e 7420  Tony Nugent
000006c: 3c74 6f6e 7940 7363 746e 7567  <tony@sctnug
```

Display just the date from the file xxd.1
```
% xxd -s 0x36 -l 13 -c 13 xxd.1
0000036: 3231 7374 204d 6179 2031 3939 36  21st May 1996
```

Copy input_file to output_file and prepend 100 bytes of value 0x00.
```
% xxd input_file | xxd -r -s 100 > output_file
```

Patch the date in the file xxd.1
```
% echo "0000037: 3574 68" | xxd -r - xxd.1
% xxd -s 0x36 -l 13 -c 13 xxd.1
0000036: 3235 7468 204d 6179 2031 3939 36  25th May 1996
```

Create a 65537 byte file with all bytes 0x00, except for the  last  one which is 'A' (hex 0x41).
```
% echo "010000: 41" | xxd -r > file
```

Hexdump this file with autoskip.
```
% xxd -a -c 12 file
0000000: 0000 0000 0000 0000 0000 0000  ............
*
000fffc: 0000 0000 40                   ....A
```

Create  a  1  byte  file containing a single 'A' character.  The number
after '-r -s' adds to the linenumbers found in the file; in effect, the
leading bytes are suppressed.
```
% echo "010000: 41" | xxd -r -s -0x10000 > file
```

Read single characters from a serial line
```
% xxd -c1 < /dev/term/b &
% stty < /dev/term/b -echo -opost -isig -icanon min 1
% echo -n foo > /dev/term/b
```

## Additional Details

When searching for the `xxd` utility one finds it distributed as a part of vim-project and its packages.
This project extracts the code, creates an MSVC solution workspace and provides
the ability to easily build `xxd` on `Win*` or `*nix`.

The original code was taken from: [vim github repo](https://github.com/vim/vim) on 28-March-2022.
The simplicity of the code port has been previously established (among other places)
in [fancer/xxd](https://github.com/fancer/xxd).

The `xxd` code copyrights are left untouched
except for adding an additional note regarding the MSVC build.
The license as well as the license declaration are left untouched.
This original package is licensed by GPL-2.0. This version retains compatibility with the
licensing of the original utility.

Continuous integration runs with GCC, clang and MSVC.
The CI runs exercise both building `xxd` as well as running
several straightforward `xxd` test cases.
