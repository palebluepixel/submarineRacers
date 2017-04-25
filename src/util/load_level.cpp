#include "json.hpp"

#include <string>
#include <map>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

double *three_tuple(std::vector<double> inp) {
    double *res = (double*)malloc(3 * sizeof(double));
    *res = inp[0]; *(res+1) = inp[1]; *(res+2) = inp[2];
    return res;
}

typedef struct {
    char *name;
    double *location;
} entity, *ent;

ent make_ent(const char *name, std::vector<double> location) {
    ent a = (ent)malloc(sizeof(entity));
    a->name = strdup(name);
    a->location = three_tuple(location);
    return a;
}

typedef struct {
    std::map <int, ent> entities;
    std::vector<int> drawables;
    std::vector<int> collidables;
} level, *lev;

lev make_lev() {
    lev a = (lev)malloc(sizeof(level));
    std::map<int, ent> entities;
    a->entities = entities;
    return a;
}

char* load_file(const char* path){
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

int main(int argc, char* argv[]) {
    lev a = make_lev();
    char *raw = load_file("level1.json");
//    printf("%s\n", raw);
    json raw_j = json::parse(raw);
    std::string j_name = raw_j["name"];
    json entities = raw_j["entities"];
    json::iterator it;
    for (it = entities.begin(); it != entities.end(); ++it) {
        int index = std::stoi(it.key());
        json this_entity = it.value();
        std::string entity_name = this_entity["name"];
        const char *real_name = entity_name.c_str();
        std::vector<double> position = this_entity["position"];
        a->entities[index] = make_ent(real_name, position);
    }
    std::vector<int>::const_iterator int_it;
    std::vector<int> collidables = raw_j["collidables"];
    std::vector<int> drawables = raw_j["drawables"];
    a->collidables = collidables;
    a->drawables = drawables;
    std::cout << "collidables" << std::endl;
    for (int_it = a->collidables.begin(); int_it != a->collidables.end(); ++int_it) {
        std::cout << *int_it << std::endl;
        std::cout << a->entities[*int_it]->name << std::endl;
        std::cout << a->entities[*int_it]->location[0] << std::endl;
//        std::cout << a->entities[*int_it]->location << std::endl;
    }
    std::cout << "drawables" << std::endl;
    for (int_it = a->drawables.begin(); int_it != a->drawables.end(); ++int_it) {
        std::cout << *int_it << std::endl;
        std::cout << a->entities[*int_it]->name << std::endl;
    }
//    std::cout << j_name << std::endl;
//    for (json::iterator it = raw_j.begin(); it != raw_j.end(); ++it) {
//        std::cout << it.key() << " : " << it.value()  << std::endl;
//    }
//    printf("%s\n", raw_j[0]);
//    a->entities[0] = NULL;
//    std::string turtle = "turtle";
//    double f[3] = {0.0, 1.0, 2.0};
//    a->entities[1] = make_ent(&turtle, f);
//    return 0;
}
