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
    if(mesh.mesh != NULL){
      setUniform(modelLoc, modelMatrix * mesh.transform);
      this->render(view, mesh);
    }
  }
}

/*==================== class FlatShadingRenderer member functions======================*/

FlatShadingRenderer::FlatShadingRenderer (Shader *sh)
    : Renderer (sh)
{ }

FlatShadingRenderer::~FlatShadingRenderer ()
{ }

void FlatShadingRenderer::render (View *view, TransformedMesh mesh) {

  mat4 projectionMat = view->projectionMatrix();
  mat4 viewMat = view->viewMatrix();

  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);
  setUniform(colorLoc, mesh.mesh->data.color);
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

void SunlightShadingRenderer::render (View *view, TransformedMesh mesh){
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
  setUniform(colorLoc, mesh.mesh->data.color);
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
}

UnderwaterRenderer::~UnderwaterRenderer()
{ }

void UnderwaterRenderer::enable (){
  _shader->use();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}
void UnderwaterRenderer::render(View *view, TransformedMesh mesh){

  if(!mesh.mesh->data.visible){
    return;
  }
 
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
  setUniform(colorLoc, mesh.mesh->data.color);

  glPolygonMode(GL_FRONT_AND_BACK, mesh.mesh->data.polygon_mode);

  mesh.mesh->draw();
}






/*==================== class SkyboxRenderer member functions======================*/

SkyboxRenderer::SkyboxRenderer (Shader *sh)
    : Renderer (sh)
{ 
  camPosLoc = _shader->getUniformLocation("camPos");

  oceanTopBrightnessLoc = _shader->getUniformLocation("oceanTopBrightness");
  oceanBottomBrightnessLoc = _shader->getUniformLocation("oceanBottomBrightness");
  oceanBottomColorLoc = _shader->getUniformLocation("oceanBottomColor");
}

SkyboxRenderer::~SkyboxRenderer(){}

void SkyboxRenderer::enable (){
  _shader->use();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
}


void SkyboxRenderer::render (View *view, TransformedMesh mesh){
  mat4 projectionMat = view->projectionMatrix();
  mat4 viewMat = view->viewMatrix();

  OceanColoring oc = view->getColoring();
  setUniform(oceanTopBrightnessLoc, oc.oceanTopBrightness);
  setUniform(oceanBottomBrightnessLoc, oc.oceanBottomBrightness);
  setUniform(oceanBottomColorLoc, oc.oceanBottomColor);

  setUniform(camPosLoc, view->activeCamera()->position());

  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);
  setUniform(colorLoc, mesh.mesh->data.color);
  mesh.mesh->draw();
}