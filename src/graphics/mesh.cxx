#include "mesh.hxx"
#include <util/file.hxx>
#include <obj/obj.hxx>
#include <obj/obj-reader.hxx>
#include <stdio.h>
#include <stdlib.h>

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
const GLint CoordAttrLoc = 0;   //!< location of vertex coordinates attribute
const GLint NormAttrLoc = 1;
const GLint TexCoordAttrLoc = 2;

// build an empty Mesh.
Mesh::Mesh(GLenum p) {
  data.vbufId=0;
  data.ibufId=0;
  data.prim=p;
  data.nIndicies=0;
  data.color=vec4 (1,1,1,0.5f);
  data.shouldTexture = 1;

  data.polygon_mode=GL_FILL;
  data.visible=1;
  data.owner = this;

  data.tex = 0;
  data.vaoId = 0;


}



Mesh::Mesh(const Mesh &copyfrom){
  data = copyfrom.data;
}

//! initialize the vertex data buffers for the mesh
void Mesh::loadVertices (int nVerts, const vec3 *verts){
    if(!data.vaoId)glGenVertexArrays(1, &data.vaoId);
    glBindVertexArray(data.vaoId);
    glGenBuffers(1, &data.vbufId);
    glBindBuffer(GL_ARRAY_BUFFER, data.vbufId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*nVerts, verts, GL_STATIC_DRAW);
    glVertexAttribPointer(CoordAttrLoc, 3, GL_FLOAT, GL_FALSE, sizeof(verts[0]), (GLvoid *)0);
    glEnableVertexAttribArray(CoordAttrLoc);
}

//! initialize the element array for the mesh
void Mesh::loadIndices (int n, const uint32_t *indices){
    if(!data.vaoId)glGenVertexArrays(1, &data.vaoId);
    data.nIndicies = n; 
    glBindVertexArray (data.vaoId);
    glGenBuffers (1, &data.ibufId);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, data.ibufId);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, n*sizeof(uint32_t), indices, GL_STATIC_DRAW); //might be STATIC_DRAW
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(CoordAttrLoc);
}

void Mesh::LoadTexCoords (int nCoords, vec2 *tcoords){
  if(!data.vaoId)glGenVertexArrays(1, &data.vaoId);
  data.nTexCoords = nCoords;
  glBindVertexArray (data.vaoId);
  glGenBuffers (1, &data.tbufId);
  glBindBuffer (GL_ARRAY_BUFFER, data.tbufId);
  glBufferData (GL_ARRAY_BUFFER, nCoords*sizeof(vec2), tcoords, GL_STATIC_DRAW);
  glVertexAttribPointer(TexCoordAttrLoc, 2, GL_FLOAT, GL_FALSE, sizeof(tcoords[0]), (GLvoid *)0);
  glEnableVertexAttribArray(TexCoordAttrLoc);
}

//! initalize the vertex array for the normals
void Mesh::loadNormals (int nVerts, vec3 *norms){
  if(!data.vaoId)glGenVertexArrays(1, &data.vaoId);
  glBindVertexArray (data.vaoId);
  glGenBuffers (1, &data.nbufId);
  glBindBuffer (GL_ARRAY_BUFFER, data.nbufId);
  glBufferData (GL_ARRAY_BUFFER, nVerts*sizeof(vec3), norms, GL_STATIC_DRAW);
  glVertexAttribPointer(NormAttrLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), ((GLvoid*) 0));
  glEnableVertexAttribArray(NormAttrLoc);
}

void Mesh::draw (){
    if(!data.vaoId)glGenVertexArrays(1, &data.vaoId);
    glBindVertexArray(data.vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, data.vbufId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ibufId);

    glDrawElements (data.prim, data.nIndicies, GL_UNSIGNED_INT, 0);

}

