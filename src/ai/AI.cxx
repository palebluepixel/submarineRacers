#include "AI.hxx"
#include <ent/Entity.hxx>
#include <cmath>
#include <util/log.hxx>
#include <ent/Actuator.hxx>  

AI::AI() {}

AI::~AI() {}

SubmarineAI::SubmarineAI()
{
    this->curSeekPoint = -1;
}

SubmarineAI::~SubmarineAI() {}


void SubmarineAI::updateAI()
{
    this->subAct->accelerate();
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

