#ifndef _GADGET_HXX_
#define _GADGET_HXX_

#include <ent/Entity.hxx>

using namespace glm;

/* Simple example mesh */

#define RED_COLOR  vec4(1.0f, 0.0f, 0.0f, 1.0f)   /*!< Red color */ 
#define GREEN_COLOR vec4(0.0f, 1.0f, 0.0f, 1.0f)    /*!< Green color */
#define YELLOW_COLOR  vec4(1.0f, 1.0f, 0.0f, 1.0f)    /*!< Yellow color */
#define BLUE_COLOR vec4(0.0f, 0.0f, 1.0f, 1.0f)   /*!< Blue color */
#define SKY_BLUE_COLOR vec4(0.0f, 1.0f, 1.0f, 1.0f)   /*!< Sky Blue color */
#define PINK_COLOR vec4(1.0f, 0.0f, 0.5f, 1.0f)   /*!< Pink */ 
#define WHITE_COLOR vec4(1, 1, 1, 1)

#define INDICES_ARRAY const uint32_t[4] = {0, 1, 2, 3};

typedef struct GWall { 
  vec4 color; 
  vec3 norm;
  vec3 corners[4]; 
  vec2 texCoords[4]; 
} GWall; 



class Gadget : public Entity {

public:

    Gadget(vec3 initial_position, quaternion initial_orientation, int id, char*name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color);
    ~Gadget();

    void initalizeVisualData();

protected:

    vec3 color;

    void initalizeTextures(const char* texfile);
    void initalizeMeshes();
};



#endif //_CUBE_HXX_