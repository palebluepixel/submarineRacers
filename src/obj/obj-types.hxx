#ifndef OBJ_READER_TYPES
#define OBJ_READER_TYPES

#include <glm/glm.hpp>

typedef glm::vec3 color3f;
typedef glm::vec3 vec3f;
typedef glm::vec2 color2f;
typedef glm::vec2 vec2f;

struct AABBf{
  void addPt(vec3f &p);
};

#endif