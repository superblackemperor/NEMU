#!/bin/bash

for file in `ls ~/ics2021/am-kernels/tests/cpu-tests/tests`;do
	n=`expr index "$file" .`
	n=`expr ${n} - 1`
	file=`expr substr ${file} 1 $n`
	echo $file
	make ARCH=$ISA-nemu ALL=$file run
done
