
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

      int polygon_mode;
      int visible;

      int shouldTexture;

      texture2d *tex;

      vec4 color; //R,G,B,Alpha
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

class HeightmapMesh : public Mesh{
public:
  HeightmapMesh();
  void init(int w, int h, vec3 scale, float texscalex, float texscaley);
  int loadDefaultGenerator();
  int loadFile(std::string filename);
  void loadFileOBJ(char *file);
  void setGenerator(std::function<float(float,float)> in);

  inline int getWidth() { return this->width; }
  inline int getHeight() { return this->height; } 

private:
  std::function<float(float,float)> generator;
  int width, height;
};

class TransformedMesh{
public:
  class MeshInfo{
  public:
    MeshInfo(Mesh *m, mat4 transform);
    Mesh *mesh;
    mat4 transform;
    // additional rendering information
    //  ....
    ///
  };
public:
  TransformedMesh(MeshInfo mesh);
  std::vector<MeshInfo> meshes;


};

#endif // !_MESH_HXX_


// a Mesh is a low-level aggregate of polygons.
// a TransformedMesh is a Mesh coupled with transformation/rendering information (eg. mvp, back-face culling, shader effects)
// a Model is a collection of TransformedMeshes