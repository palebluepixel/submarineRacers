#ifndef _TRACK_HXX_
#define _TRACK_HXX_ 

#include <ent/Checkpoint.hxx>
#include <unordered_map>

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