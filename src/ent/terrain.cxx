#include "terrain.hxx"

Terrain::Terrain(vec3 initial_position, quaternion initial_orientation, int id, std::string name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color)
: Entity(initial_position, initial_orientation, name, type, status, tick_interval){
    this->color = color;

    this->initalizeVisualData();
}

Terrain::~Terrain() { }

void Terrain::initalizeVisualData(){
    this->initalizeTextures("../assets/textures/moss1.png");
    this->initalizeMeshes();
}

//call this before initalizing meshes
void Terrain::initalizeTextures(const char* texfile){
    //load texture
    image2d * image = new image2d(texfile, true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}

void Terrain::initalizeMeshes(){
    HeightmapMesh *mesh = new HeightmapMesh();
    mesh->init(32,32, 0.5f, 0.5f);
    // mesh->loadOBJ(modelfile);
    mesh->data.color = vec4(this->color,0.5);
    mesh->data.tex = this->tex;
    TransformedMesh tm(mesh);
    tm.transform = glm::scale(glm::mat4(1),vec3(10,1,10));
    meshes.push_back(tm);
}
