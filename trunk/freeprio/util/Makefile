
LIB_SRCS=		log.cpp
LIB_OBJS=		$(LIB_SRCS:%.cpp=build/%.o)

CFLAGS=	-I.. -ggdb -O0  #-Wall -W -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align

LISTTEST_SRCS=	list_test.cpp $(LIB_SRCS)
LISTTEST_OBJS=	$(LISTTEST_SRCS:%.cpp=build/%.o)
LISTTEST_LD=	m

RINGTEST_SRCS=	ring_test.cpp $(LIB_SRCS)
RINGTEST_OBJS=	$(RINGTEST_SRCS:%.cpp=build/%.o)
RINGTEST_LD=	m	stdc++

SOURCES=	$(LIB_SRCS)
#$(LISTTEST_SRCS) $(RINGTEST_SRCS)

lib:	build	$(LIB_OBJS)
	@echo	"AR	util"
	@ar cr ../build/libutil.a	$(LIB_OBJS)

ring_test:	$(RINGTEST_OBJS)
	@echo     "LD  $@"
	gcc $(LFLAGS) -o $@ $(RINGTEST_OBJS) $(RINGTEST_LD:%=-l%)

list_test:	$(LISTTEST_OBJS)
	@echo     "LD  $@"
	gcc $(LFLAGS) -o $@ $(LISTTEST_OBJS) $(LISTTEST_LD:%=-l%)

include ../Makefile.inc
