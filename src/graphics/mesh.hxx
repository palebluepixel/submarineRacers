
#ifndef _MESH_HXX_
#define _MESH_HXX_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdio.h>
#include <iostream>

using namespace glm;

//! the information needed to render a mesh
struct Mesh {


    GLuint vaoId;
    GLuint vbufId; //vertices
    GLuint ibufId; //indices
    GLuint nbufId; //normals
    GLenum prim;
    int nIndicies;

    vec4 color; //R,G,B,Alpha

    Mesh (GLenum p);

  //! initialize the vertex data buffers for the mesh
    void loadVertices (int nVerts, const vec3 *verts);

  //! initialize the element array for the mesh
    void loadIndices (int n, const uint32_t *indices);

  //! initalize the vertex array for the normals
    void loadNormals (int nVerts, vec3 *norms);

  //! draw the mesh using a glDrawElements call
    void draw ();

};

#endif // !_MESH_HXX_
