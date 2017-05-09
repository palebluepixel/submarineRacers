#ifndef _COLLISION_UTILS_HXX_
#define _COLLISION_UTILS_HXX_

#include <glm/glm.hpp>

using namespace glm;

struct Segment {
    vec3 a;
    vec3 b;
};

struct DistanceResult {
    vec3 a;
    vec3 b;
    float distance;
};

class Polygon{
public:
    Polygon(vec3 *pts, int n);
    Polygon(vec2 *pts, int n);
    bool pointInPolygon(vec3 pt);
    DistanceResult distance(vec3 pt);
    DistanceResult distance(Segment line);
    ~Polygon();
    vec2 *points;
    vec3 *points3;
    int n;

    // normalized coordinate vectors to polygon-space.
    mat4 orient;
    vec3 locus;
    vec3 x;
    vec3 z;
    vec3 normal;
};

DistanceResult shortestDistance(vec3 p1, vec3 p2);
DistanceResult shortestDistance(vec3 p, Segment l);
DistanceResult shortestDistance(Segment l1, Segment l2);

#endif