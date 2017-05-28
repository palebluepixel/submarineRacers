
#ifndef _MESH_HXX_
#define _MESH_HXX_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <graphics/texture.hxx>

using namespace glm;

//! the information needed to render a mesh
class Mesh {
public:
    struct {
      GLuint vaoId;
      GLuint vbufId; //vertices
      GLuint ibufId; //indices
      GLuint nbufId; //normals
      GLuint tbufId; //texcoords
      GLenum prim;
      int nIndicies;
      int nTexCoords;

      Mesh *owner;

      int shouldTexture;

      texture2d *tex;
      
    }data;

    Mesh (GLenum p);

    /**
     * used to save space in the graphics buffer. construct
     * a new mesh that uses the same data as copyfrom, but
     * which has different auxiliary fields, eg. visible, ...
     */
    Mesh (const Mesh &copyfrom);

  //! initialize the vertex data buffers for the mesh
    void loadVertices (int nVerts, const vec3 *verts);

  //! initialize the element array for the mesh
    void loadIndices (int n, const uint32_t *indices);

  //! initialize the texture-coordinate data buffer for the mesh
    void LoadTexCoords (int nCoords, vec2 *tcoords);

  //! initalize the vertex array for the normals
    void loadNormals (int nVerts, vec3 *norms);

  //! draw the mesh using a glDrawElements call
    void draw ();

    void loadOBJ(char *file);

};
struct HeightmapData{
  int width;
  int height;
  vec3 *verts;
  vec3 *norms;
  float *values;
  unsigned int *indices;
};
class HeightmapMesh : public Mesh{
public:
  HeightmapMesh();
  void init(int w, int h, vec2 texscale);
  int loadDefaultGenerator();
  int loadFile(std::string filename);
  void loadFileOBJ(char *file);
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



class Model{
public:

  class RenderState{
  public:
    // RenderState();
    RenderState(bool visible, vec4 color, int polymode, int cullmode, int linewidth);
    RenderState(vec4 color);
    RenderState();
    bool visible;
    int polymode;
    int cullmode;
    int linewidth;
    vec4 color;
  };

  class FancyMesh{
  public:
    FancyMesh(Mesh *m, mat4 transform, RenderState state);
    Mesh *mesh;             // mesh to be rendered
    mat4 transform;         // how do we transform the mesh?
    RenderState glState;    // how do we render the mesh?
  };
public:
  Model(FancyMesh mesh);
  std::vector<FancyMesh> meshes;

  /* Change the color of the RenderState of the ith mesh*/
  void setMeshColor(int i, vec4 newCol);


};

#endif // !_MESH_HXX_


// a Mesh is a low-level aggregate of polygons.
// a Model is a Mesh coupled with transformation/rendering information (eg. mvp, back-face culling, shader effects)
// a Model is a collection of Modeles