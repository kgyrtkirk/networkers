#!/bin/bash


times=3

NAME=$1

line="`grep '^S' ./extra/signals.h|grep -E "[ 	]$NAME[	 ,]"|tr -d '(),'|sed 's/^S//'`"
[ "$line" == "" ] && echo "can't find: $NAME" && exit 1

name="`echo $line|awk '{print $1}'`"
loop="`echo $line|awk '{print $2}'`"
seq="`echo $line|cut -d ' ' -f 3-`"

make basic_signals

gen()
{
	local	seq
	seq="$*"
	t=1
	names="$(
	echo $seq|tr ' ' '\n'|while read s;do
		[ $s -eq 0 ] && break
		echo build/signal-$s-$t.wav
		t=$[ ( $t + 1 ) % 2 ]
	done	)"
	echo $names
	sox $names signal-$name.wav
#tr ' ' '\n'|sed 's/^/signal-/g;s/$/.wav/g'
}
#echo $seq
gen $seq


play()
{
	echo	"playing:	$name	loop:$loop"
	artsplay signal-$NAME.wav
}

while [ $times -gt 0 ];do
	times=$[ $time - 1 ]
	play
done
