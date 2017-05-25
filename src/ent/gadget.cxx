#include "gadget.hxx"


Gadget::Gadget(int ID, vec3 initial_position, quaternion initial_orientation, std::string name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein, char * texfilein)
: Entity(ID, initial_position, initial_orientation, name, type, status, tick_interval){

    this->color = color;
    this->modelfile = strdup(modelfilein);
    this->texfile = texfilein;

    image2d * image = new image2d(texfilein, true);
    texture2d * texture = new texture2d(GL_TEXTURE_2D, image);
    this->tex = texture;
    this->img = image;

    Mesh *mesh = new Mesh(GL_TRIANGLES);
    mesh->loadOBJ(modelfile);
    mesh->data.tex = this->tex;
    meshes.push_back(Model(Model::FancyMesh(mesh,mat4(),Model::RenderState(vec4(color,1.f)))));
}

Gadget::~Gadget()
{ }