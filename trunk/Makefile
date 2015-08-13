
#		@echo	"***	BUILD COMPLETE ***"
#		@echo	"CFLAGS=	$(CFLAGS)"
#		@echo	"LDFLAGS=	$(LDLAGS)"
#		@echo	"***	BUILD COMPLETE ***"

#	echo	CC	$*
#		@$(CC)	-C -E$*.cce $*.c -O$*.obj $(CFLAGS)

SUBDIRS		=	port util src xl misc

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

OPTFLAGS		=	-g -ggdb -O0
#OPTFLAGS		=	-O3 -msse3

CFLAGS		=	-std=gnu99 -g -DWITH_TESTS -DFFC_EMU  $(OPTFLAGS)  $(WFLAGS) $(INCLUDES) \
				`sdl-config --cflags`

CXXFLAGS		=	$(OPTFLAGS) $(INCLUDES) -DIO_EMULATOR `sdl-config --cflags` -Wall 

#CXXFLAGS	+=	-DMSG_LOG


HEADERS		=	$(wildcard *.h)

LDFLAGS		=	-lm -lpthread -lstdc++  -lGL -lGLU `sdl-config --libs` -lglpng -lplibfnt -lplibul $(CXXFLAGS)
				#-lsqlite3 `Magick++-config --ldflags --libs`
DEPCC		=	cc

.SILENT:	
.PHONY:	all	clean	hex

all:	geosteiner	build	build/version.h nw

nw:		$(NW_OBJ)
	@echo	"LD	$@"
	$(CC) -o $@  $^ $(LDFLAGS)


geosteiner:
	tar zxf dist/geosteiner-3.1.tar.gz
	tar zxf dist/geosteiner_patch.tgz
	ln -s geosteiner-3.1 geosteiner
	cd geosteiner && ./configure && make
	cd geosteiner && make bb_tx

.deps/system/globals.d:		build/version.h

build/version.h:	build	$(SOURCES)
	@echo	"VER	$@"
	svnversion	|	awk '{print "const char*bin_version=\"r"$$1"\";"}'	>$@
	date '+const char*build_date="%y%m%d_%H%M%S";'	>>$@

depclean:
	echo	"DEPCLEAN"
	rm -rf .deps build nw

include	Makefile.inc

valg:
	echo	"MKDIR	valg"
	mkdir valg

valgrind:	nw
	echo	"VALGRIND"
	NOTERM=yes valgrind -v --log-file=valg/$@.memcheck --gen-suppressions=all	\
		--suppressions=valgrind.suppress					\
		--num-callers=24							\
		--track-origins=yes							\
		--leak-check=full							\
		--show-reachable=yes							\
		--error-limit=no							\
		--malloc-fill=0xbd							\
		--free-fill=0xfe							\
		./$< 
		a
	echo	"LN	_last_memcheck"
	ln -fs valg/$$(ls -tr valg|tail -n1) _last_memcheck

callgrind:	nw
	echo	"CALLGRIND"
	NOTERM=yes valgrind --tool=callgrind --log-file=$@.callgrind -v --dump-every-bb=100000000 ./nw              
	ln -fs valg/$$(ls -tr valg|tail -n1) _last_callgrind
