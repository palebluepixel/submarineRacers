TARGET = game

SHELL = /bin/sh

BUILD_DIR = $(shell pwd)
SRC_DIR = $(BUILD_DIR)/../src

SDL = $(BUILD_DIR)/../lib/SDL2-2.0.5
GLEW = $(BUILD_DIR)/../lib/glew-2.0.0
GLFW = $(BUILD_DIR)/../lib/glfw
SDL_CONFIG = 

CXX = g++ -std=c++11
CXXFLAGS = -g -Wall
CPPFLAGS = -I/usr/local/include

SRCS = $(wildcard $(SRC_DIR)/*/*.cxx)
INCLUDES = $(wildcard $(SRC_DIR)/*/*.hxx)
OBJS = $(notdir $(SRCS:.cxx=.o))

RPATH = -Wl,-rpath,$(SDL)/build -Wl,-rpath,$(GLEW)/lib -Wl,-rpath,$(GLFW)/src
LIBS = -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
LDFLAGS = -L$(BUILD_DIR)/../lib -L$(GLEW)/lib -L$(GLFW)/src
IPATH = -I../src -I../lib/glm -I$(GLEW)/include -I$(GLFW)/include -I$(BUILD_DIR)/../lib

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