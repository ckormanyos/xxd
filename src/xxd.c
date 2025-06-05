/* xxd: my hexdump facility. jw
 *
 *  2.10.90 changed to word output
 *  3.03.93 new indent style, dumb bug inserted and fixed.
 *      -c option, mls
 * 26.04.94 better option parser, -ps, -l, -s added.
 *  1.07.94 -r badly needs - as input file.  Per default autoskip over
 *         consecutive lines of zeroes, as unix od does.
 *      -a shows them too.
 *      -i dump as c-style #include "file.h"
 *  1.11.95 if "xxd -i" knows the filename, an 'unsigned char filename_bits[]'
 *      array is written in correct c-syntax.
 *      -s improved, now defaults to absolute seek, relative requires a '+'.
 *      -r improved, now -r -s -0x... is supported.
 *         change/suppress leading '\0' bytes.
 *      -l n improved: stops exactly after n bytes.
 *      -r improved, better handling of partial lines with trailing garbage.
 *      -r improved, now -r -p works again!
 *      -r improved, less flushing, much faster now! (that was silly)
 *  3.04.96 Per repeated request of a single person: autoskip defaults to off.
 * 15.05.96 -v added. They want to know the version.
 *      -a fixed, to show last line inf file ends in all zeros.
 *      -u added: Print upper case hex-letters, as preferred by unix bc.
 *      -h added to usage message. Usage message extended.
 *      Now using outfile if specified even in normal mode, aehem.
 *      No longer mixing of ints and longs. May help doze people.
 *      Added binify ioctl for same reason. (Enough Doze stress for 1996!)
 * 16.05.96 -p improved, removed occasional superfluous linefeed.
 * 20.05.96 -l 0 fixed. tried to read anyway.
 * 21.05.96 -i fixed. now honours -u, and prepends __ to numeric filenames.
 *      compile -DWIN32 for NT or W95. George V. Reilly, * -v improved :-)
 *      support --gnuish-longhorn-options
 * 25.05.96 MAC support added: CodeWarrior already uses ``outline'' in Types.h
 *      which is included by MacHeaders (Axel Kielhorn). Renamed to
 *      xxdline().
 *  7.06.96 -i printed 'int' instead of 'char'. *blush*
 *      added Bram's OS2 ifdefs...
 * 18.07.96 gcc -Wall @ SunOS4 is now silent.
 *      Added osver for MSDOS/DJGPP/WIN32.
 * 29.08.96 Added size_t to strncmp() for Amiga.
 * 24.03.97 Windows NT support (Phil Hanna). Clean exit for Amiga WB (Bram)
 * 02.04.97 Added -E option, to have EBCDIC translation instead of ASCII
 *      (azc10@yahoo.com)
 * 22.05.97 added -g (group octets) option (jcook@namerica.kla.com).
 * 23.09.98 nasty -p -r misfeature fixed: slightly wrong output, when -c was
 *      missing or wrong.
 * 26.09.98 Fixed: 'xxd -i infile outfile' did not truncate outfile.
 * 27.10.98 Fixed: -g option parser required blank.
 *      option -b added: 01000101 binary output in normal format.
 * 16.05.00 Added VAXC changes by Stephen P. Wall
 * 16.05.00 Improved MMS file and merge for VMS by Zoltan Arpadffy
 * 2011 March  Better error handling by Florian Zumbiehl.
 * 2011 April  Formatting by Bram Moolenaar
 * 08.06.2013  Little-endian hexdump (-e) and offset (-o) by Vadim Vygonets.
 * 11.01.2019  Add full 64/32 bit range to -o and output by Christer Jensen.
 * 04.02.2020  Add -d for decimal offsets by Aapo Rantalainen
 * 14.01.2022  Disable extra newlines with -c0 -p by Erik Auerswald.
 * 28.03.2022  Adapted for MSVC/GCC/clang standalone build by Christopher Kormanyos.
 *             Also run through Artistic Style with (among others) allman style.
 * 15.03.2023  Handle CodeSonar issues by Christopher Kormanyos.
 * 06.08.2024  Add comments for running unit tests by Christopher Kormanyos.
 *               cd /mnt/c/Users/ckorm/Documents/Ks/PC_Software/xxd/.gcov/make
 *               make prepare -f make_gcov_01_generic.gmk MY_ALL_COV=0 MY_CC=g++
 *               make gcov -f make_gcov_01_generic.gmk MY_ALL_COV=0 MY_CC=g++
 * 12.08.2024  Remove unreachable code and refactor "continue" statements.
 * 18.08.2024  Add more tests and improve coverage.
 * 05.06.2025  Handle a LINTer issue.
 *
 * (c) 1990-1998 by Juergen Weigert (jnweiger@gmail.com)
 *
 * I hereby grant permission to distribute and use xxd
 * under X11-MIT or GPL-2.0 (at the user's choice).
 *
 * Contributions by Bram Moolenaar et al.
 */

