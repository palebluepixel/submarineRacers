#include "CollisionUtils.hxx"

enum PointStatus { IN, A, B };

bool insideSegment(vec3 p, Segment l) {
    //Check if c in line segment from a to b.
    //TODO could be much more efficient, this is just quick
    if(length(p - l.a) < length(l.b - l.a)) {
        if(length(p - l.b) < length(l.b - l.a)) {
            return IN;
        } else {
            return A;
        }
    } else {
        return B;
    }
}


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

    PointStatus stat = insideSegment(c, l);
    if(stat == IN) {
        p2 = c;
    } else if(stat == A) {
        p2 = a;
    } else {
        p2 = b;
    }

    return shortestDistance(p, p2);
}

//TODO
DistanceResult shortestDistance(Segment l1, Segment l2) {
    // TODO
    /*if(lines are parallel) {
        Captain, we've got a problem
    }*/


    //Math from https://en.wikipedia.org/wiki/Skew_lines#Nearest_Points

    vec3 p1 = l1.a;
    vec3 p2 = l2.a;

    vec3 d1 = l1.b - l1.a;
    vec3 d2 = l2.b - l2.a;

    vec3 n = cross(d1, d2);

    vec3 n1 = cross(d1, n);
    vec3 n2 = cross(d2, n);

    vec3 c1 = p1 + (dot((p2 - p1), n2) / dot(d1, n2)) * d1;
    vec3 c2 = p2 + (dot((p1 - p2), n1) / dot(d2, n1)) * d2;

    // If both of these points are inside our line segments, then we are done
    if(insideSegment(c1, l1) && insideSegment(c2, l2)) {
        return shortestDistance(c1, c2);
    }

    // TODO Check point line against each of the four endpoints, returning minimum
}

//DistanceResult shortestDistance(vec3 p, Plane l);
//DistanceResult shortestDistance(Segment l, Plane p);