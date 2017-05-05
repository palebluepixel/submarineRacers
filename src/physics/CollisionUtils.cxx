#include "CollisionUtils.hxx"
#include "stdio.h"

enum PointStatus { IN, A, B };

PointStatus insideSegment(vec3 pt, Segment l) {
    //Check if c in line segment from a to b, (assuming c is on the infinite line created by these points).
    //Since we know c is on the line, we can just check the x position unless the line is paralell to the x axis.
    //Should be very fast

    float p;
    float la;
    float lb;
    if(fabsf(l.a.x - l.b.x) > 0.0001f) {
        p = pt.x;
        la = l.a.x;
        lb = l.b.x;
    } else if(fabsf(l.a.y - l.b.y) > 0.0001f) {
        p = pt.y;
        la = l.a.y;
        lb = l.b.y;
    } else if(fabsf(l.a.z - l.b.z) > 0.0001f){
        p = pt.z;
        la = l.a.z;
        lb = l.b.z;
    } else{
        return IN;
    }

    if ((la<p && p<lb) || (lb<p && p<la)){
        return IN;
    }else if(la<=p && lb<=p){
        if(lb>la)return B;
        else return A;
    }else if(la>=p && lb>=p){
        if(la>lb)return A;
        else return B;
    }
}

bool parallel(vec3 a, vec3 b) {
    return a * (length(b) / length(a)) == b;
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

    // fprintf(stderr,"\nshortestDistance (%.3f,%.3f,%.3f)/(%.3f,%.3f,%.3f) <-> (%.3f,%.3f,%.3f)\n",
      // l.a.x,l.a.y,l.a.z,l.b.x,l.b.y,l.b.z,p.x,p.y,p.z);

    //Closest point
    vec3 c = a - (dot((a-p), n) * n);

    // fprintf(stderr,"closest point: (%.3f,%.3f,%.3f)\n",c.x,c.y,c.z);

    vec3 p2;

    //Check if c in line segment from a to b.
    PointStatus stat = insideSegment(c, l);
    // fprintf(stderr,"stat = %s\n",stat==IN?"in":stat==A?"A":"B");
    if(stat == IN) {
        p2 = c;
    } else if(stat == A) {
        p2 = a;
    } else {
        p2 = b;
    }

    return shortestDistance(p, p2);
}

//Math from https://en.wikipedia.org/wiki/Skew_lines#Nearest_Points
DistanceResult shortestDistance(Segment l1, Segment l2) {

    // fprintf(stderr,"\nshortestDistance (%.3f,%.3f,%.3f)/(%.3f,%.3f,%.3f) <-> (%.3f,%.3f,%.3f)/(%.3f,%.3f,%.3f)\n",
    //   l1.a.x,l1.a.y,l1.a.z,l1.b.x,l1.b.y,l1.b.z,
    //   l2.a.x,l2.a.y,l2.a.z,l2.b.x,l2.b.y,l2.b.z);

    vec3 p1 = l1.a;
    vec3 p2 = l2.a;

    vec3 d1 = normalize(l1.b - l1.a);
    vec3 d2 = normalize(l2.b - l2.a);


    // If parallel, perturb one end slightly =D
    if(parallel(d1, d2)) {
        if(d1.x == 0) {
            d1.x += .00001;
        } else {
            d1.y += .00001;
        }
    }

    vec3 n = cross(d1, d2);

    vec3 n1 = cross(d1, n);
    vec3 n2 = cross(d2, n);

    vec3 c1 = p1 + (dot((p2 - p1), n2) / dot(d1, n2)) * d1;
    vec3 c2 = p2 + (dot((p1 - p2), n1) / dot(d2, n1)) * d2;

    // printf("c1: (%.3f,%.3f,%.3f)\n",c1.x,c1.y,c1.z);
    // printf("c2: (%.3f,%.3f,%.3f)\n",c2.x,c2.y,c2.z);

    // If both of these points are inside our line segments, then we are done
    if((insideSegment(c1, l1)==IN) &&
       (insideSegment(c2, l2)==IN)) {
        return shortestDistance(c1, c2);
    }

    // TODO Check point line against each of the four endpoints, returning minimum
    DistanceResult best = shortestDistance(l1.a, l2);
    DistanceResult bl2 = shortestDistance(l1.b, l2);
    DistanceResult al1 = shortestDistance(l2.a, l1);
    DistanceResult bl1 = shortestDistance(l2.b, l1);

    if(bl2.distance < best.distance) {
        best = bl2;
    }
    if(al1.distance < best.distance) {
        best = al1;
    }
    if(bl1.distance < best.distance) {
        best = bl1;
    }

    DistanceResult dr = best;
    // fprintf(stderr,"Result: (%.3f,%.3f,%.3f) (%.3f,%.3f%.3f) : %.3f",dr.a.x,dr.a.y,dr.a.z,dr.b.x,dr.b.y,dr.b.z,dr.distance);
    return best;
}