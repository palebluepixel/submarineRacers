TARGET = game

SHELL = /bin/sh

BUILD_DIR = $(shell pwd)
SRC_DIR = $(BUILD_DIR)/../src

SDL = $(BUILD_DIR)/../lib/SDL2-2.0.5
GLEW = $(BUILD_DIR)/../lib/glew-2.0.0
SDL_CONFIG = 

CXX = g++ -std=c++11
CXXFLAGS = -g -Wall
CPPFLAGS = -I/usr/local/include

SRCS = $(wildcard $(SRC_DIR)/*/*.cxx)
INCLUDES = $(wildcard $(SRC_DIR)/*/*.hxx)
OBJS = $(notdir $(SRCS:.cxx=.o))

RPATH = -Wl,-rpath,$(SDL)/build -Wl,-rpath,$(GLEW)/lib
LIBS = -lSDL2 -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -pthread -lSDL2 -ldl -lsndio -lGL -lGLU -lGLEW
LDFLAGS = -L$(BUILD_DIR)/../lib -L$(SDL)/build -L$(GLEW)/lib
IPATH = -I../src -I../lib/glm -I$(SDL)/include -I$(GLEW)/include

$(TARGET): $(OBJS) .depend
	$(CXX) $(OBJS) $(CXXFLAGS) $(IPATH) -o $(TARGET) $(LDFLAGS) $(RPATH) $(LIBS)

# world.o:
# 	$(CXX) -c $(LIBS) $(CPPFLAGS) $(CXXFLAGS) $(SRC_DIR)/world/world.cxx $(SRC_DIR)/world/world.hxx

# main.o:
# 	$(CXX) -c $(LIBS) $(CPPFLAGS) $(CXXFLAGS) $(SRC_DIR)/world/main.cxx

%.o :
	$(CXX) $(IPATH) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@


# include-file dependency information
#
.depend:	$(SRCS) $(INCLUDES)
	- $(CXX) -MM $(SRCS) > .depend

ifneq ($(MAKECMDGOALS),clean)
sinclude .depend
endif

# cleanup by removing generated files
#
.PHONY:		clean
clean:
		rm -rf *.o $(TARGET)

print-%: 
	@echo $* = $($*)