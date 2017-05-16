#include "Track.hxx"
#include <glm/gtx/vector_angle.hpp>


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

/* Returns an index into the seekpoint array such that the array wraps around */
int Track::modularIndex(int i, int off)
{
    int index = i + off;
    int negflag = 0;
    if(index < 0){
        index = -index;
        negflag = 1;
    }
    int offset = index % this->nSeeks();
    return (negflag) ? this->nSeeks() - offset : offset;
}

/* Returns 1 if the given point is between two seekpoints, 0 otherwise */
/* A point is "between" two seekpoints if the distance vectors to each seekpoint
have a >threshold degree angle between them 

THIS DOESNT WORK BTW */
int betweenSeeks(vec3 point3d, SeekPoint *sp1, SeekPoint *sp2)
{
    /* Get 2d center points of seekpoints and point*/
    vec3 c13d = sp1->getCenter();
    vec3 c23d = sp2->getCenter();
    vec2 c1 = vec2(c13d.x, c13d.z);
    vec2 c2 = vec2(c23d.x, c23d.z);
    vec2 point = vec2(point3d.x, point3d.z);

    /* Check angle between distance vectors */
    return angle(point-c1, point-c2) > 1;
}

/* Returns the seekpoint that is howFar ahead of cur. The AI should keep track
of which seek point it is currently at */
SeekPoint *Track::getNextSeekPoint(int cur, int howFar)
{
    int target = modularIndex(cur, howFar);
    return seeks[target];
}


SeekPoint *Track::getNextSeekPoint(int cur, int howFar, vec3 pos)
{
    /* Get the seekpoint the AI is currently seeking as well as the
    previous and next seekpoints in the array.*/
    int previ = modularIndex(cur,-1);
    int activei = modularIndex(cur,0);
    int nexti = modularIndex(cur,1);
    SeekPoint *prev = seeks[previ];
    SeekPoint *active = seeks[activei];
    SeekPoint *next = seeks[nexti]; 

    /* The most common case is that the AI hasn't reached its current
    tracking seekpoint, meaning it is between prev and active */
    if(betweenSeeks(pos, prev, active))
        return active;

    /* Second most common case is the AI reached its current seekpoint and needs to update */
    if(betweenSeeks(pos, active, next))
        return next;

    printf("%d %d : %d\n", modularIndex(cur,-1), modularIndex(cur,-0), betweenSeeks(pos, prev, active));
    printf("%d %d : %d\n", modularIndex(cur,0), modularIndex(cur,1), betweenSeeks(pos, active, next));

    return active;

}
