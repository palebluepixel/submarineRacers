#include "AI.hxx"
#include <ent/Entity.hxx>
#include <cmath>
#include <util/log.hxx>
#include <ent/Actuator.hxx>  
#include <glm/gtx/vector_angle.hpp>
#include <util/interpolate.hxx>
#include <world/world.hxx>

extern World *world;

AI::AI() {}

AI::~AI() {}

SubmarineAI::SubmarineAI()
{
    this->racer = NULL;
}

SubmarineAI::~SubmarineAI() {}


void SubmarineAI::updateAI()
{
    /* If racer is null or unbound , we aren't bound to any submarine
    so don't try to do anything. If track is null, this level has no 
    seeking information, so don't try to do anything. */
    if(!racer || !racer->isBound() || !world->getLevel()->getTrack())
        return;

    Submarine *sub = this->getOurSub();

    /* Get our seek point as the next checkpoint in the track*/

    int seekn = sub->getPTSeek()->getNextPoint(0);
    //logln(LOGMEDIUM, "Seeking %d", seekn);
    if(seekn < 0)
        return; //0 points to seek
    SeekPoint *seek = world->getLevel()->getTrack()->getNextSeekPoint(seekn,0);
    //printf("Seeking %d\n", seek->getTrackID());
    vec3 point3d = seek->getCenter();
    vec2 point = vec2(point3d.x, point3d.z);

    this->seekPoint(point);
}

/* Returns 1 if the target is on the left, 0 otherwise */
int targetOnLeft(float angle)
{
    return angle > 0;
}

void SubmarineAI::turnTowards(float angle, float threshAny, float threshFull)
{
    Submarine *sub = this->getOurSub();
    
    float turnSpeed = LERP_FLOAT(abs(angle), threshAny, threshFull, 0, sub->getMaxTurn());
    //printf("Angle: %f Turn speed: %f\n", angle, turnSpeed);

    if(targetOnLeft(angle)) {
        this->getOurSubAct()->turnLeft(turnSpeed);
    } else {
        this->getOurSubAct()->turnRight(turnSpeed);
    }
}


void SubmarineAI::accelerateBasedOn(float angle, float threshAny, float threshFull)
{
    Submarine *sub = this->getOurSub();

    float accelerationSpeed = LERP_FLOAT(abs(angle), threshAny, threshFull, sub->getMaxAccel(), 0.1);

    this->getOurSubAct()->accelerate(accelerationSpeed);

}

void SubmarineAI::seekPoint(vec2 point)
{
    /* Get our current position and heading */
    Submarine *sub = this->getOurSub();
    vec3 facing3D = sub->getDirection();
    vec2 facing = normalize(vec2(facing3D.x, facing3D.z));
    vec3 pos3D = sub->getPosition();
    vec2 pos = vec2(pos3D.x, pos3D.z);

    // Get target direction
    vec2 target = normalize(point-pos);
    //printf("Pos: %f %f Point: %f %f Target: %f %f\n", pos[0], pos[1], point[0], point[1], target[0], target[1]);

    // Angle between our sub and our target direction. We compute it
    // here because we will use it in multiple functions.  
    float angle = orientedAngle(target, facing);

    // Turn towards target
    this->turnTowards(angle, 0.03, 0.2);

    // Accelerate based on direction, but only if we aren't close enough
    float dist = abs(length(point-pos));
    if(dist > CLOSEENOUGH)
        this->accelerateBasedOn(angle, 0.05, 0.1);
}

// AI functions
int return_0(int x){
//what is a computer
     return 0;
}

struct ai_steer {
	//should keep track of the forces 
	// the ai is trying to apply
	int x;
};



//ai_steer follow_track(Checkpoint* track,entity sumbarine){
//given a track, currect checkpoint index, and a submarine
//seeks the next checkpoint 
//  if(sub_entity.oxygen<threshold){
//	steer up
//  	} else {
//  		if(sub_entity.y>checkpoint.center.y && collide(sub_entity,floor){
//		steer down	
//  	}
//  }
// 
//  if(sub_entity.direction_vector is pointing left of next checkpoint){
//	turn_right*magnitude(cross product( sub direction and (adjusted height sub location(-checkpoint))))
//  }
// " same idea but turning left " 
// if(magnitue turn right<threshold) {
//	accelarate
//	} else {
//	deccelerate
// end


// whisker -- idea: spawn a series of 4 squares (rectangles?) 
//of sizes r_1 and r_2 each with one corner centered on the sub
// parallel to the direction the submarine is facing, check for 
//collisions on the squares between heightmaps and other subs
//if a sub is one 1 side of the sub, and there is an open space on the other,
// tend towards the other side, etc. 
//if there is a sub appearing in both squares in front choose the square with
//furthest distance from wall

