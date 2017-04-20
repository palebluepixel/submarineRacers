#include "gadget.hxx"

Gadget::Gadget(vec3 initial_position, quaternion initial_orientation, int id, char*name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color)
: Entity(initial_position, initial_orientation, id, name, type, status, tick_interval)
{
    this->nMeshes = 1;
    this->meshes = NULL;

    this->color = color;

    this->initalizeVisualData();
}

Gadget::~Gadget()
{ }

void Gadget::initalizeVisualData()
{
    this->initalizeTextures("../assets/textures/cubetex.png");
    this->initalizeMeshes();
}

//call this before initalizing meshes
void Gadget::initalizeTextures(const char* texfile)
{
    //load texture
    image2d * image = new image2d(texfile, true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}

void Gadget::initalizeMeshes(){

    vec3 v[4] = {vec3(-1,0,-1),vec3(1,0,-1),vec3(0,0,1),vec3(0,4,0)};
    vec2 t[4] = {vec2(0,1),vec2(1,1),vec2(1,0),vec2(0,0)};
    vec3 n[4] = {vec3(0,0,1),vec3(0,0,1),vec3(0,0,1)};

    vec3 verts[12] = {v[0],v[3],v[1],
                      v[1],v[3],v[2],
                      v[2],v[3],v[1],
                      v[0],v[1],v[2]};
    vec2 texcs[12] = {t[0],t[1],t[2],
                      t[0],t[1],t[2],
                      t[0],t[1],t[2],
                      t[0],t[1],t[2]};
    vec3 norms[12] = {n[0],n[1],n[2],
                      n[0],n[1],n[2],
                      n[0],n[1],n[2],
                      n[0],n[1],n[2]};

    meshes = new Mesh*[this->nMeshes];
    meshes[0] = new Mesh(GL_TRIANGLES);

    meshes[0]->loadOBJ("../assets/models/monkey.obj");

    meshes[0]->color = vec4(this->color,1.0);

    //create an array of size nverts with the same normal
    // vec3 normvec[4] = {curwall.norm, curwall.norm, curwall.norm, curwall.norm};
    // meshes[0]->loadNormals(12, norms);

    // meshes[0]->LoadTexCoords(12, texcs);

    // meshes[0]->loadVertices(12, verts);

    // const uint32_t indices[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
    // meshes[0]->loadIndices(12, indices);

    meshes[0]->tex = this->tex;
}