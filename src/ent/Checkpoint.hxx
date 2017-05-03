#ifndef _CHECKPOINT_HXX_
#define _CHECKPOINT_HXX_

#include <ent/Entity.hxx>

/* Class for checkpoints and used to represent the path
of a track */
class Checkpoint : public Entity {

public:
	Checkpoint(/*Plane *plane*/);
	~Checkpoint();

protected:

	/* Set to 1 if a submarine needs to clear this checkpoint for a 
	lap to be considered valid. If we use checkpoints for AI pathfinding
	behavior, then we may want to place some checkpoints in the world
	which represent a good apth for the AI, but which are not mandatory
	(in case the player manages to find a legitimate shortcut) */
	int mandatory;

	/* When the player collides with this checkpoint, set the player's
	info to indicate they have passed through it. */

}



/* Finish line. The player must pass through this some number of times
to fin the race, but only if all associated Checkpoints have been passed
through. Currently we don't check the order of the checkpoints */

class FinishLine : public Checkpoint {

public:
	FinishLine(/*Plane *plane, */int laps);
	~FinishLine();

protected:

	/* Array of checkpoints that must be cleared for a pass through this
	finish line to be considered valid. */
} 







#endif