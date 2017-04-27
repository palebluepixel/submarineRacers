#ifndef _TETHERED_CAMERA_
#define _TETHERED_CAMERA_


#include <graphics/camera.hxx>
#include <ent/Entity.hxx>
#include <util/log.hxx>

/* First person camera is fixed to a point on the Entity and rotates its
look direction. A third person camera looks at a fixed point on the
Entity and moves its position */
typedef enum {
	FIRSTPERSON,
	THIRDPERSON
} cameraType;


/* =============TODO============
   Third person camera controls */

class TetheredCamera : public Camera {
public:
	TetheredCamera(cameraType type, Entity *tether, vec3 fixed);
	~TetheredCamera();

	/* All camera movement is done through rotate commands. If we are
	a first person camera, we rotate in place using ypr. If we are a
	third person camera, we move on the surface of a sphere of radius
	this->dist surrounding our fixed point on the entity. */
	void rotateThetheredCamera(vec3 rotate);
	void setCameraDistance(float dist);
	void translateCameraDistance(float delta);

	/* Updates our fixed point to be relative to the entity's new position.
	This should be called every graphics tick to make sure our camera
	sticks with the thether point. If we are a first person camera, this
	just sets camera->_pos to be thether->getPosition() + fixed. If we are
	a third person camera, we need to move  */
	void updateTetheredCameraPos();

protected:
	cameraType type;

	Entity * tether;

	/* If we are a first person camera, this is our fixed position,
	   If we are a third person camera, this is our fixed look point, 
	   Where the origin is the position of the Entity to which we are
	   tethered. 
	*/
	vec3 fixed;

	/* If we are a third person camera, this is out distance from the
	thether point */
	float dist;

};

#endif //!_THETHERED_CAMERA_