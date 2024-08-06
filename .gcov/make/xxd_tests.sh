#!/usr/bin/env bash
#
#  Copyright Christopher Kormanyos 2024.
#  Distributed under the Boost Software License,
#  Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#

echo 'test with -help'
./bin/xxd -help | grep Usage
res_00=$?
echo

echo 'create zero_a.bin'
echo '010000: 41' | bin/xxd -r > zero_a.bin
sha1sum zero_a.bin | grep 'e534caeb1e149b6bc61ea48cf59861c7e15caa63'
res_00=$?
echo

bin/xxd -l 120 -c 12 ../../doc/xxd.1 | grep 'Tony Nugent'
res_01=$?
echo

bin/xxd -l 120 -c 12 ../../doc/xxd.1 > xxd.1.txt
unix2dos xxd.1.txt
sha1sum xxd.1.txt | grep b271bc7fc3716a44830bc33ee3a8313c0d3ab7f7
res_02=$?
echo

bin/xxd -s 0x36 -l 13 -c 13 ../../doc/xxd.1 | grep '21st May 1996'
res_03=$?
echo

echo 'create a 3-byte abc-file'
echo -n '0:61 1:62 2:63' | bin/xxd -r > abc.bin
sha1sum abc.bin | grep 'a9993e364706816aba3e25717850c26c9cd0d89d'
res_04=$?
echo

echo -n 'Hello, World!' > testfile.txt
bin/xxd -p testfile.txt | grep '48656c6c6f2c20576f726c6421'
res_05=$?
echo

echo -n 'Hello, World!' > testfile.txt
bin/xxd -n 5 testfile.txt | grep 'Hello'
res_06=$?
echo

echo -n '0123456789' > testfile.txt
bin/xxd -g 5 testfile.txt | grep '3031323334 3536373839'
res_07=$?
echo

echo -e -n '\x01\x02\x03\x04\x05' > testbin.bin
bin/xxd testbin.bin | grep '00000000: 0102 0304 05'
res_08=$?
echo

echo -n 'Hello World!' > testfile.txt
bin/xxd -o 10 testfile.txt | grep '          H'
res_09=$?
echo

bin/xxd -o -2 testfile.txt | grep 'fffe'
res_10=$?
echo

bin/xxd -v 2> version.txt
grep 'ckormanyos' version.txt
res_11=$?
echo

result_total=$((res_00+res_01+res_02+res_03+res_04+res_05+res_06+res_07+res_08+res_09+res_10+res_11))

echo "result_total : "  "$result_total"
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
echo

exit $result_total
