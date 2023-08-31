CXX = g++

INCLUDEDIRS		+= -I. -I/usr/local/opt/openssl@3/include
LIVDIRS			+= -L /usr/local/opt/openssl@3/lib

CFLAGS			= $(INCLUDEDIRS) -g -o -W -Wall -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient 
LIBS 			= -ljsoncpp -lmysqlclient -lssl -lcrypto -L /usr/local/opt/openssl/lib -lpthread
LDFLAGS			= $(LIVDIRS)

TARGET   = Test
SOURCES := client_test.cpp

OBJS = $(SOURCES:.cpp=.o)

all : $(TARGET)

Client : $(SOURCES:.cpp=.o)

$(TARGET): $(OBJS)
	cd ../command_parser; make obj; cp command_parser.o ../Client/command_parser.o
	$(CXX) $(LDFLAGS) $(OBJS) command_parser.o -o $@ $(LIBS)

%.o:%.cpp
	@echo "C++ compiling $< ..."
	$(CXX) -nostartfiles -c $(CFLAGS) -o $@ $<

clean:
	cd ../command_parser; make clean
	rm -f *.bak
	rm -f *.map
	rm -f *.o
	rm -f $(DIR_COMMON)*.o
	rm -f $(DIR_FANET)*.o
	rm -f $(OBJS)
	rm -f $(TARGET) core
