
#		@echo	"***	BUILD COMPLETE ***"
#		@echo	"CFLAGS=	$(CFLAGS)"
#		@echo	"LDFLAGS=	$(LDLAGS)"
#		@echo	"***	BUILD COMPLETE ***"

#	echo	CC	$*
#		@$(CC)	-C -E$*.cce $*.c -O$*.obj $(CFLAGS)

SUBDIRS		=	port util src #xl misc

NW_SRC		=	$(wildcard src/*.cpp port/*.cpp util/*.cpp xl/*.cpp misc/*.cpp )
NW_OBJ		=	$(addprefix build/,$(addsuffix .o,$(basename $(NW_SRC))))

SOURCES		=	$(NW_SRC)
M4_SOURCES	=	$(wildcard system/*.m4.h app/*.m4.h util/*.m4.h extra/*.m4.h system/*.m4.h)

INCLUDES		=	$(addprefix -I,$(SUBDIRS)) -I.

WFLAGS		=	-Werror-implicit-function-declaration \
				-Wimplicit -Wimplicit-int -Wall -W -Wshadow \
				-Wpointer-arith \
				-Wcast-qual -Wcast-align	\
				-Wstrict-aliasing

#CFLAGS		=	-std=gnu99 -g -DWITH_TESTS -DFFC_EMU  -ggdb -O0  $(WFLAGS) $(INCLUDES) \
CFLAGS		=	-std=gnu99 -g -DWITH_TESTS -DFFC_EMU  -fomit-frame-pointer -msse3 -O3  $(WFLAGS) $(INCLUDES) \
				`sdl-config --cflags`

#CXXFLAGS		=	-g -ggdb -O0 $(INCLUDES) -DIO_EMULATOR `Magick++-config --cppflags --cxxflags` 		`sdl-config --cflags`	
CXXFLAGS		=	-fomit-frame-pointer -msse2 -O3 $(INCLUDES)
#`Magick++-config --cppflags --cxxflags` 		`sdl-config --cflags`	 


HEADERS		=	$(wildcard *.h)

LDFLAGS		=	-lm -lstdc++ -fomit-frame-pointer -msse2 -O3 
#-lm -lpthread -lstdc++  -lGL -lGLU `sdl-config --libs` -lglpng -lplibfnt -lplibul
				#-lsqlite3 `Magick++-config --ldflags --libs`
DEPCC		=	cc

.SILENT:	
.PHONY:	all	clean	hex

all:	build	build/version.h frp

frp:		$(NW_OBJ)
	@echo	"LD	$@"
	$(CC) -o $@  $^ $(LDFLAGS)

.deps/system/globals.d:		build/version.h

build/version.h:	build	$(SOURCES)
	@echo	"VER	$@"
	svnversion	|	awk '{print "const char*bin_version=\"r"$$1"\";"}'	>$@
	date '+const char*build_date="%y%m%d_%H%M%S";'	>>$@

depclean:
	echo	"DEPCLEAN"
	rm -rf .deps build

include	Makefile.inc

valg:
	echo	"MKDIR	valg"
	mkdir valg

valgrind:	nw
	echo	"VALGRIND"
	NOTERM=yes valgrind -v --log-file=valg/ffc_emu --gen-suppressions=all	\
		--suppressions=valgrind.suppress	\
		./$< >>/dev/null
	echo	"LN	_last_valgrind"
	ln -fs valg/$$(ls -tr valg|tail -n1) _last_valgrind


transfer:
	tar czf /tmp/_frp.tgz .
	scp /tmp/_frp.tgz wiki:/tmp/
	ssh wiki -- 'mkdir -p frp && cd frp && tar zxf /tmp/_frp.tgz && make clean && make'
