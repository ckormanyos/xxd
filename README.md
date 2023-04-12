ckormanyos/xxd
==================

<p align="center">
    <a href="https://github.com/ckormanyos/xxd/actions">
        <img src="https://github.com/ckormanyos/xxd/actions/workflows/xxd.yml/badge.svg" alt="Build Status"></a>
    <a href="https://github.com/ckormanyos/xxd/issues?q=is%3Aissue+is%3Aopen+sort%3Aupdated-desc">
        <img src="https://custom-icon-badges.herokuapp.com/github/issues-raw/ckormanyos/xxd?logo=github" alt="Issues" /></a>
    <a href="https://sonarcloud.io/summary/new_code?id=ckormanyos_xxd">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=ckormanyos_xxd&metric=alert_status" alt="Quality Gate Status"></a>
    <a href="https://github.com/ckormanyos/xxd/blob/master/LICENSE">
        <img src="https://img.shields.io/badge/license-GPL%202.0-blue.svg" alt="GNU GENERAL PUBLIC LICENSE 2.0"></a>
    <a href="https://img.shields.io/github/commit-activity/y/ckormanyos/xxd">
        <img src="https://img.shields.io/github/commit-activity/y/ckormanyos/xxd" alt="GitHub commit activity" /></a>
    <a href="https://github.com/ckormanyos/xxd">
        <img src="https://img.shields.io/github/languages/code-size/ckormanyos/xxd" alt="GitHub code size in bytes" /></a>
    <a href="https://godbolt.org/z/n9fxM6Yd9" alt="godbolt">
        <img src="https://img.shields.io/badge/try%20it%20on-godbolt-green" /></a>
</p>

This is the well-known hex-dump-type utility `xxd` commonly distributed
as a part of the [vim-project](https://www.vim.org).
The version of `xxd` in this repository has been adapted
for MSVC/GCC/clang standalone build.

Building `ckormanyos/xxd` is straightforward.

In addition, build artifacts (i.e., the executable xxd program) are created
in CI for Windows, LINUX and MacOS . These can be readily found
in each CI run area and downloaded for convenient use.
When using the CI build artifacts, there is no need to build `ckormanyos/xxd`

## Build

Th source code in `ckormanyos/xxd` has been adapted
for standalone build on most common operating systems.

The source code of `ckormanyos/xxd` is written in the C language.
It is compatible with language standards C99, 11, 2x and beyond.

### Build on Windows with MSVC

On Windows with MSVC simply build `ckormanyos/xxd` with the following.

  - Open the `xxd.sln` workspace.
  - Rebuild `Release` (i.e., for the `x64` project configuration).
  - After building, find the executable `xxd.exe` in the expected output directory.

### Build on the LINUX command line

On LINUX it is straightforward to build `ckormanyos/xxd` on the command line.

  - Switch to the `xxd`directory.
  - Build (for instance with GCC) using an easy command, as shown below.
  - This compiles `src/xxd.c` to `xxd`.

In other words,

```sh
cd xxd
g++ -x c -std=c11 -O2 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion src/xxd.c -o xxd
```

### Build on the MacOS command line

When on MacOS it is also easy to build `ckormanyos/xxd` on the command line.

  - Switch to the `xxd`directory.
  - Build (for instance with clang) using an easy command, as shown below.
  - This compiles `src/xxd.c` to `xxd`.

In other words,

```sh
cd xxd
clang++ -x c -std=c11 -O2 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion src/xxd.c -o xxd
```

### Platform-independent build with CMake (and ninja)

Building `ckormanyos/xxd` on the command line is also supported
with platform-independent CMake (and ninja).
This has been motivated by [issue 16](https://github.com/ckormanyos/xxd/issues/16).

```sh
cd xxd
mkdir build
cd build
cmake .. -G Ninja && ninja
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
  - Resolve code-technical issues (and/or disable some) found via quality checks performed with CodeSonar, as described in [issue 15](https://github.com/ckormanyos/xxd/issues/15) and [issue 23](https://github.com/ckormanyos/xxd/issues/23).

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

Other code parts (such as continuous integration scripts) are licensed under BSL 1.0.

Continuous integration (CI) runs with GCC, clang and MSVC with both
tool-specific builds as well as platform-independent CMake builds.
CI exercises both building `xxd` as well as running
several straightforward `xxd` test cases.
