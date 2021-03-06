#include "Track.hxx"
#include <glm/gtx/vector_angle.hpp>
#include <util/mod.hxx>

startInfo::startInfo()
{
    this->pos = vec3(0,0,0);
    this->ori = quaternion();
    this->vel = vec3(0,0,0);
}

startInfo::startInfo(vec3 pos, quaternion ori, vec3 vel)
{
    this->pos = pos;
    this->ori = ori;
    this->vel = vel;
}


Track::Track(int lapsToWin)
{
    this->lapsToWin = lapsToWin;
    this->nextPosition = 0;
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

void Track::addStartingInfo(startInfo start)
{
    this->startPoints.push_back(start);
}

/* Add player starting positions to our vector */
void Track::addStartingInfo(startInfo *starts, int n)
{
    int i;
    for(i=0; i<n; i++)
        this->startPoints.push_back(starts[i]);
}

/* Get the ith player start position. Returns a default start if i is beyond the
bounds of the starting positions vector */
startInfo Track::getStartInfoi(int i)
{
    if(i < 0 || i >=this->startPoints.size())
        return startInfo(vec3(0,0,0), quaternion(), vec3(0,0,0));
    return this->startPoints[i];
}

/* Used for visual client-side changes to checkpoints as the player-controlled 
submarine makes progress. These should be called when the player recieves a 
message from the server indicating that they have cleared the given checkpoint.

Example behavior may include: changing the color, starting an animation, changing
the appearance as the player draws closer, etc */
void Track::clearCheckVis(int id)
{
    this->clearCheckVis(this->getNextCheckPoint(id,0));
    this->nextCheckVis(this->getNextCheckPoint(id,1));
}

void Track::nextCheckVis(CheckPoint *check)
{
    check->setMeshColor(0,NEXTCOLOR);
}

void Track::clearCheckVis(CheckPoint *check)
{
    check->setMeshColor(0,CLEAREDCOLOR);
}


void Track::resetCheckVis(int id)
{
    this->resetCheckVis(this->getNextCheckPoint(id,0));
}

void Track::resetCheckVis(CheckPoint *check)
{
    if(check->isFinishLine())
        check->setMeshColor(0, FINISHLINECOLOR);
    else
        check->setMeshColor(0,UNCLEAREDCOLOR);
}

/* Called after lap completion */
void Track::resetAllChecksVis()
{
    for(CheckPoint *check : this->checks)
        this->resetCheckVis(check);
    this->nextCheckVis(this->checks[0]);
}


/* Tells the track that player p has finished the race. Returns the integer of
the position the player finished the race in, e.g. 1st, 2nd, ... 
Returns -1 after this function is called more times than the maximum number
of players. */
int Track::playerFinish(int pno)
{
    if(this->nextPosition >= WORLD_MAX_PLAYERS || this->nextPosition < 0)
        return -1;

    this->positions[this->nextPosition++] = pno;
    return this->nextPosition;
}