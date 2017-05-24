#ifndef _WATER_HXX_
#define _WATER_HXX_

#include <ent/Entity.hxx>
#include <graphics/mesh.hxx>

using namespace glm;

class WaterMesh : public Mesh{
public:
  WaterMesh();
  void init(int w, int h, vec2 texscale);
  int loadDefaultGenerator();
  int loadFile(std::string filename);
  // void loadFileOBJ(char *file);
  void setGenerator(std::function<float(float,float)> in);

  HeightmapData getHmpData();

  float* getValues();
  inline int getWidth() { return this->width; }
  inline int getHeight() { return this->height; }

private:
  std::function<float(float,float)> generator;
  int width, height;
  float* values;
  HeightmapData hmpdata;
};


class Water : public Entity {

public:

    Water(int ID,vec3 initial_position, quaternion initial_orientation, std::string name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color, const char* texfile,
    const char* hmpfile);
    ~Water();

    void initalizeVisualData();
    virtual int onTick(float dt);

protected:

  std::string hmpfile;
  vec3 color;

  void initalizeTextures(const char* texfile);
  void initalizeMeshes();
};




#endif //_WATER_HXX_