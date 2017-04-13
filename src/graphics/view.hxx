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
#include "camera.hxx"


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


class View {

public:

    View(GLFWwindow *win);
    
    ~View();

    //Camera
    void addCamera(Camera *cam);
    void switchCamera(int cami);
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

private:    
    GLFWwindow *win;

    Camera **cameras;
    int nCameras;
    int maxCameras;
    int activeCamerai;

    int wid, ht; //window dimensions
    float aspect;

    inline void updateAspect() {this->aspect = (ht!=0.0f ? (float) wid / (float) ht : -6.9f); }

    // Uncomment as they are implemented
    // Skybox * currentSky;
    Sunlight sunlight;
    // Ground * currentGround;
    // view frustum
    Fog fog;
};

#endif