void Mesh::loadOBJ(char *file){
  data.prim = GL_TRIANGLES;
  data.shouldTexture = false;  // change.
  //data.color = vec4(1.f,1.f,1.f,1.f);

  // todo: change code to use this form vvv . and make each
  //       group a single mesh.
  // Model loaded(std::string(file));
  //            ^^^

  OBJmodel *model = OBJReadOBJ (file);

  //printf("texture: %s\n",model->mtllibname);

  vec3 *verts = new vec3[model->numtriangles*3];
  vec3 *norms = new vec3[model->numtriangles*3];
  vec2 *texcs = new vec2[model->numtriangles*3];

  // indices. for our purposes, it is {0,1,2, 3,4,5, ... n*3};
  unsigned int *indices =new unsigned int[model->numtriangles*3];
  for(int i=0;i<model->numtriangles*3;++i){
    indices[i]=i;
  }

  OBJgroup *gp = model->groups;

  int ind=0;
  while(gp != 0){
    // struct Material mat = model->Material(gp->material);
    // printf("g texture: %s\n",mat.diffuseMap.c_str());
    for(int i=0;i<gp->numtriangles;++i){
      OBJtriangle &tri = model->triangles[gp->triangles[i]];
      verts[ind+0]=model->vertices[tri.vindices[0]];
      verts[ind+1]=model->vertices[tri.vindices[1]];
      verts[ind+2]=model->vertices[tri.vindices[2]];

      // printf("triangle with %d %d %d\n",tri.vindices[0],tri.vindices[1],tri.vindices[2]);
      // printf("triangle starting (%.3f,%.3f,%.3f)\n",verts[ind].x,verts[ind].y,verts[ind].z);

      norms[ind+0]=model->normals[tri.nindices[0]];
      norms[ind+1]=model->normals[tri.nindices[1]];
      norms[ind+2]=model->normals[tri.nindices[2]];

      if(model->numtexcoords > 0){
        texcs[ind+0]=model->texcoords[tri.tindices[0]];
        texcs[ind+1]=model->texcoords[tri.tindices[1]];
        texcs[ind+2]=model->texcoords[tri.tindices[2]];
      }

      ind+=3;
    }
    gp = gp->next;
  }

  loadNormals(model->numtriangles*3, norms);
  LoadTexCoords(model->numtriangles*3, texcs);
  loadVertices(model->numtriangles*3, verts);
  loadIndices(model->numtriangles*3, indices);
}

TransformedMesh::TransformedMesh(TransformedMesh::MeshInfo in){
  this->meshes.push_back(in);
}

TransformedMesh::MeshInfo::MeshInfo(Mesh *m, mat4 transform){
  mesh = m;
  this->transform = transform;
}

//////////////////////////////////////////*
//           Heightmap Volume
/////////////////////////////////////////*/

static std::function<float(float,float)> canyon_generator = [](float x, float z){
  x*=8;
  z*=8;
  return pow(x,4)+z*z*z;
};

HeightmapMesh::HeightmapMesh() : Mesh(GL_QUADS){

}
void HeightmapMesh::init(int w, int h, float texscalex, float texscaley){
  if(w<2)w=2;
  if(h<2)h=2;
  data.prim=GL_QUADS;

  vec3 *verts = new vec3[w*h];
  vec2 *texcs = new vec2[w*h];
  vec3 *norms = new vec3[w*h];
  unsigned int *indices =new unsigned int[4*(w-1)*(h-1)];

  float x_inc = 1.f/float(w);
  float z_inc = 1.f/float(h);

  float xp,zp;

  int ind=0;
  for(int z=0;z<h;++z){
    for(int x=0;x<w;++x){
      xp=-0.5f+x_inc*float(x);
      zp=-0.5f+z_inc*float(z);
      texcs[ind] = vec2(-0.0f+x_inc*float(x)/texscalex, -0.0f+z_inc*float(z)/texscaley);
      // verts[ind] = vec3(xp, sin(x*2.f/3.14f)*cos(z*2.f/3.14f) - xp*xp*30 - zp*zp*20, zp);
      verts[ind] = vec3(xp,canyon_generator(xp,zp),zp);
      norms[ind] = vec3(sin(xp),cos(xp),0);
      ++ind;
    }
  }
  ind=0;
  for(int i=0;i<h-1;i++){
    for(int j=0;j<w-1;j++){
      indices[ind+3]=i*w+j;
      indices[ind+2]=i*w+(j+1);
      indices[ind+1]=(i+1)*w+(j+1);
      indices[ind+0]=(i+1)*w+j;
      ind+=4;
    }
  }

  loadNormals(w*h,norms);
  LoadTexCoords(w*h,texcs);
  loadVertices(w*h,verts);
  loadIndices(4*(w-1)*(h-1),indices);
}

int HeightmapMesh::loadFile(std::string filename){
  char *file = fileio::load_file(filename.c_str());
  float *data = 0;
  int data_ind=0;
  char *tok;
  char *tok2;
  char *delim = "\n\t ";
  int err =0;
  int w=-1,h=-1;

  while((tok = strtok(file,delim)) != 0){
    if(!strcmp(tok,"resl")){
      tok =strtok(file,delim);
      tok2=strtok(file,delim);
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
      int xind = (x+0.5f)*w;
      int zind = (z+0.5f)*h;
      if(xind<0)xind=0;
      if(xind>=w)xind=w-1;
      if(zind<0)zind=0;
      if(zind>=h)zind=h-1;

      return data[xind + zind*w];

    };
  }
  return 0;
}

typedef struct{
  vec3 point;
  vec3 normal;
}Plane;

