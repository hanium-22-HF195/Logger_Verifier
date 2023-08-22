#--compiler----
CXX = g++

#--Flages----

SIGN_VER_FLAGS = -lssl -lcrypto -L /usr/local/opt/openssl/lib

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV) -lpthread -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -ljsoncpp
#--sources----
SOURCES :=   Logger.cpp

#--Object files----
OBJS = $(SOURCES:.cpp=.o)

all: Logger

Logger: $(OBJS)
	cd ../command_parser; make obj; cp command_parser.o ../Logger/command_parser.o
	$(CXX)  $(OBJS) command_parser.o -o $@ $(LIBS) $(SIGN_VER_FLAGS)

Logger.o: ./Logger.cpp
	${CXX} -c -o $@  $< $(LIBS) 

clean:
	cd ../command_parser; make clean
	$(RM) *.o  Logger
