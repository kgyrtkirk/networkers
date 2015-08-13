#!/bin/bash

#1 565.0 575.0
#2 25.0 185.0
#3 345.0 750.0
#4 100.0 100.0

tmp="`mktemp`"
cat > $tmp

N=`cat "$tmp"|wc -l `
i=1
((	cat;
	cat $tmp|	while read l;do
		echo "$i $l"
		i=$[ $i + 1 ]
	done )|
	 ./geosteiner/lib_points | ./geosteiner/efst | ./geosteiner/bb_tx | grep '@@E'|cut -d 'E' -f 2-

)<< EOF
NAME: berlin52
TYPE: TSP
COMMENT: $N locations in Berlin (Groetschel)
DIMENSION: $N
EDGE_WEIGHT_TYPE: EUC_2D
NODE_COORD_SECTION
EOF
#cat $tmp
unlink $tmp

