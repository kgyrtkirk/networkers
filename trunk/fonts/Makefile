GENTEXFONT=~/texfont/gentexfont
F=-*-courier-*-r-*-*-*-100-*-*-*-*-*-*
#GOPTS=-w 1024 -h 1024

all:	default.txf	courier.txf lucida.txf swis721.txf

default.txf:	Makefile
#	echo $(GENTEXFONT) -w 512 -h 512 -glist ' ABCDEFGHIJKLMNOPQRSTUVWXYZ'	$@
	$(GENTEXFONT) $(GOPTS) -glist ' ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijmklmnopqrstuvwxyz?.;,!*:"/+@#$%^&()#|\\><-'	\
		-file $@

courier.txf:	Makefile
#	echo $(GENTEXFONT) -w 512 -h 512 -glist ' ABCDEFGHIJKLMNOPQRSTUVWXYZ'	$@
	$(GENTEXFONT) $(GOPTS) -glist ' ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijmklmnopqrstuvwxyz?.;,!*:"/+@#$%^&()#|\\><-'	\
		-file $@	\
		-fn '-*-courier-*-r-*-*-34-*-*-*-*-*-*-*'
#		-fn '-*-courier-*-r-*-*-*-100-*-*-*-*-*-*'


#lucida.txf:	Makefile
#	$(GENTEXFONT) $(GOPTS) -glist ' ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijmklmnopqrstuvwxyz?.;,!*:"/+@#$%^&()#|\\><-'	\
#		-file $@	\
#		-fn '-*-lucida-*-*-*-*-*-240-*-*-*-*-*-*'
##		-fn '-*-*-*-r-*-*-*-240-*-*-*-*-*-*'
##		-fn '-*-courier-*-r-*-*-*-100-*-*-*-*-*-*'

lucida.txf:	Makefile
	$(GENTEXFONT) $(GOPTS) -glist ' ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijmklmnopqrstuvwxyz?.;,!*:"/+@#$%^&()#|\\><-'	\
		-file $@ -w 512	\
		-fn '-*-*-*-*-*-*-*-360-*-*-*-*-*-*'
#		-fn '-*-*-*-r-*-*-*-240-*-*-*-*-*-*'
#		-fn '-*-courier-*-r-*-*-*-100-*-*-*-*-*-*'

swis721.txf:	Makefile
	$(GENTEXFONT) $(GOPTS) -glist ' ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijmklmnopqrstuvwxyz?.;,!*:"/+@#$%^&()#|\\><-'	\
		-file $@ 	\
		-fn '-*-swis721 bt-*-*-*-*-*-*-*-*-*-*-*-*'


#courier.txf:	Makefile
#	echo $(GENTEXFONT) -w 512 -h 512 -glist ' ABCDEFGHIJKLMNOPQRSTUVWXYZ'	$@
#	$(GENTEXFONT) -glist ' ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijmklmnopqrstuvwxyz?.;,!*:"/+@#$%^&()#|\\><-'	\
#		-file $@	\
#		-fn '-*-courier-*-r-*-*-34-*-*-*-*-*-*-*'


