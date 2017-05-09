#ifndef _TRACK_HXX_
#define _TRACK_HXX_ 

#include <ent/Checkpoint.hxx>

/* Defines the path that players and AI must take through the level. */
class Track {

public:

    Track();
    ~Track();

    void addSeekPoint(SeekPoint *sp);

    /* Returns the seekpoint that is howFar ahead of cur. The AI should keep track
    of which seek point it is currently at */
    SeekPoint *getNextSeekPoint(int cur, int howFar);


private:

    /* Vector of all seekpoints in the order that they ought to be completed. */
    vector<SeekPoint*> seeks;

};

#endif