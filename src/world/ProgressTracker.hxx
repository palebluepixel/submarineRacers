#ifndef _PROGRESS_TRACKER_
#define _PROGRESS_TRACKER_

#include <vector>

using namespace std;


/* Status for whether an entity has cleared a given point or not.
NOTCLEARED must be kept equal to 0. */
typedef enum {
    NOTCLEARED=0,
    CLEARED
} clearedStatus;



/* Maintains a data structure of every check/seekpoint in a track and information about 
what points the player / AI has cleared in this lap. One of these is maintained for each 
player / AI. */
class ProgressTracker {

public:
    ProgressTracker();
    ~ProgressTracker();

    /* Creates the array of statuses with length n. If the array has already been
    created, it resets them all to NOTCLEARED. This will also reset the number of
    laps and curPoint. */
    void initalizePoints(int n);

    /* Return 1 if the player has cleared the given point, 0 otherwise */
    int pointCleared(int p);

    /* Sets the status of the given point to CLEARED and updates curPoint
    to be the next point in the array. returns the updated curPoint */
    int clearPoint(int p);

    /* returns 1 if every point has been completed, 0 otherwise */
    int isLapComplete();

    /* resets every point to uncompleted and increments the lap count. Also
    returns number of laps completed. */
    int completeLap();


    /* Returns the index of the point the player / AI needs to complete next */
    inline int getCurPoint() { return curPoint; }

    /* Returns the index of the point that is off ahead of the current point
    (can be used for looking further ahead). off = 0 gives the same result as
    getCurPoint() */
    int getNextPoint(int off);

    inline int nPoints() {return points.size();}

protected:

    /* Reset every point to be NOTCLEARED */
    void resetPoints();

    int nLaps; // Number of completed laps
    int curPoint; // The seekpoint this entity should be headed towards

    /* Array of status for each point. */
    vector<clearedStatus> points;

};



#endif