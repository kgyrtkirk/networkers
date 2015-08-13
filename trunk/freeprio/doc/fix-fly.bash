#!/bin/bash

[ "$1" == "flyer.tex" ] && exit 0
[ ! -e "$1" ] && echo "usage fasdf" && exit 1
grep "^% FIXXER" $1 | awk '{print "s/"$3"/"$4"/g;"}'
exp="`grep "^% FIXXER" flyer.tex | awk '{print "s/"$3"/"$4"/g;"}'`"
echo $exp
sed -r "/^% FIXXER/{p;d};${exp}" -i.last "$1"