/* Visual Studio 2005 has 'deprecated' many of the standard CRT functions */
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
# define _CRT_SECURE_NO_DEPRECATE
# define _CRT_NONSTDC_NO_DEPRECATE
#endif
#if !defined(CYGWIN) && defined(__CYGWIN__)
# define CYGWIN
#endif

#if (defined(__linux__) && !defined(__ANDROID__)) || defined(__CYGWIN__)
# define _XOPEN_SOURCE 700   /* for fdopen() */
#endif

#include <stdbool.h>
#include <stdio.h>
#ifdef VAXC
# include <file.h>
#else
# include <fcntl.h>
#endif
#if defined(_WIN32) || defined(CYGWIN)
# include <io.h>  /* for setmode() */
#else
# ifdef UNIX
#  include <unistd.h>
# endif
#endif
#include <stdlib.h>
#include <string.h>  /* for strncmp() */
#include <ctype.h>  /* for isalnum() */
#include <limits.h>
#if __MWERKS__ && !defined(BEBOX)
# include <unix.h>  /* for fdopen() on MAC */
#endif


/*  This corrects the problem of missing prototypes for certain functions
 *  in some GNU installations (e.g. SunOS 4.1.x).
 *  Darren Hiebert <darren@hmi.com> (sparc-sun-sunos4.1.3_U1/2.7.2.2)
 */
#if defined(__GNUC__) && defined(__STDC__)
# ifndef __USE_FIXED_PROTOTYPES__
#  define __USE_FIXED_PROTOTYPES__
# endif
#endif

#ifndef __USE_FIXED_PROTOTYPES__
/*
 * This is historic and works only if the compiler really has no prototypes:
 *
 * Include prototypes for Sun OS 4.x, when using an ANSI compiler.
 * FILE is defined on OS 4.x, not on 5.x (Solaris).
 * if __SVR4 is defined (some Solaris versions), don't include this.
 */
#if defined(sun) && defined(FILE) && !defined(__SVR4) && defined(__STDC__)
#  define __P(a) a
/* excerpt from my sun_stdlib.h */
extern int fprintf __P((FILE*, char*, ...));
extern int fputs   __P((char*, FILE*));
extern int _flsbuf __P((unsigned char, FILE*));
extern int _filbuf __P((FILE*));
extern int fflush  __P((FILE*));
extern int fclose  __P((FILE*));
extern int fseek   __P((FILE*, long, int));
extern int rewind  __P((FILE*));

extern void perror __P((char*));
# endif
#endif

char version[] = "xxd 2022-01-14 by Juergen Weigert et al. (2024-08-06 standalone-port ckormanyos)";
#ifdef _WIN32
char osver[] = " (Win32)";
#else
char osver[] = "";
#endif

#if defined(_WIN32)
# define BIN_READ(yes)  ((yes) ? "rb" : "rt")
# define BIN_WRITE(yes) ((yes) ? "wb" : "wt")
# define BIN_CREAT(yes) ((yes) ? (O_CREAT|O_BINARY) : O_CREAT)
# define BIN_ASSIGN(fp, yes) setmode(fileno(fp), (yes) ? O_BINARY : O_TEXT)
# define PATH_SEP '\\'
#elif defined(CYGWIN)
# define BIN_READ(yes)  ((yes) ? "rb" : "rt")
# define BIN_WRITE(yes) ((yes) ? "wb" : "w")
# define BIN_CREAT(yes) ((yes) ? (O_CREAT|O_BINARY) : O_CREAT)
# define BIN_ASSIGN(fp, yes) ((yes) ? (void) setmode(fileno(fp), O_BINARY) : (void) (fp))
# define PATH_SEP '/'
#else
# ifdef VMS
#  define BIN_READ(dummy)  "r"
#  define BIN_WRITE(dummy) "w"
#  define BIN_CREAT(dummy) O_CREAT
#  define BIN_ASSIGN(fp, dummy) fp
#  define PATH_SEP ']'
#  define FILE_SEP '.'
# else
#  define BIN_READ(dummy)  "r"
#  define BIN_WRITE(dummy) "w"
#  define BIN_CREAT(dummy) O_CREAT
#  define BIN_ASSIGN(fp, dummy) fp
#  define PATH_SEP '/'
# endif
#endif

