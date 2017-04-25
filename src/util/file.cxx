#include "file.hxx"
#include <ent/Entity.hxx>
#include <json.hpp>
#include <glm/glm.hpp>

#include <fstream>
#include <streambuf>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

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

glm::vec3 convert3vec(double *vec) {
    glm::vec3 v(vec[0], vec[1], vec[2]);
    return v;
}

Entity* fileio::load_Entity(char* raw) {
        auto j = json::parse(raw);
        double* vec = j["initial_position"];
        glm::vec3 initial_position;
        glm::mat3 initial_orientation;
        int id;
        char *name;
        EntityType type;
        EntityStatus status;
        float tick_interval;
        /*
         * do stuff
         */
        Entity e(initialPosition, initial_orientation, id, name, type, status, tick_interval);
        return &e;
}

/*

{
    "initial_pos": [1, 2, 3],

*/
