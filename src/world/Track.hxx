#ifndef _TRACK_HXX_
#define _TRACK_HXX_ 

#include <ent/Checkpoint.hxx>
#include <unordered_map>

/* Color of cleared and uncleared checkpoints */
#define UNCLEAREDCOLOR vec4(1.0, 0.5, 0.5, 1.0)
#define CLEAREDCOLOR vec4(0.5,1.0,0.5,1.0)
#define FINISHLINECOLOR vec4(1.0,1.0,1.0,1.0)


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

private:

    /* Vector of all seekpoints in the order that they ought to be completed. 
    Used for AI pathfinding. */
    vector<SeekPoint*> seeks;

    /* Vector of all checkpoints in the order they ought to be completed. Used by
    both player and AI to verify that they have completed the lap. */
    vector<CheckPoint*> checks;

    /* Number of levels needed to win the track */
    int lapsToWin;

};

#endif