/* open has only to arguments on the Mac */
#if __MWERKS__
# define OPEN(name, mode, umask) open(name, mode)
#else
# define OPEN(name, mode, umask) open(name, mode, umask)
#endif

#ifdef AMIGA
# define STRNCMP(s1, s2, l) strncmp(s1, s2, (size_t)l)
#else
# define STRNCMP(s1, s2, l) strncmp(s1, s2, l)
#endif

#ifndef __P
# if defined(__STDC__) || defined(_WIN32)
#  define __P(a) a
# else
#  define __P(a) ()
# endif
#endif

#define TRY_SEEK  /* attempt to use lseek, or skip forward by reading */
#define COLS 256  /* change here, if you ever need more columns */
#define LLEN ((2*(int)sizeof(unsigned long)) + 4 + (9*COLS-1) + COLS + 2)

const char hexxa[] = "0123456789abcdef0123456789ABCDEF";
const char* hexx = hexxa;

/* the different hextypes known by this program: */
#define HEX_NORMAL 0
#define HEX_POSTSCRIPT 1
#define HEX_CINCLUDE 2
#define HEX_BITS 3    /* not hex a dump, but bits: 01111001 */
#define HEX_LITTLEENDIAN 4

#define CONDITIONAL_CAPITALIZE(c) (capitalize ? toupper((int)c) : c)

static char* pname;

static void
exit_with_usage(void)
{
  fprintf(stderr, "Usage:\n       %s [options] [infile [outfile]]\n", pname);
  fprintf(stderr, "    or\n       %s -r [-s [-]offset] [-c cols] [-ps] [infile [outfile]]\n", pname);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "    -a          toggle autoskip: A single '*' replaces nul-lines. Default off.\n");
  fprintf(stderr, "    -b          binary digit dump (incompatible with -ps,-i,-r). Default hex.\n");
  fprintf(stderr, "    -C          capitalize variable names in C include file style (-i).\n");
  fprintf(stderr, "    -c cols     format <cols> octets per line. Default 16 (-i: 12, -ps: 30).\n");
  fprintf(stderr, "    -E          show characters in EBCDIC. Default ASCII.\n");
  fprintf(stderr, "    -e          little-endian dump (incompatible with -ps,-i,-r).\n");
  fprintf(stderr, "    -g bytes    number of octets per group in normal output. Default 2 (-e: 4).\n");
  fprintf(stderr, "    -h          print this summary.\n");
  fprintf(stderr, "    -i          output in C include file style.\n");
  fprintf(stderr, "    -n name     set the variable name used in C include output (-i).\n");
  fprintf(stderr, "    -l len      stop after <len> octets.\n");
  fprintf(stderr, "    -o off      add <off> to the displayed file position.\n");
  fprintf(stderr, "    -ps         output in postscript plain hexdump style.\n");
  fprintf(stderr, "    -r          reverse operation: convert (or patch) hexdump into binary.\n");
  fprintf(stderr, "    -r -s off   revert with <off> added to file positions found in hexdump.\n");
  fprintf(stderr, "    -d          show offset in decimal instead of hex.\n");
  fprintf(stderr, "    -s %sseek  start at <seek> bytes abs. %sinfile offset.\n",
#ifdef TRY_SEEK
          "[+][-]", "(or +: rel.) ");
#else
          "", "");
#endif
  fprintf(stderr, "    -u          use upper case hex letters.\n");
  fprintf(stderr, "    -v          show version: \"%s%s\".\n", version, osver);
  exit(1);
}

static void
perror_exit(int ret)
{
  fprintf(stderr, "%s: ", pname);
  perror(NULL);
  exit(ret);
}

static void
error_exit(int ret, char* msg)
{
  fprintf(stderr, "%s: %s\n", pname, msg);
  exit(ret);
}

static int
fgetc_or_die(FILE* fpi)
{
  int c = fgetc(fpi);

  if(c == EOF && ferror(fpi))
  {
    perror_exit(2);
  }

  return c;
}

