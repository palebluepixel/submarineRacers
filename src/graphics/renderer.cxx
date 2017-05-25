#include "renderer.hxx"
#include <stdio.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

/***** virtual class Renderer member functions *****/

Renderer::Renderer (Shader *sh)
    : _shader(sh)
{ 
  projectionLoc = _shader->getUniformLocation("projection");
  modelViewLoc = _shader->getUniformLocation ("modelView");
  colorLoc = _shader->getUniformLocation("color");
  modelLoc = _shader->getUniformLocation("model");

}

Renderer::~Renderer ()
{ }

/* Prepare the shader for this renderer for use, and set openGL global
state information */
void Renderer::enable (){
  _shader->use();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable (GL_DEPTH_TEST);
}

void Renderer::render(){ }

/* Given an entity, render each of its meshes */
void Renderer::render(View *view, Entity *entity){
  mat4 modelMatrix = entity->modelMatrix();
  for (auto mesh : entity->getMeshes()){
    for (auto FancyMesh : mesh.meshes){
      if(FancyMesh.mesh != NULL){
        setUniform(modelLoc, modelMatrix * FancyMesh.transform);
        this->render(view, FancyMesh);
      }
    }
  }
}

/*==================== class FlatShadingRenderer member functions======================*/

FlatShadingRenderer::FlatShadingRenderer (Shader *sh)
    : Renderer (sh)
{ }

FlatShadingRenderer::~FlatShadingRenderer ()
{ }

void FlatShadingRenderer::enable()
{
  _shader->use();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(1);
  glDisable(GL_CULL_FACE);
}

void FlatShadingRenderer::render (View *view, Model::FancyMesh mesh) {

  mat4 projectionMat = view->projectionMatrix();
  mat4 viewMat = view->viewMatrix();

  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);

  setUniform(colorLoc, mesh.glState.color);

  mesh.mesh->draw();
}



/*==================== class SunlightShadingRenderer member functions======================*/

SunlightShadingRenderer::SunlightShadingRenderer (Shader *sh)
    : Renderer (sh) { 
  lightDirLoc = _shader->getUniformLocation("lightDir");
  lightIntenLoc = _shader->getUniformLocation("lightInten");
  lightAmbLoc = _shader->getUniformLocation("lightAmb");

  fogOnLoc = _shader->getUniformLocation("fogOn");
  fogColorLoc = _shader->getUniformLocation("fogColor");
  fogDensityLoc = _shader->getUniformLocation("fogDensity");
  fogStartLoc = _shader->getUniformLocation("fogStart");

  shouldTextureLoc = _shader->getUniformLocation("shouldTexture");
  texSamplerLoc = _shader->getUniformLocation("texSampler");
}

SunlightShadingRenderer::~SunlightShadingRenderer() { }

void SunlightShadingRenderer::render (View *view, Model::FancyMesh mesh){
  mat4 projectionMat = view->projectionMatrix();
  mat4 viewMat = view->viewMatrix();

  Sunlight sun = view->getSunlight();
  setUniform(lightDirLoc,   sun.lightDir);
  setUniform(lightIntenLoc, sun.lightInten);
  setUniform(lightAmbLoc,   sun.lightAmb);

  Fog fog = view->getFog();
  setUniform(fogOnLoc, fog.fogOn);
  setUniform(fogColorLoc, fog.fogColor);
  setUniform(fogDensityLoc, fog.fogDensity);
  setUniform(fogStartLoc, fog.fogStart);

  setUniform(shouldTextureLoc, mesh.mesh->data.shouldTexture);
  texture2d *tex = mesh.mesh->data.tex;
  if(tex){
    tex->Bind();
    tex->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    tex->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  }
  setUniform(texSamplerLoc, 0);

  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);
  setUniform(colorLoc, mesh.glState.color);
  mesh.mesh->draw();

}





/*==================== class UnderwaterRenderer member functions======================*/

UnderwaterRenderer::UnderwaterRenderer (Shader *sh)
    : SunlightShadingRenderer(sh)
{ 
  oceanColoringOnLoc = _shader->getUniformLocation("oceanColoringOn");
  oceanTopBrightnessLoc = _shader->getUniformLocation("oceanTopBrightness");
  oceanBottomBrightnessLoc = _shader->getUniformLocation("oceanBottomBrightness");
  oceanTopColorLoc = _shader->getUniformLocation("oceanTopColor");
  oceanBottomColorLoc = _shader->getUniformLocation("oceanBottomColor");
  oceanDensityLoc = _shader->getUniformLocation("oceanDensity");
  surfaceDepthLoc = _shader->getUniformLocation("surfaceDepth");
  floorDepthLoc = _shader->getUniformLocation("floorDepth");
  umode = _shader->getUniformLocation("mode");
}

