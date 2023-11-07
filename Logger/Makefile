#--compiler----
CXX = g++

#--Flages----

SIGN_VER_FLAGS = -lssl -lcrypto -L /usr/local/opt/openssl/lib

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV) -g -lmysqlclient -lpthread -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -ljsoncpp
#--sources----
SOURCES :=  ../command_parser/command_parser.cpp Logger.cpp

#--Object files----
OBJS = $(SOURCES:.cpp=.o)

all: Logger

Logger: $(OBJS)
	$(CXX)  $(OBJS) -o $@ $(LIBS) $(SIGN_VER_FLAGS) -w

%.o: %.cpp
	${CXX} -c -o $@  $< $(LIBS) -w

clean:
	cd ../command_parser; make clean
	$(RM) *.o  Logger
	clear
