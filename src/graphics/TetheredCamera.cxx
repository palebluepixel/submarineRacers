#include "TetheredCamera.hxx"
#include <util/log.hxx>

TetheredCamera::TetheredCamera(cameraType type, Entity *tether, vec3 fixed)
{
	this->tethered = 1;
	this->type = type;
	this->tether = tether;
	this->fixed = fixed;
	this->dist = 5;

	// logln(LOGMEDIUM,"tethering to: %p",tether);
	if(tether){
		vec3 pos = tether->getPosition()+fixed;
		log(LOGMEDIUM, "%f %f %f\n", pos[0], pos[1], pos[2]);
		this->init(pos, vec3(0,0,0), vec3(0,1,0));
	}
}

TetheredCamera::~TetheredCamera() 
{ }

/* All camera movement is done through rotate commands. If we are
a first person camera, we rotate in place using ypr. If we are a
third person camera, we move on the surface of a sphere of radius
this->dist surrounding our fixed point on the entity. */
void TetheredCamera::rotateThetheredCamera(vec3 rotate)
{
	if(this->type == FIRSTPERSON)
		this->addYPR(rotate);
	else
		return; /*TODO*/
}

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
	else
		return; /*TODO*/
}