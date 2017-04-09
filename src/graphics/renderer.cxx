
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
}

Renderer::~Renderer ()
{ }

void Renderer::Render (mat4 const &modelViewMat, Mesh *mesh)
{

  //std::cout << "model view: " << glm::to_string(modelViewMat) << "\ncolor: " << glm::to_string(mesh->color) << "\n";

    setUniform(modelViewLoc, modelViewMat);
    setUniform(colorLoc, mesh->color);
    mesh->Draw();

}




/*==================== class FlatShadingRenderer member functions======================*/

FlatShadingRenderer::FlatShadingRenderer (Shader *sh)
    : Renderer (sh)
{ }

FlatShadingRenderer::~FlatShadingRenderer ()
{ }

void FlatShadingRenderer::Enable (mat4 const &projectionMat)
{
  _shader->use();
  //std::cout << "projection: " << glm::to_string(projectionMat) << "\n";
  setUniform(projectionLoc,projectionMat);
}
