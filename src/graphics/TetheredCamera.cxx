#include "TetheredCamera.hxx"
#include <util/log.hxx>

TetheredCamera::TetheredCamera(cameraType type, Entity *tether, vec3 fixed)
{
	this->tethered = 1;
	this->type = type;
	this->tether = tether;
	this->fixed = fixed;
	this->dist = 5;

	// loglnln(LOGMEDIUM,"tethering to: %p",tether);
	if(tether){
		vec3 pos = tether->getPosition()+fixed;
		logln(LOGMEDIUM, "%f %f %f\n", pos[0], pos[1], pos[2]);
		this->init(pos, vec3(0,0,0), vec3(0,1,0));
	} else {
		// Start at the origin if entity was null
		this->init(vec3(0,0,0), vec3(0,0,0), vec3(0,1,0));
	}
}

TetheredCamera::~TetheredCamera() 
{ }

/* Change our entity */
void TetheredCamera::changeTether(Entity *entity)
{
	this->tether = entity;
}


vec3 TetheredCamera::sphericalToCartesian(float yaw, float pitch, float r)
{
	return vec3(r*sin(pitch)*cos(yaw), r*sin(pitch)*sin(yaw), r*cos(pitch));

}

/*void TetheredCamera::setYPR(float yaw, float pitch, float roll)
{
	if(!tether) return;
	if(this->type == FIRSTPERSON){
		((Camera*)this)->setYPR(yaw, pitch, roll);
	} else {
		this->_pos = tether->getPosition() + fixed + 
			this->sphericalToCartesian(yaw, pitch, this->dist);
		this->ypr[2] = roll;
		this->updateYPRLookDir(tether->getPosition() + fixed);
	}
}*/

void TetheredCamera::setCameraDistance(float dist)
{
	this->dist = dist;
}

void TetheredCamera::translateCameraDistance(float delta)
{
	/*TODO*/
}

/* Updates our fixed point to be relative to the entity's new position.
This should be called every graphics tick to make sure our camera
sticks with the thether point. If we are a first person camera, this
just sets camera->_pos to be thether->getPosition() + fixed. If we are
a third person camera, we need to move  */
void TetheredCamera::updateTetheredCameraPos(){
	if(!tether)return;
	if(this->type == FIRSTPERSON){
		this->_pos = tether->getPosition() + fixed;
		this->updateLookDir();
	}
	else {

	}
}