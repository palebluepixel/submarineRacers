#ifndef _COLLISION_UTILS_HXX_
#define _COLLISION_UTILS_HXX_

#include <glm/glm.hpp>

using namespace glm;

struct Segment {
	vec3 a;
	vec3 b;
};

//TODO struct Plane

struct DistanceResult {
	vec3 a;
	vec3 b;
	float distance;
};

DistanceResult shortestDistance(vec3 p1, vec3 p2);
DistanceResult shortestDistance(vec3 p, Segment l);
DistanceResult shortestDistance(Segment l1, Segment l2);
//DistanceResult shortestDistance(vec3 p, Plane l);
//DistanceResult shortestDistance(Segment l, Plane p);

#endif