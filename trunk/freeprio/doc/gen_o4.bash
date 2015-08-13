cat ../output4 | tr -d 'X'|awk '{print $1,$2}'|grep -v '^$'|tr '|' '&'|while read l;do

	if [ "x$l" == "x" ];then
		echo	'\hline'
		continue;
	fi
	echo	-n	"${l/ */}"
	echo		"`echo "${l/* }"|sed -r 's/(.)/\&\1/g;s/1/\\\\XONE/g;s/0/\\\\XZERO/g'`"
	echo		'\\'
done
