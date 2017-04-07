#ifndef _RENDER_HXX_
#define _RENDER_HXX_

#include "mesh.hxx"
#include "shader.hxx"

using namespace glm;

//! an abstract base class that wraps a renderer behavior
//
class Renderer {
  public:

  //! enable the renderer
  //! \param projectMat the projection matrix for the current camera state
    virtual void Enable (mat4 const &projectionMat) = 0;

  //! render a mesh using this renderer
  //! \param modelViewMat the model-view matrix for the mesh and camera
  //! \param mesh the mesh to be rendered
    void Render (mat4 const &modelViewMat, Mesh *mesh);

    virtual ~Renderer ();

  protected:
    Shader  *_shader; //!< the shader program

    GLint projectionLoc;
    GLint modelViewLoc;
    GLint colorLoc;

    
    Renderer (Shader *sh);

};
class FlatShadingRenderer : public Renderer {
  public:
    FlatShadingRenderer (Shader *sh);
    virtual ~FlatShadingRenderer ();

    void Enable (mat4 const &projectionMat);

};

#endif // !_RENDER_HXX_
