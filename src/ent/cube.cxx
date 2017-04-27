#include "cube.hxx"

Cube::Cube(vec3 initial_position, quaternion initial_orientation, char*name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color)
: Entity(initial_position, initial_orientation, name, type, status, tick_interval)
{
    this->color = color;

    this->initalizeVisualData();
}

Cube::~Cube()
{ }

void Cube::initalizeVisualData()
{
    this->initalizeTextures("../assets/textures/cubetex.png");
    this->initalizeMeshes();
}

//call this before initalizing meshes
void Cube::initalizeTextures(const char* texfile)
{
    //load texture
    image2d * image = new image2d(texfile, true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}

void Cube::initalizeMeshes()
{
    int i;
    Wall curwall;
    for(i=0; i<7; i++){
        Mesh *mesh = new Mesh(GL_TRIANGLE_FAN);
        curwall = wallsguys[i];

        mesh->data.color = vec4(this->color,1.0);

        //create an array of size nverts with the same normal
        vec3 normvec[4] = {curwall.norm, curwall.norm, curwall.norm, curwall.norm};
        mesh->loadNormals(4, normvec);

        mesh->LoadTexCoords(4, curwall.texCoords);

        mesh->loadVertices(4, curwall.corners);

        const uint32_t indices[4] = {3, 2, 1, 0};
        mesh->loadIndices(4, indices);

        mesh->data.tex = this->tex;

        meshes.push_back(mesh);
    }
}