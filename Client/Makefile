CXX = g++

INCLUDEDIRS		+= -I.
CFLAGS			= $(INCLUDEDIRS) -g -o -W -Wall -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient 
LIBS = -ljsoncpp -lmysqlclient

TARGET   = Test
SOURCES := client.cpp
TEST_SOURCES := client_test.cpp

OBJS = $(SOURCES:.cpp=.o)

all : $(TARGET)

Client : $(SOURCES:.cpp=.o)

$(TARGET): $(OBJS)
	cd ../command_parser; make obj; cp command_parser.o ../Client/command_parser.o
	$(CXX)  $(OBJS) command_parser.o -o $@ $(LIBS)

%.o:%.cpp
	@echo "C++ compiling $< ..."
	$(CXX) -c $(CFLAGS) -o $@ $<

clean:
	cd ../command_parser; make clean
	rm -f *.bak
	rm -f *.map
	rm -f *.o
	rm -f $(DIR_COMMON)*.o
	rm -f $(DIR_FANET)*.o
	rm -f $(OBJS)
	rm -f $(TARGET) core
