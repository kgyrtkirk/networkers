#!/bin/bash


cat $1 | grep GLAPIENTRY | grep '^GLAPI' | tr '*(' '  ' | awk '{print "#define	"$4"(P0,PARAMS...)	GLD("$4",P0,##PARAMS)"}' |grep -v 'GLAPIENTRY'|grep -v 'glGetError' > $2
