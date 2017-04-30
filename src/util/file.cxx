#include "file.hxx"

#include <fstream>
#include <streambuf>
#include <sstream>
#include <iostream>

char* fileio::load_file(const char* path){
	std::ifstream t;
	int length;
	t.open(path);      // open input file
	t.seekg(0, std::ios::end);    // go to the end
	length = t.tellg();           // report location (this is the length)
	t.seekg(0, std::ios::beg);    // go back to the beginning
	char *buffer = (char*)malloc(sizeof(char)*(length+1));    // allocate memory for a buffer of appropriate dimension
	t.read(buffer, length);       // read the whole file into the buffer
	buffer[length] = 0;
	t.close();                    // close file handle
	return buffer;
}
