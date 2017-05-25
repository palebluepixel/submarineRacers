#include "water.hxx"
#include <physics/Volume.hxx>
#include <util/file.hxx>

Water::Water(int ID, std::string name, vec3 initial_position, vec2 size, vec3 scale, vec3 color)
: Entity(ID,initial_position, quaternion(), name, WATER, SPAWNED, 0.1f){
    this->color = color;

    this->texfile = "../assets/textures/moss1.png";
    this->hmpfile = "../assets/heightmaps/bump_bump.hmp";

    this->size = size;
    this->scale = scale;
    this->initalizeVisualData();
}

Water::~Water() { }

void Water::initalizeVisualData(){
    this->initalizeTextures(NULL);
    this->initalizeMeshes();
}

//call this before initalizing meshes
void Water::initalizeTextures(const char* texfile){
    //load texture
    image2d * image = new image2d(this->texfile.c_str(), true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}

void Water::initalizeMeshes(){
    WaterMesh *mesh = new WaterMesh();
    //mesh->loadFileOBJ("../assets/levels/bumps.obj");
    mesh->loadFile(hmpfile);
    // mesh->loadDefaultGenerator();
    mesh->init(size.x,size.y, vec2(0.5f, 0.5f));
    mesh->data.tex = this->tex;

    Model::FancyMesh tmi(mesh,glm::scale(glm::mat4(1),scale), Model::RenderState(vec4(color,1.f)));
    Model tmesh(tmi);
    meshes.push_back(tmesh);


    // Mesh *monkey = new Mesh(GL_TRIANGLES);
    // monkey->loadOBJ("../assets/models/sphere.obj");
    // monkey->data.color = vec4(this->color,0.5);
    // monkey->data.tex = this->tex;
    // meshes.push_back(Model(Model::FancyMesh(monkey,mat4())));

    // pos(vec3(-50,-20,-50));
    // volume = new HeightmapVolume(Volume::Pos(this), tmi.transform, mesh->getWidth(), mesh->getHeight(), mesh->getHmpData());
}

int Water::onTick(float dt){

    return 0;
}

WaterMesh::WaterMesh() : Mesh(GL_QUADS){
}

/* Call this after setting the generator. Initalize the verte mesh representation of a heightmap
using the given information. 
  If we loaded the generator from a file, w and h should be the width
and height of the heightmap as specified in that file. If the generator is functional, they can
be any value such that {(x,z) | x in [0:w], z in [0:h]} is in the domain of the function. 
  Scale is a 3-vector representing the scaling for the position of each vertex. 
  the two texscale coords set the resolution for the texturing of the heightmap. 
*/
void WaterMesh::init(int w, int h, vec2 texscale){
  this->width = w;
  this->height = h;
  if(w<2)w=2;
  if(h<2)h=2;
  data.prim=GL_TRIANGLES;

  vec3 *verts = new vec3[w*h];
  vec2 *texcs = new vec2[w*h];
  vec3 *norms = new vec3[w*h];
  unsigned int *indices =new unsigned int[6*(w-1)*(h-1)];

  values = new float[w*h];

  float x_inc = 1.f/float(w);
  float z_inc = 1.f/float(h);

  float xp,zp;

  int ind=0;
  float stepsw[w];
  float stepsh[h];

  float fw = w-1;
  float fh = h-1;

  for(int z=0;z<h;++z){
    for(int x=0;x<w;++x){
      xp=x_inc*float(x);
      zp=z_inc*float(z);

      xp = 0.5+(2.f/(fw*fw))*fabsf(x-fw/2.f)*(x-fw/2.f);
      zp = 0.5+(2.f/(fh*fh))*fabsf(z-fh/2.f)*(z-fh/2.f);
      texcs[ind] = vec2(x_inc*float(x)/texscale.x, z_inc*float(z)/texscale.y);
      // verts[ind] = vec3(xp, sin(x*2.f/3.14f)*cos(z*2.f/3.14f) - xp*xp*30 - zp*zp*20, zp);
      verts[ind] = vec3(xp,0,zp);
      values[ind] = verts[ind].y;
      norms[ind] = vec3(0,1,0);
      ++ind;
    }
  }
  ind=0;
  for(int i=0;i<h-1;i++){
    for(int j=0;j<w-1;j++){
      indices[ind+5]=i*w+j;
      indices[ind+4]=i*w+(j+1);
      indices[ind+3]=(i+1)*w+(j+1);
      indices[ind+2]=i*w+j;
      indices[ind+1]=(i+1)*w+j;
      indices[ind+0]=(i+1)*w+(j+1);

      vec3 v1 = verts[indices[ind+0]];
      vec3 v2 = verts[indices[ind+1]];
      vec3 v3 = verts[indices[ind+3]];
      vec3 norm = normalize(cross(v2-v1,v3-v1));

      // fprintf(stderr,"norm %d: %.3f,%.3f,%.3f\n",indices[ind+0],norm.x,norm.y,norm.z);

      norms[indices[ind+0]] = norm;
      norms[indices[ind+1]] = norm;
      norms[indices[ind+2]] = norm;
      norms[indices[ind+3]] = norm;

      ind+=6;
    }
  }

  loadNormals(w*h,norms);
  LoadTexCoords(w*h,texcs);
  loadVertices(w*h,verts);
  loadIndices(6*(w-1)*(h-1),indices);

  hmpdata = HeightmapData{w,h,verts,norms,values,indices};

}

static std::function<float(float,float)> canyon_generator = [](float x, float z){
  // x=x-0.5f;
  // z=z-0.5f;
  // return 10.f*x*x-32.f*z*z*z*z;
  return 0;
  // return -x;
};

/* Load a functional generator which produces a canyon. */
int WaterMesh::loadDefaultGenerator()
{
  this->generator = canyon_generator;
  return 0;
}

/* Create a generator which for (x,z) returns the value stored at (x,z) in the file. */
int WaterMesh::loadFile(std::string filename){
  char *file = fileio::load_file(filename.c_str());
  float *data = 0;
  int data_ind=0;
  char *tok;
  char *tok2;
  char *delim = "\n\t ";
  int err =0;
  int w=-1, h=-1;

  tok = strtok(file,delim);
  while((tok = strtok(NULL,delim)) != 0){
    if(!strcmp(tok,"resl")){
      tok =strtok(NULL,delim);
      tok2=strtok(NULL,delim);
      if(!tok || !tok2){
        err=-1;
        break;
      }
      w = atoi(tok);
      h = atoi(tok2);
      data = new float[w*h];
    }
    else{
      if(!data){
        err=-2;
        break;
      }
      data[data_ind++] = atof(tok);
    }
  }
  if(w==-1 || h==-1){
    err=-3;
  }
  if(!err){
    generator = [data,w,h](float x, float z){
      float xind = x*w;
      float zind = z*h;
      if(xind<0)xind=0.01f;
      if(xind>=w-1.01f)xind=w-1.01f;
      if(zind<0)zind=0.01f;
      if(zind>=h-1.01f)zind=h-1.01f;

      float xoff = 1.f-(xind-(int)xind);
      float zoff = (zind-(int)zind);
      // fprintf(stderr,"off: %f,%f\n",xoff,zoff);

      int xlow = (int)xind;
      int zlow = (int)zind;

      // bilinearly interpolate
      float val1 = data[xlow + zlow*w]*xoff + data[xlow + zlow*w +1]*(1-xoff);
      float val2 = data[xlow + (zlow+1)*w]*xoff + data[xlow + (zlow+1)*w +1]*(1-xoff);

      float val = val1*zoff + val1*(1-zoff);

      // return xoff;
      // return data[xlow + zlow*w];
      return val;
      // return (val - data[xlow + zlow*w])*10.f;

    };
  }
  return 0;
}


float* WaterMesh::getValues(){
  return values;
}
HeightmapData WaterMesh::getHmpData(){
  return hmpdata;
}

void WaterMesh::setGenerator(std::function<float(float,float)> in){
  generator=in;
}