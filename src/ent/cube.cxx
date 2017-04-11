#include "cube.hxx"

Cube::Cube(vec2 initial_position, mat3 initial_orientation, int id, char*name, 
    EntityType type, EntityStatus status, float tick_interval)
: Entity(initial_position, initial_orientation, id, name, type, status, tick_interval)
{
    this->nMeshes = 6;
    this->meshes = NULL;

    this->initalizeMeshes();
}

Cube::~Cube()
{ }


void Cube::initalizeMeshes()
{
    meshes = new Mesh*[this->nMeshes];

    int i;
    Wall curwall;
    for(i=0; i<this->nMeshes; i++){
        meshes[i] = new Mesh(GL_TRIANGLE_FAN);
        curwall = wallsguys[i];

        meshes[i]->color = curwall.color;

        //create an array of size nverts with the same normal
        vec3 normvec[4] = {curwall.norm, curwall.norm, curwall.norm, curwall.norm};
        meshes[i]->loadNormals(4, normvec);

        //meshes[i]->LoadTexCoords(4, curwall.texCoords);

        meshes[i]->loadVertices(4, curwall.corners);

        const uint32_t indices[4] = {3, 2, 1, 0};
        meshes[i]->loadIndices(4, indices);

        //meshes[i]->texture = texture;
    }
}