#ifndef _TRACK_HXX_
#define _TRACK_HXX_ 

#include <ent/Checkpoint.hxx>
#include <unordered_map>
#include <world/defines.hxx>

/* Color of cleared and uncleared checkpoints */
#define UNCLEAREDCOLOR vec4(1.0, 0.5, 0.5, 1.0)
#define CLEAREDCOLOR vec4(0.5,1.0,0.5,1.0)
#define FINISHLINECOLOR vec4(1.0,1.0,1.0,1.0)

class startInfo {
public:
    startInfo();
    startInfo(vec3 pos, quaternion ori, vec3 vel);
    vec3 pos;
    quaternion ori;
    vec3 vel;
};

/* Defines the path that players and AI must take through the level. */
class Track {

public:

    Track(int lapsToWin);
    ~Track();

    void addSeekPoint(SeekPoint *sp);
    void addCheckPoint(CheckPoint *cp);

    /* Returns the seekpoint that is howFar ahead of cur. The AI should keep track
    of which seek point it is currently at */
    SeekPoint *getNextSeekPoint(int cur, int howFar);
    CheckPoint *getNextCheckPoint(int cur, int howFar);

    inline int nSeeks() { return seeks.size(); }
    inline int nChecks() { return checks.size(); }

    inline int getLapsToWin() { return this->lapsToWin; }

    /* Add player starting positions to our vector */
    void addStartingInfo(startInfo start);
    void addStartingInfo(startInfo *starts, int n);

    /* Get the ith player start position. Returns vec3(0,0,0) if i is beyond the
    bounds of the starting positions vector */
    startInfo getStartInfoi(int i);



    /* Used for visual client-side changes to checkpoints as the player-controlled 
    submarine makes progress. These should be called when the player recieves a 
    message from the server indicating that they have cleared the given checkpoint.

    Example behavior may include: changing the color, starting an animation, changing
    the appearance as the player draws closer, etc */
    void clearCheckVis(int id);
    void clearCheckVis(CheckPoint *check);

    void resetCheckVis(int id);
    void resetCheckVis(CheckPoint *check);
    void resetAllChecksVis();

    /* Tells the track that player p has finished the race. Returns the integer of
    the position the player finished the race in, e.g. 1st, 2nd, ... 
    Returns -1 after this function is called more times than the maximum number
    of players. */
    int playerFinish(int pno);

    /* A map of positions in the race to the player number who finished in
    that position. Each client is informed every time a player finishes the
    race, but we also maintain that data here for the server in case we
    want to do anything with it. */
    int positions[WORLD_MAX_PLAYERS];

private:

    /* Vector of all seekpoints in the order that they ought to be completed. 
    Used for AI pathfinding. */
    vector<SeekPoint*> seeks;

    /* Vector of all checkpoints in the order they ought to be completed. Used by
    both player and AI to verify that they have completed the lap. */
    vector<CheckPoint*> checks;

    /* Vector of player start positions and orientations. Each sub should start 
    at a unique position.*/
    vector<startInfo> startPoints;

    /* Number of levels needed to win the track */
    int lapsToWin;

    /* Position of next submarine to finish in the race - 1*/
    int nextPosition;

    /* Scaling info */
    vec3 scale;


};

#endif