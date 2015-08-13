
LIB_SRCS=		portability.cpp
LIB_OBJS=		$(LIB_SRCS:%.cpp=build/%.o)

CFLAGS=	-I.. -ggdb -O0  #-Wall -W -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align

lib:	build	$(LIB_OBJS)
	@echo	"AR	port"
	@ar cr ../build/libport.a	$(LIB_OBJS)


include ../Makefile.inc
