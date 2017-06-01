#include "ScreenQuad.hxx"

ScreenQuad::ScreenQuad(int ID, vec3 initial_position, quaternion initial_orientation, std::string name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein, char*texfile)
: Entity(ID, initial_position, initial_orientation, name, type, status, tick_interval){
    this->color = color;

    this->modelfile = strdup(modelfilein);
    this->texfile = strdup(texfile);
    this->initalizeVisualData();
}

ScreenQuad::~ScreenQuad()
{ }

void ScreenQuad::initalizeVisualData(){
    this->initalizeTextures(this->texfile);
    this->initalizeMeshes();
}

//call this before initalizing meshes
void ScreenQuad::initalizeTextures(const char* texfile){
    //load texture
    image2d * image = new image2d(texfile, true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}

void ScreenQuad::initalizeMeshes(){
    Mesh *mesh = new Mesh(GL_TRIANGLES);
    mesh->loadOBJ(modelfile);
    mesh->data.tex = this->tex;
    meshes.push_back(Model(Model::FancyMesh(mesh,mat4(),Model::RenderState(vec4(color,1.f)))));
}
