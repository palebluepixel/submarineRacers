
#ifndef _MESH_HXX_
#define _MESH_HXX_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

//! the information needed to render a mesh
struct Mesh {


    GLuint vaoId;
    GLuint vbufId;
    GLuint ibufId;
    GLenum prim;
    int nIndicies;

    vec4 color; //R,G,B,Alpha

    Mesh (GLenum p);

  //! initialize the vertex data buffers for the mesh
    void LoadVertices (int nVerts, const vec3 *verts);

  //! initialize the element array for the mesh
    void LoadIndices (int n, const uint32_t *indices);

  //! draw the mesh using a glDrawElements call
    void Draw ();

};

#endif // !_MESH_HXX_