static void
fputc_or_die(int c, FILE* fpo)
{
  if(fputc(c, fpo) == EOF)
  {
    perror_exit(3);
  }
}

static void
fputs_or_die(const char* s, FILE* fpo)
{
  if(fputs(s, fpo) == EOF)
  {
    perror_exit(3);
  }
}

/* Use a macro to allow for different arguments. */
#define FPRINTF_OR_DIE(args) if (fprintf args < 0) perror_exit(3)

static void
fclose_or_die(FILE* fpi, FILE* fpo)
{
  if(fclose(fpo) != 0)
  {
    perror_exit(3);
  }

  if(fclose(fpi) != 0)
  {
    perror_exit(2);
  }
}

/*
 * If "c" is a hex digit, return the value.
 * Otherwise return -1.
 */
static int
parse_hex_digit(int c)
{
  return   (c >= '0' && c <= '9') ? c - '0'
         : (c >= 'a' && c <= 'f') ? c - 'a' + 10
         : (c >= 'A' && c <= 'F') ? c - 'A' + 10
         : -1;
}

/*
 * Ignore text on "fpi" until end-of-line or end-of-file.
 * Return the '\n' or EOF character.
 * When an error is encountered exit with an error message.
 */
static int
skip_to_eol(FILE* fpi, int c)
{
  while(c != '\n' && c != EOF)
  {
    c = fgetc_or_die(fpi);
  }

  return c;
}

/*
 * Max. cols binary characters are decoded from the input stream per line.
 * Two adjacent garbage characters after evaluated data delimit valid data.
 * Everything up to the next newline is discarded.
 *
 * The name is historic and came from 'undo type opt h'.
 */
static int
huntype(
  FILE* fpi,
  FILE* fpo,
  int cols,
  int hextype,
  long base_off)
{
  int ign_garb = 1;

  int n1 = -1;
  int n2 = 0;
  int p  = cols;

  long have_off = 0;
  long want_off = 0;

  rewind(fpi);

  int c;

  while((c = fgetc(fpi)) != EOF)
  {
    /* Doze style input file?
     * Or also:
     * Allow multiple spaces. This doesn't work when there is normal
     * text after the hex codes in the last line that looks like hex,
     * thus only use it for PostScript format.
     */

    bool skip_this_char =
    (
      (c == '\r') || (hextype == HEX_POSTSCRIPT && (c == ' ' || c == '\n' || c == '\t'))
    );

    if(!skip_this_char)
    {
      const int n3 = n2;

      n2 = n1;

      n1 = parse_hex_digit(c);

      if(!(n1 == -1 && ign_garb))
      {
        ign_garb = 0;

        if((hextype == HEX_NORMAL) && (p >= cols))
        {
          if(n1 < 0)
          {
            p = 0;

            skip_this_char = true;
          }

          if(!skip_this_char) { want_off = (want_off << 4) | n1; }

          skip_this_char = true;
        }

        if(!skip_this_char)
        {
          if(base_off + want_off != have_off)
          {
            if(fflush(fpo) != 0)
            {
              perror_exit(3);
            }

#ifdef TRY_SEEK

            if(fseek(fpo, base_off + want_off - have_off, SEEK_CUR) >= 0)
            {
              have_off = base_off + want_off;
            }

#endif

            if(base_off + want_off < have_off)
            {
              error_exit(5, "Sorry, cannot seek backwards.");
            }

            for(; have_off < base_off + want_off; have_off++)
            {
              fputc_or_die(0, fpo);
            }
          }

          if(n2 >= 0 && n1 >= 0)
          {
            fputc_or_die((n2 << 4) | n1, fpo);
            have_off++;
            want_off++;
            n1 = -1;

            if(hextype == HEX_NORMAL)
            {
              ++p;

              if(p >= cols)
              {
                /* skip the rest of the line as garbage */
                c = skip_to_eol(fpi, c);
              }
            }
          }
          else if(n1 < 0 && n2 < 0 && n3 < 0)
          {
            /* already stumbled into garbage, skip line, wait and see */
            c = skip_to_eol(fpi, c);
          }

          if(c == '\n')
          {
            if(hextype == HEX_NORMAL)
            {
              want_off = 0;
            }

            p = cols;

            ign_garb = 1;
          }
        }
      }
    }
  }

  if(fflush(fpo) != 0)
  {
    perror_exit(3);
  }

#ifdef TRY_SEEK
  fseek(fpo, 0L, SEEK_END);
#endif
  return 0;
}

