#include "cube.hxx"

Cube::Cube()
{
    int i;
    Wall curwall;
    for(i=0; i<6; i++){
        meshes[i] = new Mesh(GL_TRIANGLE_FAN);
        curwall = wallsguys[i];

        meshes[i]->color = curwall.color;

        //create an array of size nverts with the same normal
        vec3 normvec[4] = {curwall.norm, curwall.norm, curwall.norm, curwall.norm};
        meshes[i]->LoadNormals(4, normvec);

        //meshes[i]->LoadTexCoords(4, curwall.texCoords);

        meshes[i]->LoadVertices(4, curwall.corners);

        const uint32_t indices[4] = {3, 2, 1, 0};
        meshes[i]->LoadIndices(4, indices);

        //meshes[i]->texture = texture;
    }
}

Cube::~Cube()
{ }