#include "terrain.hxx"
#include <physics/Volume.hxx>

Terrain::Terrain(int ID, vec3 initial_position, quaternion initial_orientation, std::string name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color, const char* texfile, const char* hmpfile)
: Entity(ID,initial_position, initial_orientation, name, type, status, tick_interval){
    this->color = color;

    this->texfile = string(strdup(texfile));
    this->hmpfile = string(strdup(hmpfile));

    this->initalizeVisualData();
}

Terrain::~Terrain() { }

void Terrain::initalizeVisualData(){
    this->initalizeTextures(NULL);
    this->initalizeMeshes();
}

//call this before initalizing meshes
void Terrain::initalizeTextures(const char* texfile){
    //load texture
    image2d * image = new image2d(this->texfile.c_str(), true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}

void Terrain::initalizeMeshes(){
    HeightmapMesh *mesh = new HeightmapMesh();
    //mesh->loadFileOBJ("../assets/levels/bumps.obj");
    mesh->loadFile(this->hmpfile);
    mesh->init(mesh->getWidth(), mesh->getHeight(), vec3(1,10,1), 0.5f, 0.5f);
    mesh->data.color = vec4(this->color,0.5);
    mesh->data.tex = this->tex;

    TransformedMesh::MeshInfo tmi(mesh,glm::scale(glm::mat4(1),vec3(100,1,100)));
    TransformedMesh tmesh(tmi);
    meshes.push_back(tmesh);
    pos(vec3(0,-20,0));
    volume = new HeightmapVolume(Volume::Pos(this), vec3(10,1,10), 32, 32, 0); //This is incorrect now
}