/*
 * Print line l. If nz is false, xxdline regards the line a line of
 * zeroes. If there are three or more consecutive lines of zeroes,
 * they are replaced by a single '*' character.
 *
 * If the output ends with more than two lines of zeroes, you
 * should call xxdline again with l being the last line and nz
 * negative. This ensures that the last line is shown even when
 * it is all zeroes.
 *
 * If nz is always positive, lines are never suppressed.
 */
static void
xxdline(FILE* fp, const char* l, const int nz)
{
  static char z[LLEN + 1];
  static int zero_seen = 0;

  if(!nz && zero_seen == 1)
  {
    strcpy(z, l);
  }

  if(nz || !zero_seen++)
  {
    if(nz)
    {
      if(nz < 0)
      {
        zero_seen--;
      }

      if(zero_seen == 2)
      {
        fputs_or_die(z, fp);
      }

      if(zero_seen > 2)
      {
        fputs_or_die("*\n", fp);
      }
    }

    if(nz >= 0 || zero_seen > 0)
    {
      fputs_or_die(l, fp);
    }

    if(nz)
    {
      zero_seen = 0;
    }
  }
}

/* This is an EBCDIC to ASCII conversion table */
/* from a proposed BTL standard April 16, 1979 */
static unsigned char etoa64[] =
{
  0040, 0240, 0241, 0242, 0243, 0244, 0245, 0246,
  0247, 0250, 0325, 0056, 0074, 0050, 0053, 0174,
  0046, 0251, 0252, 0253, 0254, 0255, 0256, 0257,
  0260, 0261, 0041, 0044, 0052, 0051, 0073, 0176,
  0055, 0057, 0262, 0263, 0264, 0265, 0266, 0267,
  0270, 0271, 0313, 0054, 0045, 0137, 0076, 0077,
  0272, 0273, 0274, 0275, 0276, 0277, 0300, 0301,
  0302, 0140, 0072, 0043, 0100, 0047, 0075, 0042,
  0303, 0141, 0142, 0143, 0144, 0145, 0146, 0147,
  0150, 0151, 0304, 0305, 0306, 0307, 0310, 0311,
  0312, 0152, 0153, 0154, 0155, 0156, 0157, 0160,
  0161, 0162, 0136, 0314, 0315, 0316, 0317, 0320,
  0321, 0345, 0163, 0164, 0165, 0166, 0167, 0170,
  0171, 0172, 0322, 0323, 0324, 0133, 0326, 0327,
  0330, 0331, 0332, 0333, 0334, 0335, 0336, 0337,
  0340, 0341, 0342, 0343, 0344, 0135, 0346, 0347,
  0173, 0101, 0102, 0103, 0104, 0105, 0106, 0107,
  0110, 0111, 0350, 0351, 0352, 0353, 0354, 0355,
  0175, 0112, 0113, 0114, 0115, 0116, 0117, 0120,
  0121, 0122, 0356, 0357, 0360, 0361, 0362, 0363,
  0134, 0237, 0123, 0124, 0125, 0126, 0127, 0130,
  0131, 0132, 0364, 0365, 0366, 0367, 0370, 0371,
  0060, 0061, 0062, 0063, 0064, 0065, 0066, 0067,
  0070, 0071, 0372, 0373, 0374, 0375, 0376, 0377
};

/* static and non-local because it may be too big for stack */
static char non_local_buf[LLEN + 1];

