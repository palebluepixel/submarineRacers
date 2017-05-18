#include "Track.hxx"
#include <glm/gtx/vector_angle.hpp>
#include <util/mod.hxx>


Track::Track(int lapsToWin)
{
    this->lapsToWin = lapsToWin;
}

Track::~Track()
{

}


void Track::addSeekPoint(SeekPoint *sp)
{
    /* Set the ID of the seekpoint to its position in the array */
    sp->setTrackID(this->nSeeks());

    /* Add to array */
    this->seeks.push_back(sp);
}

/* Returns an index into the seekpoint array such that the array wraps around */
int Track::modularIndex(int i, int off)
{
    return modularIndexOffset(i, off, this->nSeeks());
}

/* Returns the seekpoint that is howFar ahead of cur. The AI should keep track
of which seek point it is currently at */
SeekPoint *Track::getNextSeekPoint(int cur, int howFar)
{
    int target = modularIndex(cur, howFar);
    return seeks[target];
}