#!/bin/bash

#PICC='taskset 2 wine "c:\Program\\\ Files\\hi-tech\ software\\picc-18\\9.50\\bin\\picc18.exe"'
#PICC='taskset 2 wine picc18'
PICC='wine picc18'

. tools/x-colors
tmp=`mktemp`

$PICC	 $* > $tmp 2>&1
r=$?

cat $tmp|while read line;do


        if [ "${line##* error*}" == "" ];then
                err "$line"
                continue;
        fi

        if [ "${line##* warning*}" == "" ];then
                if [ "${line##*(359)*}" == "" ];then
                        err "$line"
                else
                        wrn "$line"
                fi
                continue;
        fi
	echo	"$line"
done

unlink $tmp

exit	$r
