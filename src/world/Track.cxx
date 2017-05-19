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

void Track::addCheckPoint(CheckPoint *cp)
{
    cp->setTrackID(this->nChecks());
    this->checks.push_back(cp);
}

/* Returns the seekpoint that is howFar ahead of cur. The AI should keep track
of which seek point it is currently at */
SeekPoint *Track::getNextSeekPoint(int cur, int howFar)
{
    int target = modularIndexOffset(cur, howFar, this->nSeeks());
    return seeks[target];
}

CheckPoint *Track::getNextCheckPoint(int cur, int howFar)
{
    int target = modularIndexOffset(cur, howFar, this->nChecks());
    return checks[target];
}