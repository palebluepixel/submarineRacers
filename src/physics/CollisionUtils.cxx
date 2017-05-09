#include "CollisionUtils.hxx"
#include "stdio.h"
#include <util/log.hxx>
#include <limits>

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
        if(la>lb)return B;
        else return A;
    }
}

bool parallel(vec3 a, vec3 b) {
    /* Determine if parallel using three divisions,
     * two subtractions, and twelve boolean operators.
     */
    float f1 = b.x/a.x;
    float f2 = b.y/a.y;
    float f3 = b.z/a.z;

    // all three ratios are 0/0
    if(f1!=f1 && f2!=f2 && f3!=f3)return true;

    return fabsf(f1-f2)<0.0001f && fabsf(f2-f3)<0.0001f;

    /* Uses two sqrts, seven multiplications, one
     * division, four addditions, and one boolean op.
     */
    // return a * (length(b) / length(a)) == b;
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

    // logln(LOGHIGH,"result: %s\n",stat==IN?"in":stat==A?"A":"B");

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


Polygon::Polygon(vec3 *pts, int n) : n(n){
    points = new vec2[n];
    points3 = new vec3[n];
    vec3 avg;
    for(int i=0;i<n;++i){
        points3[i]=pts[i];
        avg+=pts[i];
    }
    avg/=float(n);
    locus=avg;
    normal = normalize(cross(pts[1]-pts[0],pts[2]-pts[0]));
    x=cross(normal,vec3(1,0,0));
    if(length(x)<0.001){
        x=cross(normal,vec3(0,1,0));
    }
    z=cross(normal,x);
    for(int i=0;i<n;++i){
        points[i]=vec2(dot(pts[i]-locus,x),dot(pts[i]-locus,z));
    }
}
bool Polygon::pointInPolygon(vec3 pt){
    // ray casting algorithm to test if
    // point in polygon. refer to
    // https://en.wikipedia.org/wiki/Point_in_polygon
    // https://en.wikipedia.org/wiki/Even%E2%80%93odd_rule

    // 1. reduce to 2D coordinates.
    vec3 proj = (pt-locus) - dot(pt-locus,normal)*normal;
    vec2 p(dot(proj,x),dot(proj,z));

    fprintf(stderr,"testing: (%.3f,%.3f,%.3f)\n",proj.x,proj.y,proj.z);
    fprintf(stderr,"testing: (%.3f,%.3f)\n",p.x,p.y);

    // 2. intersect with ray {(0,0) along positive x axis}.
    int ray_int =0;
    for(int i=0;i<n;i++){
        vec2 a = points[i]-p;
        vec2 b = i==n?points[0]:points[i+1]-p;

        // test ray intersection.
        if((a.y<0 && b.y>0) || (a.y>0 && b.y<0)){
            float zero = a.x+(b.x-a.x)*(-a.y/(b.y-a.y));
            if(zero>0){
                fprintf(stderr,"(%.3f,%.3f),(%.3f,%.3f)\n",a.x,a.y,b.x,b.y);
                ++ray_int;
            }
        }
    }

    // return true if odd number of intersections.
    return ray_int && (ray_int % 2);
}
DistanceResult Polygon::distance(vec3 pt){
    if(pointInPolygon(pt)){
        return DistanceResult{pt,(pt-locus) - (dot(pt-locus,normal)*normal),dot(pt-locus,normal)};
    }
    else{
        DistanceResult mindr;
        float mindist = std::numeric_limits<float>::infinity();

        // check distance against each segment of edge.
        for(int i=0;i<n;i++){
            vec3 a = points3[i];
            vec3 b = i==n?points3[0]:points3[i+1];
            DistanceResult dr = shortestDistance(pt,Segment{a,b});
            if(dr.distance<mindist){
                mindist = dr.distance;
                mindr=dr;
            }
        }
        return mindr;
    }
}
DistanceResult Polygon::distance(Segment line){
    if(pointInPolygon(line.a) && pointInPolygon(line.b)){
        // both points in polygon. test segment endpoints.
        float distA = dot(line.a-locus,normal);
        float distB = dot(line.b-locus,normal);

        // check if line intersects plane.
        bool intersection = (distA>0 && distB<0) || (distA<0 && distB>0);
        float mul = (intersection)?(-1.f):(1.f);
        if(fabsf(distA)<fabsf(distB)){
            // line.a is closer to the plane than line.b
            return DistanceResult{line.a,(line.a-normal*distA),distA*mul};
        }else{
            // line.b is closer to the plane than line.a
            return DistanceResult{line.b,(line.b-normal*distB),distB*mul};
        }
    }
    else{
        // otherwise, return minimum distance from all segments.
        DistanceResult mindr;
        float mindist = std::numeric_limits<float>::infinity();

        // check distance against each segment of edge.
        for(int i=0;i<n;i++){
            vec3 a = points3[i];
            vec3 b = i==n?points3[0]:points3[i+1];
            DistanceResult dr = shortestDistance(line,Segment{a,b});
            if(dr.distance<mindist){
                mindist = dr.distance;
                mindr=dr;
            }
        }
        return mindr;
    }
}
Polygon::~Polygon(){
    delete points;
}