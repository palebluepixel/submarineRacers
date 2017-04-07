#include "mesh.hxx"

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
const GLint CoordAttrLoc = 0;   //!< location of vertex coordinates attribute

//! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
//! loaded separately.
Mesh::Mesh(GLenum p)
: vbufId(0), ibufId(0), prim(p), nIndicies(0), color(0)
{
    glGenVertexArrays(1, &vaoId);

}

//! initialize the vertex data buffers for the mesh
void Mesh::LoadVertices (int nVerts, const vec3 *verts)
{
    glBindVertexArray(vaoId);
    glGenBuffers(1, &vbufId);
    glBindBuffer(GL_ARRAY_BUFFER, vbufId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*nVerts, verts, GL_STREAM_DRAW);
    glVertexAttribPointer(CoordAttrLoc, 3, GL_FLOAT, GL_FALSE, sizeof(verts[0]), (GLvoid *)0);
    glEnableVertexAttribArray(CoordAttrLoc);
}

//! initialize the element array for the mesh
void Mesh::LoadIndices (int n, const uint32_t *indices)
{
    this->nIndicies = n;
    glBindVertexArray (this->vaoId);
    glGenBuffers (1, &this->ibufId);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->ibufId);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, n*sizeof(uint32_t), indices, GL_STREAM_DRAW); //might be STATIC_DRAW
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(CoordAttrLoc);
}

void Mesh::Draw ()
{
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vbufId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibufId);

    glDrawElements (this->prim, this->nIndicies, GL_UNSIGNED_INT, 0);

}