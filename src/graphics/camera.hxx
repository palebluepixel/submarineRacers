
#ifndef _CAMERA_HXX_
#define _CAMERA_HXX_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//! The camera class encapsulates the current view and projection matrices.
//! Note that we track the camera's position using double-precision so that
//! we can support large worlds.
//

using namespace glm;

 //container strcut for light information
struct Sunlight {
  vec3 lightDir;
  vec3 lightInten;
  vec3 lightAmb;
};

class Camera {
  public:

  //! constuctor for uninitialized camera
    Camera ();

    //places the camera at the origin with the given position, looking at the given point, with the given up
    void init(vec3 pos, vec3 dir, vec3 up);

  //! the current camera position.
    vec3 position () const { return this->_pos; }
  //! the direction that the camera it looking at.
    vec3 direction () const { return this->_dir; }
  //! the camera's current up vector
    vec3 up () const { return this->_up; }

  //! the distance to the near plane of the view frustum
    float near () const { return this->_nearZ; }
  //! the distance to the far plane of the view frustum
    float far () const { return this->_farZ; }
  //! the aspect ratio of the viewport (height/width)
    float aspect () const { return this->_aspect; }
  //! the horizontal field-of-view of the view frustum
    float fov () const { return degrees(2.0 * this->_halfFOV); }

    void setUp(vec3 const &up);

  //! translate a point to the coordinate system that has the camera as the origin, but is
  //! oriented and scaled the same as the world coordinates.
    vec3 translate (vec3 const &p) const;

  //! a transformation matrix that assumes that the camera is at the origin.
    mat4 viewTransform () const;

  //! a modelView transform matrix for the camera
    mat4 ModelViewMatrix () const;

  //! the projection transform for the camera
    mat4 projTransform () const;

  //! update the camera for the aspect ratio of the given viewport.  This operation changes
  //! the aspect ratio, but not the field of view.
    void setViewport (int wid, int ht);

  //! set the horizontal field of view in degrees
    void setFOV (float angle);

  //! set the near and far planes
    void setNearFar (double nearZ, double farZ);

  //! move the camera to a new position while maintaining its heading
    void move (vec3 const &pos);

  //! move the camera to a new position and heading
    void move (vec3 const &pos, vec3 const &at);

  //! move the camera to a new position, heading, and up vector
    void move (vec3 const &pos, vec3 const &at, vec3 const &up);

  //! change the direction of the camera
    void look (vec3 const &dir);

  //! change the direction of the camera
    void look (vec3 const &dir, vec3 const &up);


    //=====rotate camera controls======//

    vec3 getLookVec();

    //rotate camera around an arbitrary axis
    void rotateCam(float theta, vec3 axis);

    void rotateCamUpDown(float theta);

    void rotateCamLeftRight(float theta);

    void rotateCamRoll(float theta);

    void rotateX(float theta);

    void rotateY(float theta);

    void rotateZ(float theta);


    //=====translate camera and look at point=====/

    //translate cam along arbitrary axis, without rotating view at all
    void translateCam(vec3 offset);

    void translateCamViewAxis(float dis);
    
    void moveZAxis(float dis);

    void translateCamStrafeAxis(float dis);

    void translateCamUpAxis(float dis);

  //! compute the screen-space error for a geometric error
  //! \param dist the distance to the geometric error
  //! \param the geometric error
  //! \return the screen-space error
    float screenError (float dist, float err) const;

     vec3 _dir;   //!< the current direction that the camera is pointing towards

  private:
    vec3    _pos;       //!< position is double precision to allow large world
    vec3    _up;        //! camera up vector
    float       _nearZ;     //!< distance to the near plane
    float       _farZ;      //!< distance to the far plane
    float       _aspect;    //! the aspect ratio of the viewport (height / width)
    float       _halfFOV;   //!< horizontal field of view / 2 (in radians)
    mutable float   _errorFactor;   //!< viewport width/(2 * tan(_halfFOV)); set to -1 when invalid
    int         _wid;       //!< the width of the viewport

};


#endif // !_CAMERA_HXX_
