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
    mesh->loadFile(hmpfile);
    // mesh->loadDefaultGenerator();
    mesh->init(300,300, vec2(0.5f, 0.5f));
    mesh->data.tex = this->tex;

    Model::FancyMesh tmi(mesh,glm::scale(glm::mat4(1),vec3(100,16,100)), Model::RenderState(vec4(color,1.f)));
    Model tmesh(tmi);
    meshes.push_back(tmesh);


    // Mesh *monkey = new Mesh(GL_TRIANGLES);
    // monkey->loadOBJ("../assets/models/sphere.obj");
    // monkey->data.color = vec4(this->color,0.5);
    // monkey->data.tex = this->tex;
    // meshes.push_back(Model(Model::FancyMesh(monkey,mat4())));

    // pos(vec3(-50,-20,-50));
    volume = new HeightmapVolume(Volume::Pos(this), tmi.transform, mesh->getWidth(), mesh->getHeight(), mesh->getHmpData());
}
