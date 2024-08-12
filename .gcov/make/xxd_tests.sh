#!/usr/bin/env bash
#
#  Copyright Christopher Kormanyos 2024.
#  Distributed under the Boost Software License,
#  Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#

# cd /mnt/c/Users/ckorm/Documents/Ks/PC_Software/xxd/.gcov/make
# ./xxd_tests.sh

if [[ "$1" != "" ]]; then
  XXD="$1"
else
  XXD=./bin/xxd
fi

echo 'test with -help'
$XXD -help 2> helpfile.txt
grep 'Usage' helpfile.txt
res_00=$?
echo

echo 'create zero_a.bin'
echo '010000: 41' | $XXD -r > zero_a.bin
sha1sum zero_a.bin | grep 'e534caeb1e149b6bc61ea48cf59861c7e15caa63'
res_00=$?
echo

$XXD -l 120 -c 12 ../../doc/xxd.1 | grep 'Tony Nugent'
res_01=$?
echo

$XXD -l 120 -c 12 ../../doc/xxd.1 > xxd.1.txt
unix2dos xxd.1.txt
sha1sum xxd.1.txt | grep b271bc7fc3716a44830bc33ee3a8313c0d3ab7f7
res_02=$?
echo

$XXD -s 0x36 -l 13 -c 13 ../../doc/xxd.1 | grep '21st May 1996'
res_03=$?
echo

echo 'create a 3-byte abc-file'
echo -n '0:61 1:62 2:63' | $XXD -r | sha1sum | grep 'a9993e364706816aba3e25717850c26c9cd0d89d'
res_04=$?
echo

echo -n 'Hello, World!' > testfile.txt
$XXD -p testfile.txt | grep '48656c6c6f2c20576f726c6421'
res_05=$?
echo

echo -n 'Hello, World!' > testfile.txt
$XXD -n 5 testfile.txt | grep 'Hello'
res_06=$?
echo

echo -n '0123456789' > testfile.txt
$XXD -group 5 testfile.txt | grep '3031323334 3536373839'
res_07=$?
echo

echo -e -n '\x01\x02\x03\x04\x05' > testbin.bin
$XXD testbin.bin | grep '00000000: 0102 0304 05'
res_08=$?
echo

echo -n 'Hello World!' > testfile.txt
$XXD -o 10 testfile.txt | grep '          H'
res_09=$?
echo

$XXD -o -2 testfile.txt | grep 'fffe'
res_10=$?
echo

$XXD -v 2> version.txt
grep 'ckormanyos' version.txt
res_11=$?
echo

echo -n 'AB' | $XXD -b | grep '01000001 01000010'
res_12=$?
echo

res_13=1
count=$(echo -e 'Hello, World!\nHello, World!\nHello, World!\nHello, World!' | $XXD -a -c 14 | grep -c 'Hello')
if [ "$count" -eq 4 ]; then
  res_13=0
fi

echo -n 'AB' | $XXD -e | grep ' 4241 '
res_14=$?
echo

echo -n 'lm' | $XXD | grep '6c6d'
res_15=$?
echo

echo -n 'lm' | $XXD -capitalize | grep '6c6d'
res_16=$?
echo

echo -n 'lm' | $XXD -u | grep '6C6D'
res_17=$?
echo

echo -n 'abc' | $XXD -o 16 | grep '00000010: 6162 63'
res_18=$?
echo

echo -n 'abc' | $XXD -o 16 -d | grep '00000016: 6162 63'
res_19=$?
echo

echo -n 'abc' | $XXD -cols 1 | grep '00000002: 63  c'
res_20=$?
echo

echo -n 'AB' | $XXD -ps | grep '4142'
res_21=$?
echo

echo -n 'AB' | $XXD -b -r 2> error.txt
grep 'Sorry' error.txt
res_22=$?
echo

echo -n 'Hello World!' | $XXD -s 1 | grep ' ello'
res_23=$?
echo

echo -n 'Hello World!' | $XXD -seek +1 -skip +1 | grep ' ello'
res_24=$?
echo

echo -n 'Hello World!' | $XXD -seek +3 -skip +2 | grep ' llo'
res_25=$?
echo

echo -n '0123456789' | $XXD -cols 2 | grep '00000008: 3839'
res_26=$?
echo

echo -n '0123456789' | $XXD -cols 2 -i | grep '0x38, 0x39'
res_27=$?
echo

echo -n 'AB' | $XXD -e -- | grep ' 4241 '
res_28=$?
echo

echo -n '00000000: 4865 6c6c 6f20 576f 726c 640a            Hello World.' > input.hex && $XXD -r -C input.hex output.txt
grep 'Hello' output.txt
res_29=$?
echo

echo -n '00000000: 4865 6c6c 6f20 576f 726c 640a            Hello World.' > input.hex && $XXD -r -E input.hex output.txt
grep 'Hello' output.txt
res_30=$?
echo

echo -n '00000000: 4865 6c6c 6f20 576f 726c 640a            Hello World.' > input.hex && $XXD -r -cols input.hex output.txt
grep 'Hello' output.txt
res_31=$?
echo

echo -n 'abc' | $XXD -offset 16 -d | grep '00000016: 6162 63'
res_32=$?
echo

echo -n "Hello, World!" > example.bin && $XXD -i example.bin
res_33=$?
echo

echo '010000: 41' | $XXD -r > lots_of_zeros && $XXD -a -c 12 lots_of_zeros | grep '*'
res_34=$?
echo


result_total=$((res_00+res_01+res_02+res_03+res_04+res_05+res_06+res_07+res_08+res_09+res_10+res_11+res_12+res_13+res_14+res_15+res_16+res_17+res_18+res_19+res_20+res_21+res_22+res_23+res_24+res_25+res_26+res_27+res_28+res_29+res_30+res_31+res_32+res_33+res_34))


echo "res_00       : "  "$res_00"
echo "res_01       : "  "$res_01"
echo "res_02       : "  "$res_02"
echo "res_03       : "  "$res_03"
echo "res_04       : "  "$res_04"
echo "res_05       : "  "$res_05"
echo "res_06       : "  "$res_06"
echo "res_07       : "  "$res_07"
echo "res_08       : "  "$res_08"
echo "res_09       : "  "$res_09"
echo "res_10       : "  "$res_10"
echo "res_11       : "  "$res_11"
echo "res_12       : "  "$res_12"
echo "res_13       : "  "$res_13"
echo "res_14       : "  "$res_14"
echo "res_15       : "  "$res_15"
echo "res_16       : "  "$res_16"
echo "res_17       : "  "$res_17"
echo "res_18       : "  "$res_18"
echo "res_19       : "  "$res_19"
echo "res_20       : "  "$res_20"
echo "res_21       : "  "$res_21"
echo "res_22       : "  "$res_22"
echo "res_23       : "  "$res_23"
echo "res_24       : "  "$res_24"
echo "res_25       : "  "$res_25"
echo "res_26       : "  "$res_26"
echo "res_27       : "  "$res_27"
echo "res_28       : "  "$res_28"
echo "res_29       : "  "$res_29"
echo "res_30       : "  "$res_30"
echo "res_31       : "  "$res_31"
echo "res_32       : "  "$res_32"
echo "res_33       : "  "$res_33"
echo "res_34       : "  "$res_34"
echo "result_total : "  "$result_total"
echo "xxd_tests"
echo

exit $result_total
