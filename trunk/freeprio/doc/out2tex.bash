#!/bin/bash

[ ! -s "$1" ] && echo "hi there!" && exit 1

cat $1|grep -v '0$'|grep -v '^-'|grep -v '^$'|sed -r 's/ +/\t/g'|
		sed -r 's/([0-9]+)\|([0-9]+)/ \1 \& \2 /'|cut -f 3,5-|tr '\t|' '&'|sed -r 's+&0+\&\\XZERO+g;s+&1+\&\\XONE+g;s+$+\\\\+;s+^+\\ROWCOLOR +'
