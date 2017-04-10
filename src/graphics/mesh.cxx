#include "mesh.hxx"

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
const GLint CoordAttrLoc = 0;   //!< location of vertex coordinates attribute
const GLint NormAttrLoc = 1;

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
    glBindVertexArray(this->vaoId);
    glGenBuffers(1, &this->vbufId);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbufId);
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

//! initalize the vertex array for the normals
void Mesh::LoadNormals (int nVerts, vec3 *norms){
  glBindVertexArray (this->vaoId);
  glGenBuffers (1, &this->nbufId);
  glBindBuffer (GL_ARRAY_BUFFER, this->nbufId);
  glBufferData (GL_ARRAY_BUFFER, nVerts*sizeof(vec3), norms, GL_STREAM_DRAW);
  glNormalPointer(GL_FLOAT, sizeof(float)*3, 0);//(const GLvoid *)(sizeof(float) * 3));
  glEnableVertexAttribArray(NormAttrLoc);
}

void Mesh::Draw (bool forceWireframe)
{
    glBindVertexArray(this->vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbufId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibufId);

    if (! forceWireframe)
        glDrawElements (this->prim, this->nIndicies, GL_UNSIGNED_INT, 0);
    else
        glDrawElements(GL_LINES, this->nIndicies, GL_UNSIGNED_INT, 0);

}