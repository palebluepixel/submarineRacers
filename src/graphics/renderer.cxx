#include "renderer.hxx"
#include <stdio.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

/***** virtual class Renderer member functions *****/

Renderer::Renderer (Shader *sh)
    : _shader(sh)
{ 
  //positionLoc = glGetAttribLocation(_shader->ID(), "aPosition");
  projectionLoc = _shader->getUniformLocation("projection");
  modelViewLoc = _shader->getUniformLocation ("modelView");
  colorLoc = _shader->getUniformLocation("color");
  modelLoc = _shader->getUniformLocation("model");
}

Renderer::~Renderer ()
{ }

void Renderer::Enable ()
{
  _shader->use();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable (GL_DEPTH_TEST);
}

void Renderer::Render(View *view, Entity *entity)
{
  //std::cout << to_string(entity->modelMatrix());
  setUniform(modelLoc, entity->modelMatrix());

  int i;
  int n = entity->getNMeshes();
  Mesh ** meshes = entity->getMeshes();
  if(meshes == NULL)
    return;
  for (i=0; i<n; i++){
    if(meshes[i] != NULL)
      this->Render(view, meshes[i]);
  }
}

/*==================== class FlatShadingRenderer member functions======================*/

FlatShadingRenderer::FlatShadingRenderer (Shader *sh)
    : Renderer (sh)
{ }

FlatShadingRenderer::~FlatShadingRenderer ()
{ }

void FlatShadingRenderer::Render (View *view, Mesh *mesh)
{

  //std::cout << "model view: " << glm::to_string(modelViewMat) << "\ncolor: " << glm::to_string(mesh->color) << "\n";

  mat4 projectionMat = view->projectionMatrix();
  mat4 viewMat = view->viewMatrix();

  //std::cout << "view matrix: " << glm::to_string(viewMat * modelMat) << "\n";
  //std::cout << "projection matrix: " << glm::to_string(projectionMat) << "\n";

  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);
  setUniform(colorLoc, mesh->color);
  mesh->draw();

}



/*==================== class SunlightShadingRenderer member functions======================*/

SunlightShadingRenderer::SunlightShadingRenderer (Shader *sh)
    : Renderer (sh)
{ 
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

SunlightShadingRenderer::~SunlightShadingRenderer()
{ }

void SunlightShadingRenderer::Render (View *view, Mesh *mesh)
{
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

  setUniform(shouldTextureLoc, mesh->shouldTexture);
  texture2d *tex = mesh->tex;
  tex->Bind();
  tex->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
  tex->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  setUniform(texSamplerLoc, 0);

  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);
  setUniform(colorLoc, mesh->color);
  mesh->draw();

}





/*==================== class UnderwaterRenderer member functions======================*/

UnderwaterRenderer::UnderwaterRenderer (Shader *sh)
    : SunlightShadingRenderer(sh)
{ }

UnderwaterRenderer::~UnderwaterRenderer()
{ }

void UnderwaterRenderer::Render(View *view, Mesh *mesh)
{ 
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

  setUniform(shouldTextureLoc, mesh->shouldTexture);
  texture2d *tex = mesh->tex;
  tex->Bind();
  tex->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
  tex->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  setUniform(texSamplerLoc, 0);

  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);
  setUniform(colorLoc, mesh->color);
  mesh->draw();
}






/*==================== class SkyboxRenderer member functions======================*/

SkyboxRenderer::SkyboxRenderer (Shader *sh)
    : Renderer (sh)
{ 
  camPosLoc = _shader->getUniformLocation("camPos");

  oceanTopBrightnessLoc = _shader->getUniformLocation("oceanTopBrightness");
  oceanBottomBrightnessLoc = _shader->getUniformLocation("oceanBottomBrightness");
  oceanFarColorLoc = _shader->getUniformLocation("oceanFarColor");
}

SkyboxRenderer::~SkyboxRenderer()
{ }

void SkyboxRenderer::Render (View *view, Mesh *mesh)
{
  mat4 projectionMat = view->projectionMatrix();
  mat4 viewMat = view->viewMatrix();

  OceanColoring oc = view->getColoring();
  setUniform(oceanTopBrightnessLoc, oc.oceanTopBrightness);
  setUniform(oceanBottomBrightnessLoc, oc.oceanBottomBrightness);
  setUniform(oceanFarColorLoc, oc.oceanFarColor);

  setUniform(camPosLoc, view->activeCamera()->position());
  setUniform(modelViewLoc, viewMat);
  setUniform(projectionLoc, projectionMat);
  setUniform(colorLoc, mesh->color);
  mesh->draw();

}