UnderwaterRenderer::~UnderwaterRenderer()
{ }

void UnderwaterRenderer::modeSkybox(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  _mode=skybox;
  setUniform(umode, 2);
}
void UnderwaterRenderer::modeNormal(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  _mode=normal;
  setUniform(umode, 1);
}

void UnderwaterRenderer::setMode(Mode m){
  if(m == skybox)modeSkybox();
  if(m == normal)modeNormal();
  _mode=m;
}

void UnderwaterRenderer::enable (){
  _shader->use();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // todo: enable cull face
  glDisable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
}
void UnderwaterRenderer::render(View *view, Model::FancyMesh mesh){

  if(!mesh.glState.visible){
    return;
  }
 
  mat4 projectionMat;
  mat4 viewMat;
  projectionMat = view->projectionMatrix();
  viewMat= view->viewMatrix();
  if(_mode == skybox){
    viewMat[3][0]=0;
    viewMat[3][1]=0;
    viewMat[3][2]=0;
  }
  if(reflectY){
    viewMat[1][0] *= -1;
    viewMat[1][1] *= -1;
    viewMat[1][2] *= -1;
    // viewMat[3][1] *= -1;
  }

  Sunlight sun = view->getSunlight();
  setUniform(lightDirLoc,   sun.lightDir);
  setUniform(lightIntenLoc, sun.lightInten);
  setUniform(lightAmbLoc,   sun.lightAmb);

  Fog fog = view->getFog();
  setUniform(fogOnLoc, fog.fogOn);
  setUniform(fogColorLoc, fog.fogColor);
  setUniform(fogDensityLoc, fog.fogDensity);
  setUniform(fogStartLoc, fog.fogStart);

  OceanColoring oc = view->getColoring();
  setUniform(oceanColoringOnLoc, oc.oceanColoringOn);
  setUniform(oceanTopBrightnessLoc, oc.oceanTopBrightness);
  setUniform(oceanBottomBrightnessLoc, oc.oceanBottomBrightness);
  setUniform(oceanTopColorLoc, oc.oceanTopColor);
  setUniform(oceanBottomColorLoc, oc.oceanBottomColor);
  setUniform(oceanDensityLoc, oc.oceanDensity);
  setUniform(surfaceDepthLoc, oc.surfaceDepth);
  setUniform(floorDepthLoc, oc.floorDepth);

  setUniform(shouldTextureLoc, 1);
  texture2d *tex = mesh.mesh->data.tex;
  if(tex){
    tex->Bind();
    tex->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    tex->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  }
  // setUniform(texSamplerLoc, 0);

  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);
  setUniform(colorLoc, mesh.glState.color);

  glPolygonMode(GL_FRONT_AND_BACK, mesh.glState.polymode);
  glCullFace(mesh.glState.cullmode);
  glLineWidth(mesh.glState.linewidth);


  mesh.mesh->draw();
}





WaterRenderer::WaterRenderer (Shader *sh)
    : Renderer (sh){

  utime = _shader->getUniformLocation("time");
  uscreensize = _shader->getUniformLocation("screensize");
  uTexReflDepth = _shader->getUniformLocation("mapReflDepth");
  uTexReflColor = _shader->getUniformLocation("texSampler");
  ucampos = _shader->getUniformLocation("campos");
  reflection_texture = 0;
  time = 0;
  screensize=vec2(100,100);
}

WaterRenderer::~WaterRenderer ()
{ }

void WaterRenderer::enable()
{
  _shader->use();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(1);
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  // glCullFace(GL_BACK);

  glUniform1i(uTexReflColor,  0);
  glUniform1i(uTexReflDepth,  1);
}

void WaterRenderer::render (View *view, Model::FancyMesh mesh) {
  mat4 projectionMat = view->projectionMatrix();
  mat4 viewMat = view->viewMatrix();
  setUniform(ucampos,vec4(view->activeCamera()->position(),1));
  setUniform(utime, time);
  setUniform(uscreensize, screensize);
  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);

  setUniform(colorLoc, mesh.glState.color);

  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D,reflection_texture);
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D,texReflDepth);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  mesh.mesh->draw();
}