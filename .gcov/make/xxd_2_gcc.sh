#!/usr/bin/env bash
#
#  Copyright Christopher Kormanyos 2024.
#  Distributed under the Boost Software License,
#  Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# cd /mnt/c/Users/ckorm/Documents/Ks/PC_Software/xxd/.gcov/make
# ./xxd_2_gcc.sh

if [[ "$1" != "" ]]; then
  XXD="$1"
else
  XXD=./bin/xxd
fi


echo "#include <stdio.h>"                                                                                             > example.c
echo "#include <string.h>"                                                                                           >> example.c
echo ""                                                                                                              >> example.c
echo -n "Hello, World!" > example.bin && $XXD -i example.bin                                                         >> example.c
echo ""                                                                                                              >> example.c
echo "int main()"                                                                                                    >> example.c
echo "{"                                                                                                             >> example.c
echo "  const int result = (strncmp(\"Hello, World!\", (const char*) example_bin, example_bin_len) == 0) ? 0 : -1;"  >> example.c
echo "  printf(\"%s\n\",example_bin);"                                                                               >> example.c
echo "  return result;"                                                                                              >> example.c
echo "}"                                                                                                             >> example.c
g++ -Wall -Wextra -O2 -x c example.c -o example
./example
result=$?
echo


result_total=$((result))


echo "result_total : "  "$result_total"
echo "xxd_2_gcc"
echo


exit $result_total
