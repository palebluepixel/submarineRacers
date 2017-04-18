#ifndef _RENDER_HXX_
#define _RENDER_HXX_

#include "view.hxx"
#include <ent/Entity.hxx>
#include "shader.hxx"

using namespace glm;

//! an abstract base class that wraps a renderer behavior
//
class Renderer {
  public:

    /* Prepare the shader for this renderer for use, and set openGL global
    state information */
    void Enable ();

  //! render a mesh using this renderer
  //! \param modelViewMat the model-view matrix for the mesh and camera
  //! \param mesh the mesh to be rendered
    void Render (View *view, Entity *entity);
    virtual void Render (View *view, Mesh *mesh) = 0;


    virtual ~Renderer ();

  protected:
    Shader  *_shader; //!< the shader program

    GLint positionLoc;
    GLint projectionLoc;
    GLint modelViewLoc;
    GLint colorLoc;
    GLint modelLoc;

    
    Renderer (Shader *sh);

};


class FlatShadingRenderer : public Renderer {
  public:
    FlatShadingRenderer (Shader *sh);
    ~FlatShadingRenderer ();

    void Render (View *view, Mesh *mesh);
};


class SunlightShadingRenderer : public Renderer {
  public:
    SunlightShadingRenderer(Shader *sh);
    ~SunlightShadingRenderer();

    void Render (View *view, Mesh *mesh);

  protected:
    GLint lightDirLoc;
    GLint lightIntenLoc;
    GLint lightAmbLoc;

    GLint fogOnLoc;
    GLint fogColorLoc;
    GLint fogDensityLoc;
    GLint fogStartLoc;

    GLint shouldTextureLoc;
    GLint texSamplerLoc;
};

class UnderwaterRenderer : public SunlightShadingRenderer {
  public:
    UnderwaterRenderer(Shader *sh);
    ~UnderwaterRenderer();

    void Render(View *view, Mesh *mesh);

  protected:
    GLint oceanColoringOnLoc;
    GLint oceanTopBrightnessLoc;
    GLint oceanBottomBrightnessLoc;
    GLint oceanTopColorLoc;
    GLint oceanBottomColorLoc;
    GLint oceanDensityLoc;
    GLint surfaceDepthLoc;
    GLint floorDepthLoc;

};

class SkyboxRenderer : public Renderer {
  public:
    SkyboxRenderer(Shader *sh);
    ~SkyboxRenderer();

    void Render(View *view, Mesh *mesh);

  protected:
    GLint camPosLoc; 

    GLint oceanTopBrightnessLoc;
    GLint oceanBottomBrightnessLoc;
    GLint oceanBottomColorLoc;
    //GLint surfaceDepth;  //everything above this is max bright
    //GLint floorDepth;    //everything below this is max dark
};

#endif // !_RENDER_HXX_
