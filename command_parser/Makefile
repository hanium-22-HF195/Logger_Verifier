#
# falinux 라이블러리를 사용하는 어플용 Makefile
#

CROSS_PREFIX		= 

CP_DIR			= ./

## 사용자 소스
TARGET			= command_parser
SRCS			= ../DB/bout_database.cpp command_parser.cpp 
#---------------------------------------------------------------------
## 소스레벨의 라이브러리

## 인클루드 디렉토리
DIR_INC			=

INCLUDEDIRS		+= -I.
LIVDIRS			+= 

OBJS			= $(SRCS:.cpp=.o)
DB_SRC = ../DB/bout_database.cpp

CFLAGS			= $(INCLUDEDIRS) -g -o -W -Wall -I/usr/include/mysql -L/usr/lib64/mysql 
LDFLAGS			= $(LIVDIRS)
LIBS			= -lpthread -lmysqlclient -ljsoncpp

#---------------------------------------------------------------------
CC			= $(CROSS_PREFIX)-gcc
CXX			= $(CROSS_PREFIX)-g++
AR			= $(CROSS_PREFIX)-ar rc
AR2			= $(CROSS_PREFIX)-ranlib
RANLIB		= $(CROSS_PREFIX)-ranlib
LD			= $(CROSS_PREFIX)-ld
NM			= $(CROSS_PREFIX)-nm
STRIP		= $(CROSS_PREFIX)-strip
OBJCOPY		= $(CROSS_PREFIX)-objcopy
CP	= cp
MV	= mv
#--------------------------------------------------------------------

all : $(TARGET)

obj : $(OBJS)

$(TARGET) : $(OBJS) $(DB_SRC)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)
	$(NM) $(TARGET) > $(TARGET).map

%.o:%.c
	@echo "Compiling $< ..."
	$(CC) -c $(CFLAGS) -o $@ $<

%.o:%.cpp
	@echo "C++ compiling $< ..."
	$(CXX) -c $(CFLAGS) -o $@ $<

dep :
	$(CXX)	-M	$(INCLUDEDIRS)	$(SRCS) > .depend

clean:
	rm -f *.bak
	rm -f *.map
	rm -f *.o
	rm -f $(DIR_COMMON)*.o
	rm -f $(DIR_FANET)*.o
	rm -f $(OBJS)
	rm -f $(TARGET) core


distclean: clean
	rm -rf .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif
