#!/bin/bash

if [ -L "$0" ]; then
	. $(dirname $(readlink $0))/tools/x-colors
else
	. tools/x-colors
fi

export	PATH="/usr/lib/colorgcc/bin:$PATH"

tmp=`mktemp`
find . -name "*.cpp" -o -name "*.h" -o -name "*.ll.l" -o -name "*.yy.y" >$tmp
[ $? -ne 0 ] && echo "find error" && exit 1
sum=0

while	:;do
	n_sum=$(ls -l -- `cat $tmp`|md5sum|cut -c -32)
	if [ "$sum" != "$n_sum" ];then
		clear
		date
		make -s 2>&1
		r=$?
		msg	"**	make	ret:$r"
		sum=$n_sum
		if [ "$r" -eq 0 -a "x$1" != "x" ];then
			msg	"**	exec:$1"
			time ./$1
			r=$?
			msg	"**	$1	ret:$r"
			if [ $r == 139 ];then
				msg	"**	trace:"
				gdb -batch -x .gdb_auto_trace $1
				msg	"**	gdb ret:$?"
			fi
		fi
		msg	"** ready."
	fi
	sleep 0.4
done
