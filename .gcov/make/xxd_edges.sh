#!/usr/bin/env bash
#
#  Copyright Christopher Kormanyos 2024.
#  Distributed under the Boost Software License,
#  Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# cd /mnt/c/Users/ckorm/Documents/Ks/PC_Software/xxd/.gcov/make
# ./xxd_edges.sh

if [[ "$1" != "" ]]; then
  XXD="$1"
else
  XXD=./bin/xxd
fi


res_00=1
echo -n '0123456789' | $XXD -g 2>/dev/null
exit_code=$?
echo

if [ $exit_code -ne 0 ]; then
  res_00=0
fi

res_01=1
echo -n 'abc' | $XXD -cols 2>/dev/null
exit_code=$?
echo

if [ $exit_code -ne 0 ]; then
  res_01=0
fi

res_02=1
echo -n 'abc' | $XXD -o 2>/dev/null
exit_code=$?
echo

if [ $exit_code -ne 0 ]; then
  res_02=0
fi


res_03=1
echo -n 'Hello, World!' | $XXD -c 2>/dev/null
exit_code=$?
echo

if [ $exit_code -ne 0 ]; then
  res_03=0
fi


res_04=1
echo -n 'Hello, World!' | $XXD -l 2>/dev/null
exit_code=$?
echo

if [ $exit_code -ne 0 ]; then
  res_04=0
fi

res_05=1
echo -n 'Hello, World!' | $XXD -n 2>/dev/null
exit_code=$?
echo

if [ $exit_code -ne 0 ]; then
  res_05=0
fi

res_06=1
echo "010: 4141" | $XXD -r -s -0x11 2>/dev/null > a_file
exit_code=$?
echo

if [ $exit_code -ne 0 ]; then
  res_06=0
fi

res_07=1
echo -n 'Hello World!' | $XXD -s 2>/dev/null
exit_code=$?
echo

if [ $exit_code -ne 0 ]; then
  res_07=0
fi

res_08=1
echo -n 'Hello, World!' | $XXD -c -4 2>/dev/null
exit_code=$?
echo

if [ $exit_code -ne 0 ]; then
  res_08=0
fi


result_total=$((res_00+res_01+res_02+res_03+res_04+res_05+res_06+res_07+res_08))


echo "res_00       : "  "$res_00"
echo "res_01       : "  "$res_01"
echo "res_02       : "  "$res_02"
echo "res_03       : "  "$res_03"
echo "res_04       : "  "$res_04"
echo "res_05       : "  "$res_05"
echo "res_06       : "  "$res_06"
echo "res_07       : "  "$res_07"
echo "res_08       : "  "$res_08"
echo "result_total : "  "$result_total"
echo "xxd_edges"
echo

exit $result_total
