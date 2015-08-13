#CC=ccache gcc
#CC=gcc
DEPDIR		=	.deps
BUILD	=	build
SHELL	=	/bin/bash

build/%.o:	%.cpp
	@echo	"CXX	$@"
	gcc $(CXXFLAGS) -c -o build/$*.o  $*.cpp

build/%.o:	%.c
	@echo	"CC	$@"
	gcc $(CFLAGS) -c -o build/$*.o  $*.c

build:
	@echo	"MKDIR	$@"
	mkdir -p build $(addprefix build/,$(SUBDIRS))

%.cpp:	%.l
	@echo	"FLEX	$@"
	flex  -d --header=$*.h -o $*.cpp $*.l

%.cpp:	%.y
	@echo	"BISON	$@"
	bison -d -t -k -o $*.cpp $*.y

#%.h:		%.h.m4
#	@echo	"M4	$@"
#	m4 -P -I $(dir $@) $*.h.m4 > $*.h

$(DEPDIR)/%.m4.d:	%.m4.h
	#echo $(M4_SOURCES)
	@echo	"M4D	$<"
	echo	-n	"$*.h:	$*.m4.h	"	>	$(DEPDIR)/$*.m4.d
	grep ^//M4_DEP: $<  |	cut -c 10-|	tr '\n' ' ' >>	$(DEPDIR)/$*.m4.d

%.h:	%.m4.h
	@echo	"M4	$@"
	m4 -P -I $(dir $@) $*.m4.h	>	$*.h

#app/alarm_config_defs.h

#build/%.d: %.c
#	@echo	"DEP	$<"
#	echo sed 's/\('${ESCAPED}'\)\.o[ :]*/build\1.o $@ : /g'
#	Q="$*";\
#	ESCAPED="$${Q/\//\\/}";\
#	set;\
#	set -e; \
#	$(DEPCC) -MM $(CFLAGS) $< \
##	| sed "s+\($$ESCAPED\)\.o[ :]*/build\1.o $@ : /g' > $@; \
#	[ -s $@ ] || rm -f $@ 

$(DEPDIR)/%.d: %.c
	@echo	"DEP	$<"
#	echo "$(basename $*).o"	\
#	|	sed 's+\($*\)\.o[ :]*+build\/\1.o $@ : +g'
#	|	sed -r 's+\([^.]*\)\.o[ :]*+build\/$*.o $@ : +g' >$@ ;
	set -e; \
	set +o pipefail; \
	[ ! -d "$(dir $@)" ] && mkdir -p "$(dir $@)";\
	$(DEPCC) -MM $(CFLAGS) $< \
	|	sed -r 's+.*:+build\/$*.o $@ : +' >$@ ;\
	[ -s $@ ] || rm -f $@ 

#	| sed 's+\($*\)\.o[ :]*+build\/\1.o $@ : +g' > $@; 

$(DEPDIR)/%.d: %.cpp
	@echo	"DEP	$<"
#	echo "$(basename $*).o"	\
#	|	sed 's+\($*\)\.o[ :]*+build\/\1.o $@ : +g'
#	|	sed -r 's+\([^.]*\)\.o[ :]*+build\/$*.o $@ : +g' >$@ ;
	set -e; \
	[ ! -d "$(dir $@)" ] && mkdir -p "$(dir $@)";\
	$(DEPCC) -MM $(CXXFLAGS) $< \
	|	sed -r 's+.*:+build\/$*.o $@ : +' >$@ ;\
	[ -s $@ ] || rm -f $@ 

#	| sed 's+\($*\)\.o[ :]*+build\/\1.o $@ : +g' > $@; 

#%.d: %.h
#	@echo	"DEP	$<"
#	set -e; \
#	$(DEPCC) -MM $(CFLAGS) $< \
#	| sed 's/\($*\)\.o[ :]*/build\/\1.h $@ : /g' > $@; \
#	[ -s $@ ] || rm -f $@ 

#build/%.d: %.cpp
#	@echo	"DEP	$<"
#	set -e; \
#	$(DEPCXX) -MM $(CFLAGS) $< \
#	| sed 's/\($*\)\.o[ :]*/build\/\1.o $@ : /g' > $@; \
#	[ -s $@ ] || rm -f $@

#build/*:	build

clean:
	@echo	"CLEAN	$(DIR)"
	rm -rf build *.d *.ll.h *.yy.output *.yy.hpp

#-include $(addprefix build/,$(SOURCES:.c=.d))

#ifneq	($(TARGET),clean)
#endif
#-include $(addprefix build/,$(SOURCES:.c=.d))
#-include build/track/GPS.d
-include $(SOURCES:%.cpp=$(DEPDIR)/%.d)
-include $(M4_SOURCES:%.m4.h=$(DEPDIR)/%.m4.d)
