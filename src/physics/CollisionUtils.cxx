using "CollisionUtils.hxx"

DistanceResult shortestDistance(vec3 p1, vec3 p2) {
	DistanceResult res;
	res.a = p1;
	res.b = p2;
	res.distance = norm(p1-p2);
}

//TODO
DistanceResult shortestDistance(vec3 p, Segment l) {}
DistanceResult shortestDistance(Segment l1, Segment l2){}
//DistanceResult shortestDistance(vec3 p, Plane l);
//DistanceResult shortestDistance(Segment l, Plane p);