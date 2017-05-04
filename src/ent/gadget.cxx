#include "gadget.hxx"

Gadget::Gadget(int ID, vec3 initial_position, quaternion initial_orientation, std::string name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein)
: Entity(ID, initial_position, initial_orientation, name, type, status, tick_interval){
    this->color = color;

    this->modelfile = strdup(modelfilein);
    this->initalizeVisualData();
}

Gadget::~Gadget()
{ }

void Gadget::initalizeVisualData(){
    this->initalizeTextures("../assets/textures/wood1.png");
    this->initalizeMeshes();
}

//call this before initalizing meshes
void Gadget::initalizeTextures(const char* texfile){
    //load texture
    image2d * image = new image2d(texfile, true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}

void Gadget::initalizeMeshes(){
    Mesh *mesh = new Mesh(GL_TRIANGLES);
    mesh->loadOBJ(modelfile);
    mesh->data.color = vec4(this->color,0.5);
    mesh->data.tex = this->tex;
    meshes.push_back(TransformedMesh(TransformedMesh::MeshInfo(mesh,mat4())));
}
