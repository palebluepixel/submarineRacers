#include "ProgressTracker.hxx"
#include <util/log.hxx>
#include <util/mod.hxx>

ProgressTracker::ProgressTracker()
{
    this->nLaps = 0;
    this->curPoint = -1;
}

ProgressTracker::~ProgressTracker() {}

/* Reset every point to be NOTCLEARED */
void ProgressTracker::resetPoints()
{
    int i;
    for(i=0; i<this->nPoints(); i++)
        this->points[i] = NOTCLEARED;
}

/* Creates the array of statuses with length n. If the array has already been
created, it resets them all to NOTCLEARED. This will also reset the number of
laps and curPoint. */
void ProgressTracker::initalizePoints(int n)
{
    /* Remove all points */
    this->points.clear();

    int i;
    for(i=0; i<n; i++)
        this->points.push_back(NOTCLEARED);

    this->curPoint = 0;
    this->nLaps = 0;
}

/* Return 1 if the player has cleared the given point, 0 otherwise */
int ProgressTracker::pointCleared(int p)
{
    if(p < 0 || p > this->nPoints()){
        logln(LOGERROR, "Invalid point in pointCleared %d", p);
        return 0;
    }

    return (int)this->points[p];
}

/* Sets the status of the given point to CLEARED and updates curPoint
to be the next point in the array. returns the updated curPoint.
If we have already cleared this point, instead does nothing. */
int ProgressTracker::clearPoint(int p)
{
    if(p < 0 || p > this->nPoints()){
        logln(LOGERROR, "Invalid point in clearPoint %d", p);
        return 0;
    }

    if(this->points[p] == NOTCLEARED)
        this->points[p] = CLEARED;
    curPoint = modularIndexOffset(p, 1, points.size());
    return curPoint;
}

int ProgressTracker::getNextPoint(int off)
{
    if(points.size() <= 0)
        return -1;
    return modularIndexOffset(curPoint, off, points.size());
}

/* returns 1 if every point has been completed, 0 otherwise */
int ProgressTracker::isLapComplete()
{
    int i;
    for(i=0; i<this->nPoints(); i++){
        if(this->points[i] == NOTCLEARED)
            return 0;
    }
    return 1;
}

/* resets every point to uncompleted and increments the lap count. Also
returns number of laps completed. */
int ProgressTracker::completeLap()
{
    this->resetPoints();
    return ++this->nLaps;
}