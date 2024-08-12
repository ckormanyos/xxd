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


result_total=$((res_00+res_01+res_02))


echo "result_total : "  "$result_total"
echo "res_00       : "  "$res_00"
echo "res_01       : "  "$res_01"
echo "res_02       : "  "$res_02"
echo

exit $result_total
