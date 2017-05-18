#ifndef _TRACK_HXX_
#define _TRACK_HXX_ 

#include <ent/Checkpoint.hxx>
#include <unordered_map>

/* Defines the path that players and AI must take through the level. */
class Track {

public:

    Track();
    ~Track();

    void addSeekPoint(SeekPoint *sp);

    /* Returns the seekpoint that is howFar ahead of cur. The AI should keep track
    of which seek point it is currently at */
    SeekPoint *getNextSeekPoint(int cur, int howFar);

    /* Returns the nth seekpoint the AI should head towards. To determine this in an 
    efficient way, the AI passes it's current position, as well as the index of
    the seekpoint it is currently tracking. The correct seekpoint will most likely
    either be the seekpoint it is currently tracking or the one right after it. */
    SeekPoint *getNextSeekPoint(int cur, int howFar, vec3 pos);

    inline int nSeeks() { return seeks.size(); }

private:

    /* Returns an index into the seekpoint array such that the array wraps around */
    int modularIndex(int i, int off);

    /* Vector of all seekpoints in the order that they ought to be completed. */
    vector<SeekPoint*> seeks;

};

#endif