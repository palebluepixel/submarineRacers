#include "CollisionUtils.hxx"

DistanceResult shortestDistance(vec3 p1, vec3 p2) {
	DistanceResult res;
	res.a = p1;
	res.b = p2;
	res.distance = length(p1-p2);

	return res;
}

// See https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Vector_formulation for math
DistanceResult shortestDistance(vec3 p, Segment l) {
	vec3 a = l.a;
	vec3 b = l.b;
	vec3 n = normalize(b - a);

	//Closest point
	vec3 c = a - (dot((a-p), n) * n);

	vec3 p2;

	//Check if c in line segment from a to b.
	//TODO could be much more efficient, this is just quick
	if(length(c-a) < length(b-a)) {
		if(length(c-b) < length(b-a)) {
			p2 = c;
		} else {
			p2 = a;
		}
	} else {
		p2 = b;
	}

	return shortestDistance(p, p2);
}

//TODO
DistanceResult shortestDistance(Segment l1, Segment l2){}
//DistanceResult shortestDistance(vec3 p, Plane l);
//DistanceResult shortestDistance(Segment l, Plane p);