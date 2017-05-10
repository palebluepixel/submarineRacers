#include "Track.hxx"


Track::Track()
{

}

Track::~Track()
{

}


void Track::addSeekPoint(SeekPoint *sp)
{
    this->seeks.push_back(sp);
}


/* Returns the seekpoint that is howFar ahead of cur. The AI should keep track
of which seek point it is currently at */
SeekPoint *Track::getNextSeekPoint(int cur, int howFar)
{
    int target = (cur + howFar) % this->nSeeks();
    return seeks[target];
}
