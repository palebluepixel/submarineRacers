#ifndef _VIEW_HXX_
#define _VIEW_HXX_

/* Contains information for the client's view of the world needed to render,
    namely:
    * Current window
    * Camera location and orientation
    * Sunlight information
    * Skybox
    * Ground map
    * Fog presence / absence
    * view distance
    * etc
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <graphics/TetheredCamera.hxx>
#include <util/log.hxx>


#define NCAMS 5 //maximum number of cameras


//container struct for light information
struct Sunlight {
  vec3 lightDir;
  vec3 lightInten;
  vec3 lightAmb;
};

struct Fog {
    int fogOn; //0=no, 1=yes
    vec3 fogColor;
    float fogDensity;
    float fogStart;
};

struct OceanColoring {
    int oceanColoringOn;
    vec3 oceanTopBrightness;
    vec3 oceanBottomBrightness;
    vec3 oceanTopColor;
    vec3 oceanBottomColor;
    float oceanDensity;
    float surfaceDepth; //everything above this is max bright
    float floorDepth; //everything below this is max dark
};


class View {

public:

    View(GLFWwindow *win);
    
    ~View();

    //Camera
    void addCamera(Camera *cam);
    void switchCamera(int cami);
    void nextCamera();
    Camera *activeCamera();

    //View info for active camera
    inline void  setFOV(float degrees)  { this->activeCamera()->setFOV(degrees); }
    inline void  setNear(float near)    { this->activeCamera()->setNear(near); }
    inline void  setFar(float far)      { this->activeCamera()->setFar(far); }

    inline float getFOV()               { return this->activeCamera()->fov(); }
    inline float getNear()              { return this->activeCamera()->near(); }
    inline float getFar()               { return this->activeCamera()->far(); }

    //View info for current view
    inline void  setWid(int wid)        { this->wid = wid; this->updateAspect(); }
    inline void  setHt(int ht)          { this->ht = ht; this->updateAspect(); }

    inline int   getWid()               { return this->wid; }
    inline int   getHt()                { return this->ht; }
    inline float getAspect()            { return this->aspect; }

    inline mat4  projectionMatrix()     { return this->activeCamera()->projTransform(this->getAspect()); }
    inline mat4  viewMatrix()           { return this->activeCamera()->ModelViewMatrix(); }


    //sunlight
    inline Sunlight getSunlight()       { return this->sunlight; }
    inline void     setSunlight(vec3 lightDir, vec3 lightInten, vec3 lightAmb) 
                    {this->sunlight.lightDir = lightDir; this->sunlight.lightInten = lightInten; 
                        this->sunlight.lightAmb = lightAmb;}

    //fog
    inline Fog  getFog()                 { return this->fog; }
    inline void setFog(int fogOn, vec3 fogColor, float fogDensity, float fogStart)
                {this->fog.fogOn = fogOn; this->fog.fogColor = fogColor;
                    this->fog.fogDensity = fogDensity; this->fog.fogStart = fogStart;}

    //ocean colors
    inline OceanColoring getColoring()   { return this->oc; }
    inline void setColoring(int coloringOn, vec3 topb, vec3 botb, vec3 topc, vec3 botc,
                            float dens, float tdepth, float bdepth)
                {this->oc.oceanColoringOn = coloringOn; this->oc.oceanTopBrightness = topb;
                    this->oc.oceanBottomBrightness = botb; this->oc.oceanTopColor = topc;
                    this->oc.oceanBottomColor = botc; this->oc.oceanDensity = dens;
                    this->oc.surfaceDepth = tdepth; this->oc.floorDepth = bdepth;}


    //First and third person tethered cameras
    inline void setFirstPersonCam(TetheredCamera *t) { this->firstp = t; }
    inline void setThirdPersonCam(TetheredCamera *t) { this->thirdp = t; }
    inline TetheredCamera *getFirstPersonCam() { return this->firstp; }
    inline TetheredCamera *getThirdPersonCam() { return this->thirdp; }

private:    
    GLFWwindow *win;

    Camera **cameras;
    int nCameras;
    int maxCameras;
    int activeCamerai;

    /* Pointers into our cameras array, used for quick access*/
    TetheredCamera *firstp;
    TetheredCamera *thirdp;

    int wid, ht; //window dimensions
    float aspect;

    inline void updateAspect() {this->aspect = (ht!=0.0f ? (float) wid / (float) ht : -6.9f); }

    // Uncomment as they are implemented
    // Skybox * currentSky;
    Sunlight sunlight;
    // Ground * currentGround;
    // view frustum
    Fog fog;
    OceanColoring oc;
};

#endif