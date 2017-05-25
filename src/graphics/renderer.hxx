#ifndef _RENDER_HXX_
#define _RENDER_HXX_

#include <vector>

#include "view.hxx"
#include <ent/Entity.hxx>
#include "shader.hxx"

using namespace glm;

//! an abstract base class that wraps a renderer behavior
//
class Renderer {
  public:
    virtual ~Renderer ();

    /**
     * Prepare the shader for this renderer for use, and set openGL global
     * state information
    **/
    virtual void enable ();

    /**
     * render a mesh using this renderer
     * param modelViewMat the model-view matrix for the mesh and camera
     * param mesh the mesh to be rendered
     */
    virtual void render (View *view, Entity *entity);
    
    /**
     * this function should be called once per render cycle.
     */
    virtual void render();
    
    /**
     * render a single mesh
     */
    virtual void render (View *view, Model::FancyMesh mesh) = 0;


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

    void enable();

    void render (View *view, Model::FancyMesh mesh);
};


class SunlightShadingRenderer : public Renderer {
  public:
    SunlightShadingRenderer(Shader *sh);
    ~SunlightShadingRenderer();

    void render (View *view, Model::FancyMesh mesh);

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

    using Renderer::render;

    void enable();
    void render(View *view, Model::FancyMesh mesh);
    void modeSkybox();
    void modeNormal();
    enum Mode {skybox, normal};
    void setMode(Mode m);
    bool reflectY;
    Mode _mode;

  protected:
    GLint umode;
    GLint oceanColoringOnLoc;
    GLint oceanTopBrightnessLoc;
    GLint oceanBottomBrightnessLoc;
    GLint oceanTopColorLoc;
    GLint oceanBottomColorLoc;
    GLint oceanDensityLoc;
    GLint surfaceDepthLoc;
    GLint floorDepthLoc;

};

class WaterRenderer : public Renderer {
public:
  WaterRenderer(Shader *sh);
  ~WaterRenderer();

  void enable();
  void render(View *view, Model::FancyMesh mesh);
  vec2 screensize;

  GLuint reflection_texture;
  GLint utime;
  GLint uscreensize;
  GLfloat time;

};

#endif // !_RENDER_HXX_
