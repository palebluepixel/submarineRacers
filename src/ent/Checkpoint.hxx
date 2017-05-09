#ifndef _CHECKPOINT_HXX_
#define _CHECKPOINT_HXX_

#include <ent/Entity.hxx>


class Hexagon {
public:
	/* Construct a hexagon using the 6 corner points */
	Hexagon(vec3 Lt, vec3 Lb, vec3 Mt, vec3 Mb, vec3 Rt, vec3 Rb);

	/* Return a mesh representation of the hexagon as a hexagonal
	prism with the given length. */
	Mesh* getMesh(float length);

	vec3 Lt;
	vec3 Lb;
	vec3 Mt;
	vec3 Mb;
	vec3 Rt;
	vec3 Rb;

	vec3 center;
	vec3 normal;

};


/* Class for checkpoints and used to represent the path
of a track. Use of the word "Player" in this file can refer
to a human player or an AI.  */
class Checkpoint : public Entity {

public:
	Checkpoint(int ID, vec3 initial_position, quaternion initial_orientation,
        std::string name, EntityType type, EntityStatus status, float tick_interval);
	~Checkpoint();

	/* When a player collides with this checkpoint, set the player's
	info to indicate they have passed through it. */
	//onCollide(Entity *)

protected:

	/* Some kind of mesh representation - for now we will use the hex
	of seekpoints, but this may eventually be rings, etc */	

};



/* Finish line. The player must pass through this some number of times
to fin the race, but only if all associated Checkpoints have been passed
through. Currently we don't check the order of the checkpoints */

class FinishLine : public Checkpoint {

public:
	FinishLine(int ID, vec3 initial_position, quaternion initial_orientation,
        std::string name, EntityType type, EntityStatus status, float tick_interval);
	~FinishLine();

	/* When the player collides with this finish line, call the finishLine()
	function of the world, which will go to the current track and check if
	the lap is valid. */
	//onCollide();
}; 



/* A seekpoint is used for AI pathfinding behavior. A track contains an
ordered vector of seekPoints which define, at a minimum, the left and
right edges of a track, as well as the middle line which the AI should
tend towards. 

In the current implementation, we represent seekpoints as a hexagon. */

class SeekPoint : public Entity {

public:
	SeekPoint(int ID, vec3 initial_position, quaternion initial_orientation,
        std::string name, EntityType type, EntityStatus status, float tick_interval,
        Hexagon *hex, int mandatory);
	~SeekPoint();


	/* Set to 1 if a submarine needs to clear this checkpoint for a 
	lap to be considered valid. If we use checkpoints for AI pathfinding
	behavior, then we may want to place some checkpoints in the world
	which represent a good path for the AI, but which are not mandatory
	(in case the player manages to find a legitimate shortcut) */
	int mandatory;

	/* The volume mesh for this entity is represented as a hexagon of
	6 points:

			   Mt
			 __*__
		 __--     --__
	 Lt *             * Rt
	    |             |
	    |             |
	    |             |
	    |             |
	 Lb *__         __* Rb
		   --__ __--
		       *
		       Mb

	The points are not constrained to be a convex hexagon as shown above, 
	but they must be divided into Left, Right, and Middle. The AI knows to
	avoid going left of Lt and Lb at all costs, to avoid going right of Rt
	and Rb at all costs, and to seek towards the middle. 
	*/
	Hexagon *hex;

	void initalizeTextures(const char* texfile);
    void initalizeVisualData();
    void initalizeMeshes();

protected:

};







#endif