void HeightmapMesh::loadFileOBJ(char *file){
  data.prim = GL_QUADS;
  data.shouldTexture = false;  // change.
  //data.color = vec4(1.f,1.f,1.f,1.f);

  // todo: change code to use this form vvv . and make each
  //       group a single mesh.
  OBJ::Model *loaded = new OBJ::Model(std::string(file));
  // int k = loaded->NumGroups();
  for(int i=0;i<loaded->NumGroups();++i){
    OBJ::Group gp = loaded->Group(i);
    fprintf(stderr,"Parsing group %s\n",gp.name.c_str());
    int nv = gp.nVerts;
    vec3f *verts = gp.verts;

    // STEP 1 : PERFORM RANSAC to determine plane of best fit.
    Plane best_plane;
    float best_score;

    srand (time(NULL));
    for(int i=0;i<100;i++){     // 100 RANSAC iterations. tune this parameter.
      
      // find random plane.
      vec3f a = verts[rand() % nv];
      vec3f b = verts[rand() % nv];
      vec3f c = verts[rand() % nv];

      Plane p = {a,glm::normalize(glm::cross(a-b,a-c))};
      float score = 0.f;
      for(int j=0;j<100;++j){
        vec3f test = verts[rand() % nv];  // test point.
        float dist = glm::dot(p.normal,p.point-test);
        if(fabs(dist)<1.f)score += 1;
      }
      if(score > best_score){
        best_plane = p;
        best_score = score;
      }
    }

    // STEP 2 : Find local coordinate system

    vec3 x_axis = (glm::cross(best_plane.normal,vec3(1.1f,0.1f,-0.1f)));
    if(glm::length(x_axis) < 0.001f){
      x_axis = (glm::cross(best_plane.normal,vec3(0.1f,0.1f,-1.1f)));
    }
    x_axis = glm::normalize(x_axis);
    vec3 z_axis = glm::normalize(glm::cross(best_plane.normal,x_axis));

    // STEP 3: Transform to local coordinate system

    // STEP 4: Find boundary points of quad

    // STEP 5: 
  }



  free(loaded);
  //            ^^^

  // OBJmodel *model = OBJReadOBJ (file);

  // printf("texture: %s\n",model->mtllibname);

  // vec3 *verts = new vec3[model->numtriangles*3];
  // vec3 *norms = new vec3[model->numtriangles*3];
  // vec2 *texcs = new vec2[model->numtriangles*3];

  // // indices. for our purposes, it is {0,1,2, 3,4,5, ... n*3};
  // unsigned int *indices =new unsigned int[model->numtriangles*3];
  // for(int i=0;i<model->numtriangles*3;++i){
  //   indices[i]=i;
  // }

  // OBJgroup *gp = model->groups;

  // int ind=0;
  // while(gp != 0){
  //   // struct Material mat = model->Material(gp->material);
  //   // printf("g texture: %s\n",mat.diffuseMap.c_str());
  //   for(int i=0;i<gp->numtriangles;++i){
  //     OBJtriangle &tri = model->triangles[gp->triangles[i]];
  //     verts[ind+0]=model->vertices[tri.vindices[0]];
  //     verts[ind+1]=model->vertices[tri.vindices[1]];
  //     verts[ind+2]=model->vertices[tri.vindices[2]];

  //     // printf("triangle with %d %d %d\n",tri.vindices[0],tri.vindices[1],tri.vindices[2]);
  //     // printf("triangle starting (%.3f,%.3f,%.3f)\n",verts[ind].x,verts[ind].y,verts[ind].z);

  //     norms[ind+0]=model->normals[tri.nindices[0]];
  //     norms[ind+1]=model->normals[tri.nindices[1]];
  //     norms[ind+2]=model->normals[tri.nindices[2]];

  //     if(model->numtexcoords > 0){
  //       texcs[ind+0]=model->texcoords[tri.tindices[0]];
  //       texcs[ind+1]=model->texcoords[tri.tindices[1]];
  //       texcs[ind+2]=model->texcoords[tri.tindices[2]];
  //     }

  //     ind+=3;
  //   }
  //   gp = gp->next;
  // }

  // loadNormals(model->numtriangles*3, norms);
  // LoadTexCoords(model->numtriangles*3, texcs);
  // loadVertices(model->numtriangles*3, verts);
  // loadIndices(model->numtriangles*3, indices);
}

void HeightmapMesh::setGenerator(std::function<float(float,float)> in){
  generator=in;
}




//////////////////////////////////////////*
//               Model
/////////////////////////////////////////*/


// Model::Model() : parent(0){

// }
// Model::Model(std::string file) : parent(0){

// }
// Model::Model(Model *parent) : parent(0){

// }