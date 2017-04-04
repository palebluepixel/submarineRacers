TARGET = game

SHELL = /bin/sh

BUILD_DIR = $(shell pwd)
SRC_DIR = $(BUILD_DIR)/../src

CXX = g++ -std=c++11
CXXFLAGS = -g -Wall
CPPFLAGS = -I/usr/local/include

SRCS = $(wildcard $(SRC_DIR)/*/*.cxx)
INCLUDES = $(wildcard $(SRC_DIR)/*/*.hxx)
OBJS = $(notdir $(SRCS:.cxx=.o))

LIBS = -lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS = -L$(BUILD_DIR)/../lib

$(TARGET): $(OBJS) .depend
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(TARGET) $(LDFLAGS) $(LIBS)

world.o:
	$(CXX) -c $(LIBS) $(CPPFLAGS) $(CXXFLAGS) $(SRC_DIR)/world/world.cxx $(SRC_DIR)/world/world.hxx

main.o:
	$(CXX) -c $(LIBS) $(CPPFLAGS) $(CXXFLAGS) $(SRC_DIR)/world/main.cxx


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