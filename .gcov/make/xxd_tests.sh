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

echo -e -n '\x01\x02\x03\x04\x05' > testbin.bin
bin/xxd testbin.bin | grep '00000000: 0102 0304 05'
res_06=$?
echo

result_total=$((res_00+res_01+res_02+res_03+res_04+res_05+res_06))

echo "result_total : "  "$result_total"

exit $result_total