int
main(int argc, char* argv[])
{
  int p = 0;
  int relseek = 1;
  int negseek = 0;
  int revert = 0;
  int cols = 0;
  int colsgiven = 0;
  int nonzero = 0;
  int autoskip = 0;
  int hextype = HEX_NORMAL;
  int capitalize = 0;
  int decimal_offset = 0;
  int ebcdic = 0;
  int octspergrp = -1;  /* number of octets grouped in output */
  int grplen;    /* total chars per octet group */

  long length = -1;
  long n = 0;
  long seekoff = 0;

  unsigned long displayoff = 0;

  char* pp;
  const char* varname = NULL;
  int addrlen = 9;

#ifdef AMIGA

  /* This program doesn't work when started from the Workbench */
  if(argc == 0)
  {
    exit(1);
  }

#endif

  pname = argv[0];

  for(pp = pname; *pp;)
  {
    if(*pp++ == PATH_SEP)
    {
      pname = pp;
    }
  }

#ifdef FILE_SEP

  for(pp = pname; *pp; pp++)
  {
    if(*pp == FILE_SEP)
    {
      *pp = '\0';
      break;
    }
  }

#endif

  while(argc >= 2)
  {
    pp = argv[1] + (!STRNCMP(argv[1], "--", 2) && argv[1][2]);

    if(!STRNCMP(pp, "-a", 2))
    {
      autoskip = 1 - autoskip;
    }
    else if(!STRNCMP(pp, "-b", 2))
    {
      hextype = HEX_BITS;
    }
    else if(!STRNCMP(pp, "-e", 2))
    {
      hextype = HEX_LITTLEENDIAN;
    }
    else if(!STRNCMP(pp, "-u", 2))
    {
      hexx = hexxa + 16;
    }
    else if(!STRNCMP(pp, "-p", 2))
    {
      hextype = HEX_POSTSCRIPT;
    }
    else if(!STRNCMP(pp, "-i", 2))
    {
      hextype = HEX_CINCLUDE;
    }
    else if(!STRNCMP(pp, "-C", 2) || (pp[2] && !STRNCMP(pp, "-capitalize", 11)))
    {
      capitalize = 1;
    }
    else if(!STRNCMP(pp, "-d", 2))
    {
      decimal_offset = 1;
    }
    else if(!STRNCMP(pp, "-r", 2))
    {
      revert++;
    }
    else if(!STRNCMP(pp, "-E", 2))
    {
      ebcdic++;
    }
    else if(!STRNCMP(pp, "-v", 2))
    {
      fprintf(stderr, "%s%s\n", version, osver);
      exit(0);
    }
    else if((!STRNCMP(pp, "-c", 2) && !(pp[2] && STRNCMP("-capitalize", pp, 11))) || (pp[2] && !STRNCMP("-cols", pp, 5)))
    {
      if(!argv[2])
      {
        exit_with_usage();
      }

      colsgiven = 1;
      cols = (int)strtol(argv[2], NULL, 0);
      argv++;
      argc--;
    }
    else if(!STRNCMP(pp, "-g", 2) || (pp[2] && !STRNCMP("-group", pp, 6)))
    {
      if(!argv[2])
      {
        exit_with_usage();
      }

      octspergrp = (int)strtol(argv[2], NULL, 0);
      argv++;
      argc--;
    }
    else if (!STRNCMP(pp, "-n", 2) || (pp[2] && !STRNCMP("-name", pp, 5)))
    {
      if (!argv[2])
      {
        exit_with_usage();
      }

      varname = argv[2];
      argv++;
      argc--;
    }
    else if(!STRNCMP(pp, "-o", 2) || (pp[2] && !STRNCMP("-offset", pp, 7)))
    {
      int reloffset = 0;
      int negoffset = 0;

      if(!argv[2])
      {
        exit_with_usage();
      }

      if(argv[2][0] == '+')
      {
        reloffset++;
      }

      if(argv[2][reloffset] == '-')
      {
        negoffset++;
      }

      if(negoffset)
      {
        displayoff = ULONG_MAX - strtoul(argv[2] + reloffset + negoffset, NULL, 0) + 1;
      }
      else
      {
        displayoff = strtoul(argv[2] + reloffset + negoffset, NULL, 0);
      }

      argv++;
      argc--;
    }
    else if(!STRNCMP(pp, "-s", 2))
    {
      relseek = 0;
      negseek = 0;

      if(!argv[2])
      {
        exit_with_usage();
      }

#ifdef TRY_SEEK

      if(argv[2][0] == '+')
      {
        relseek++;
      }

      if(argv[2][relseek] == '-')
      {
        negseek++;
      }

#endif
      seekoff = strtol(argv[2] + relseek + negseek, (char**)NULL, 0);
      argv++;
      argc--;
    }
    else if(!STRNCMP(pp, "-l", 2) || !STRNCMP("-len", pp, 4))
    {
      if(!argv[2])
      {
        exit_with_usage();
      }

      length = strtol(argv[2], (char**)NULL, 0);
      argv++;
      argc--;
    }
    else if(!strcmp(pp, "--"))  /* end of options */
    {
      argv++;
      argc--;
      break;
    }
    else if(pp[0] == '-' && pp[1])  /* unknown option */
    {
      exit_with_usage();
    }
    else
    {
      break;  /* not an option */
    }

    argv++;        /* advance to next argument */
    argc--;
  }

  if(!colsgiven || (!cols && hextype != HEX_POSTSCRIPT))
  {
    switch(hextype)
    {
      case HEX_POSTSCRIPT:
        cols = 30;
        break;

      case HEX_CINCLUDE:
        cols = 12;
        break;

      case HEX_BITS:
        cols = 6;
        break;

      case HEX_NORMAL:
      case HEX_LITTLEENDIAN:
      default:
        cols = 16;
        break;
    }
  }

  if(octspergrp < 0)
  {
    switch(hextype)
    {
      case HEX_BITS:
        octspergrp = 1;
        break;

      case HEX_NORMAL:
        octspergrp = 2;
        break;

      case HEX_LITTLEENDIAN:
        octspergrp = 4;
        break;

      case HEX_POSTSCRIPT:
      case HEX_CINCLUDE:
      default:
        octspergrp = 0;
        break;
    }
  }

  if((hextype == HEX_POSTSCRIPT && cols < 0) ||
      (hextype != HEX_POSTSCRIPT && cols < 1) ||
      ((hextype == HEX_NORMAL || hextype == HEX_BITS || hextype == HEX_LITTLEENDIAN)
       && (cols > COLS)))
  {
    fprintf(stderr, "%s: invalid number of columns (max. %d).\n", pname, COLS);
    exit(1);
  }

  if(octspergrp < 1 || octspergrp > cols)
  {
    octspergrp = cols;
  }
  else if(hextype == HEX_LITTLEENDIAN && (octspergrp & (octspergrp - 1)))
  {
    error_exit(1, "number of octets per group must be a power of 2 with -e.");
  }

  if(argc > 3)
  {
    exit_with_usage();
  }

  FILE* fp;
  FILE* fpo = NULL;

  if(argc == 1 || (argv[1][0] == '-' && !argv[1][1]))
  {
    BIN_ASSIGN(fp = stdin, !revert);
  }
  else
  {
    if((fp = fopen(argv[1], BIN_READ(!revert))) == NULL)
    {
      fprintf(stderr, "%s: ", pname);
      error_exit(2, argv[1]);
    }
  }

  if(argc < 3 || (argv[2][0] == '-' && !argv[2][1]))
  {
    BIN_ASSIGN(fpo = stdout, revert);
  }
  else
  {
    const int mode = revert ? O_WRONLY : (O_TRUNC | O_WRONLY);

    const int fd = OPEN(argv[2], mode | BIN_CREAT(revert), 0666);

    if(fd < 0)
    {
      fprintf(stderr, "%s: ", pname);
      error_exit(3, argv[2]);
    }

    fpo = fdopen(fd, BIN_WRITE(revert));

    if(fpo == NULL)
    {
      fprintf(stderr, "%s: ", pname);
      error_exit(3, argv[2]);
    }

    rewind(fpo);
  }

  if(revert)
  {
    if(hextype && (hextype != HEX_POSTSCRIPT))
    {
      error_exit(-1, "Sorry, cannot revert this type of hexdump");
    }

    const int result = huntype(fp, fpo, cols, hextype, negseek ? -seekoff : seekoff);

    fclose_or_die(fp, fpo);

    return result;
  }

  int e = 0;

  if(seekoff || negseek || !relseek)
  {
#ifdef TRY_SEEK

    if(relseek)
    {
      e = fseek(fp, negseek ? -seekoff : seekoff, SEEK_CUR);
    }
    else
    {
      e = fseek(fp, negseek ? -seekoff : seekoff, negseek ? SEEK_END : SEEK_SET);
    }

    if(e < 0 && negseek)
    {
      error_exit(4, "Sorry, cannot seek.");
    }

    if(e >= 0)
    {
      seekoff = ftell(fp);
    }
    else
#endif
    {
      long s = seekoff;

      while(s--)
      {
        if(fgetc_or_die(fp) == EOF)
        {
          error_exit(4, "Sorry, cannot seek.");
        }
      }
    }
  }

  int c;

  if(hextype == HEX_CINCLUDE)
  {
    if(varname == NULL && fp != stdin)
    {
      varname = argv[1];
    }

    if(varname != NULL)
    {
      FPRINTF_OR_DIE((fpo, "unsigned char %s", isdigit((int)varname[0]) ? "__" : ""));

      for(e = 0; (c = varname[e]) != 0; e++)
      {
        fputc_or_die(isalnum(c) ? CONDITIONAL_CAPITALIZE(c) : '_', fpo);
      }

      fputs_or_die("[] = {\n", fpo);
    }

    p = 0;

    while((length < 0 || p < length) && (c = fgetc_or_die(fp)) != EOF)
    {
      FPRINTF_OR_DIE((fpo, (hexx == hexxa) ? "%s0x%02x" : "%s0X%02X",
                      (p % cols) ? ", " : (!p ? "  " : ",\n  "), c));
      p++;
    }

    if(p)
    {
      fputs_or_die("\n", fpo);
    }

    if(varname != NULL)
    {
      fputs_or_die("};\n", fpo);
      FPRINTF_OR_DIE((fpo, "unsigned int %s", isdigit((int)varname[0]) ? "__" : ""));

      for(e = 0; (c = varname[e]) != 0; e++)
      {
        fputc_or_die(isalnum(c) ? CONDITIONAL_CAPITALIZE(c) : '_', fpo);
      }

      FPRINTF_OR_DIE((fpo, "_%s = %d;\n", capitalize ? "LEN" : "len", p));
    }

    fclose_or_die(fp, fpo);

    return 0;
  }

  if(hextype == HEX_POSTSCRIPT)
  {
    p = cols;

    while((length < 0 || n < length) && (e = fgetc_or_die(fp)) != EOF)
    {
      fputc_or_die(hexx[(e >> 4) & 0xf], fpo);
      fputc_or_die(hexx[e & 0xf], fpo);
      n++;

      if(cols > 0)
      {
        --p;

        if(!p)
        {
          fputc_or_die('\n', fpo);
          p = cols;
        }
      }
    }

    if(cols == 0 || p < cols)
    {
      fputc_or_die('\n', fpo);
    }

    fclose_or_die(fp, fpo);
    return 0;
  }

  /* hextype: HEX_NORMAL or HEX_BITS or HEX_LITTLEENDIAN */

  if(hextype != HEX_BITS)
  {
    grplen = octspergrp + octspergrp + 1;  /* chars per octet group */
  }
  else  /* hextype == HEX_BITS */
  {
    grplen = 8 * octspergrp + 1;
  }

  while((length < 0 || n < length) && (e = fgetc_or_die(fp)) != EOF)
  {
    int x;

    if(p == 0)
    {
      addrlen = sprintf(non_local_buf, decimal_offset ? "%08ld:" : "%08lx:",
                        ((unsigned long)(n + seekoff + (long) displayoff)));

      for(c = addrlen; c < LLEN; non_local_buf[c++] = ' ');
    }

    x = hextype == HEX_LITTLEENDIAN ? p ^ (octspergrp - 1) : p;
    c = addrlen + 1 + (grplen * x) / octspergrp;

    if(hextype == HEX_NORMAL || hextype == HEX_LITTLEENDIAN)
    {
      non_local_buf[c]   = hexx[(e >> 4) & 0xf];
      non_local_buf[++c] = hexx[e & 0xf];
    }
    else /* hextype == HEX_BITS */
    {
      for(int i = 7; i >= 0; i--)
      {
        non_local_buf[c++] = (e & (1 << i)) ? '1' : '0';
      }
    }

    if(e)
    {
      nonzero++;
    }

    if(ebcdic)
    {
      e = (e < 64) ? '.' : etoa64[e - 64];
    }

    /* When changing this update definition of LLEN above. */
    c = addrlen + 3 + (grplen * cols - 1) / octspergrp + p;
    const bool char_is_in_range =
      #ifdef __MVS__
      ((e >= 64) ? true : false);
      #else
      ((e > 31 && e < 127) ? true : false);
      #endif

    non_local_buf[c++] = (char) (char_is_in_range ? e : '.');
    n++;

    if(++p == cols)
    {
      non_local_buf[c] = '\n';
      non_local_buf[++c] = '\0';
      xxdline(fpo, non_local_buf, autoskip ? nonzero : 1);
      nonzero = 0;
      p = 0;
    }
  }

  if(p)
  {
    non_local_buf[c] = '\n';
    non_local_buf[++c] = '\0';
    xxdline(fpo, non_local_buf, 1);
  }
  else if(autoskip)
  {
    xxdline(fpo, non_local_buf, -1);  /* last chance to flush out suppressed lines */
  }

  fclose_or_die(fp, fpo);

  return 0;
}

/* vi:set ts=8 sw=4 sts=2 cino+={2 cino+=n-2 : */
