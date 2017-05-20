#include "CollisionUtils.hxx"
#include "stdio.h"
#include <util/log.hxx>
#include <limits>
#include <glm/gtc/type_ptr.hpp>

enum PointStatus { IN, A, B };

PointStatus insideSegment(vec3 pt, Segment l) {
    //Check if c in line segment from a to b, (assuming c is on the infinite line created by these points).
    //Since we know c is on the line, we can just check the x position unless the line is paralell to the x axis.
    //Should be very fast

    float p;
    float la;
    float lb;
    if(fabsf(l.a.x - l.b.x) > 0.000001f) {
        p = pt.x;
        la = l.a.x;
        lb = l.b.x;
    } else if(fabsf(l.a.y - l.b.y) > 0.000001f) {
        p = pt.y;
        la = l.a.y;
        lb = l.b.y;
    } else if(fabsf(l.a.z - l.b.z) > 0.000001f){
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

    return (fabsf(f1-f2)<0.000001f || f1!=f1 || f2!=f2) && (fabsf(f2-f3)<0.000001f || f2!=f2 || f3!=f3);

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
    vec3 t;
    DistanceResult best = shortestDistance(l1.a, l2);
    DistanceResult bl2 = shortestDistance(l1.b, l2);
    DistanceResult al1 = shortestDistance(l2.a, l1);
    t=al1.a; al1.a=al1.b; al1.b=t;
    DistanceResult bl1 = shortestDistance(l2.b, l1);
    t=bl1.a; bl1.a=bl1.b; bl1.b=t;


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
    points3t = new vec3[n];
    vec3 avg;
    for(int i=0;i<n;++i){
        points3[i]=pts[i];
        points3t[i]=pts[i];
    }
    recalculate();
}
static void printmatrix(char * tag, mat4 in){
    fprintf(stderr,"%s:\n"
        "%.3f %.3f %.3f %.3f\n"
        "%.3f %.3f %.3f %.3f\n"
        "%.3f %.3f %.3f %.3f\n"
        "%.3f %.3f %.3f %.3f\n\n", tag,

        in[0][0],in[1][0],in[2][0],in[3][0],
        in[0][1],in[1][1],in[2][1],in[3][1],
        in[0][2],in[1][2],in[2][2],in[3][2],
        in[0][3],in[1][3],in[2][3],in[3][3]);
}
void Polygon::transform(mat4 in){
    // todo: verify order of multiplication
    orient=in*base;
    x = orient[0];
    normal = orient[1];
    z = orient[2];
    locus = orient[3];
    // printmatrix("base",base);
    // printmatrix("in",in);
    // printmatrix("orient",orient);
    for(int i=0;i<n;++i){
        vec4 t(points3[i],1);
        t=in*t;
        points3t[i]=vec3(t.x,t.y,t.z);
        points[i]= vec2(dot(points3t[i]-locus,x),dot(points3t[i]-locus,z));
        // fprintf(stderr,"v[%d]:\n",i);
        // fprintf(stderr,"  p : (%.3f,%.3f,%.3f)\n",points3[i].x,points3[i].y,points3[i].z);
        // fprintf(stderr,"  t : (%.3f,%.3f,%.3f)\n",points3t[i].x,points3t[i].y,points3t[i].z);
        // fprintf(stderr,"  o : (%.3f,%.3f)\n",points[i].x,points[i].y);
    }
    // fprintf(stderr,"points: ");
    // for(int i=0;i<n;++i)fprintf(stderr,"(%.3f,%.3f) ",points[i].x,points[i].y);
    // fprintf(stderr,"\n");
    // fprintf(stderr,
    //     "transform to\n  x=(%.3f, %.3f, %.3f)"
    //                 "\n  n=(%.3f, %.3f, %.3f)"
    //                 "\n  z=(%.3f, %.3f, %.3f)"
    //                 "\n  l=(%.3f, %.3f, %.3f)\n",x[0],x[1],x[2],normal[0],normal[1],normal[2],z[0],z[1],z[2],locus[0],locus[1],locus[2]);

}
mat4 Polygon::transform(){
    return orient;
}
void Polygon::recalculate(){
    // fprintf(stderr," > Recalculating\n");
    vec3 avg;
    for(int i=0;i<n;++i){
        points3t[i]=points3[i];
        avg+=points3[i];
    }
    avg/=float(n);
    locus=avg;
    vec3 v1 = points3[1]-points3[0];
    for(int i=2;i<n;++i){
        vec3 v2 = points3[i]-points3[0];
        if(!parallel(v1,v2)){
            normal = normalize(cross(v1,v2));
            break;
        }
    }
    x=cross(normal,vec3(1,0,0));
    if(length(x)<0.001){
        x=cross(normal,vec3(0,1,0));
    }
    z=cross(normal,x);
    for(int i=0;i<n;++i){
        points[i]=vec2(dot(points3[i]-locus,x),dot(points3[i]-locus,z));
    }

    float matr[16]=
        {x[0],x[1],x[2],0,
         normal[0],normal[1],normal[2],0,
         z[0],z[1],z[2],0,
         locus[0],locus[1],locus[2],1
        };
    base = glm::make_mat4(matr);
    orient = base;
//todo:
    /*

    x =
    normal =
    z =
    locus =

    */
}
bool Polygon::pointInPolygon(vec3 pt){


    // ray casting algorithm to test if
    // point in polygon. refer to
    // https://en.wikipedia.org/wiki/Point_in_polygon
    // https://en.wikipedia.org/wiki/Even%E2%80%93odd_rule

    // 1. reduce to 2D coordinates.
    vec3 proj = (pt-locus) - dot(pt-locus,normal)*normal;
    vec2 p(dot(proj,x),dot(proj,z));


    // fprintf(stderr,"testing: (%.3f,%.3f,%.3f)\n",points3[0].x,points3[0].y,points3[0].z);
    // fprintf(stderr,"testing: (%.3f,%.3f,%.3f)\n",points3[1].x,points3[1].y,points3[1].z);
    // fprintf(stderr,"testing: (%.3f,%.3f,%.3f)\n",points3[2].x,points3[2].y,points3[2].z);
    // fprintf(stderr,"testing: (%.3f,%.3f,%.3f)\n",x.x,x.y,x.z);
    // fprintf(stderr,"point: ");
    // for(int i=0;i<n;++i)fprintf(stderr,"(%.3f,%.3f) ",points[i].x,points[i].y);
    // fprintf(stderr,"\n");

    // fprintf(stderr,
    //     "transform to\n  x=(%.3f, %.3f, %.3f)"
    //                 "\n  n=(%.3f, %.3f, %.3f)"
    //                 "\n  z=(%.3f, %.3f, %.3f)"
    //                 "\n  l=(%.3f, %.3f, %.3f)\n",x[0],x[1],x[2],normal[0],normal[1],normal[2],z[0],z[1],z[2],locus[0],locus[1],locus[2]);
    // fprintf(stderr,"testing:  (%.3f,%.3f,%.3f)\n",pt.x,pt.y,pt.z);
    // fprintf(stderr,"project: (%.3f,%.3f,%.3f)\n",proj.x,proj.y,proj.z);
    // fprintf(stderr,"in 2d  : (%.3f,%.3f)\n",p.x,p.y);

    // 2. intersect with ray {p along positive x axis}.
    int ray_int =0;
    for(int i=0;i<n;i++){

        // subtract by p to mkae the math easier. ray now starts at (0,0)
        vec2 a = points[i]-p;
        vec2 b = (i==n-1?points[0]:points[i+1])-p;
        // fprintf(stderr,"Points[%d]: (%.3f,%.3f)\n",i,points[i].x,points[i].y);
        // fprintf(stderr," > with: (%.3f,%.3f)\n",a.x,a.y);
        // fprintf(stderr,"       | (%.3f,%.3f)\n",b.x,b.y);

        // test ray intersection.
        if((a.y<=0 && b.y>0) || (a.y>0 && b.y<=0)){
            float zero = a.x+(b.x-a.x)*(-a.y/(b.y-a.y));
            if(zero>=0){
                // fprintf(stderr,"(%.3f,%.3f),(%.3f,%.3f) INT\n",a.x,a.y,b.x,b.y);
                ++ray_int;
            }
        }
        // fprintf(stderr,"       .  =   %d\n",ray_int);
    }

    // return true if odd number of intersections.
    return ray_int && (ray_int % 2);
}
DistanceResult Polygon::distance(vec3 pt){
    if(pointInPolygon(pt)){
        // fprintf(stderr, "dist1: %.3f\n",fabsf(dot(pt-locus,normal)));
        return DistanceResult{pt,(pt) - (dot(pt,normal)*normal),fabsf(dot(pt-locus,normal))};
    }
    else{
        DistanceResult mindr;
        float mindist = std::numeric_limits<float>::infinity();

        // check distance against each segment of edge.
        for(int i=0;i<n;i++){
            vec3 a = points3t[i];
            vec3 b = i==n-1?points3t[0]:points3t[i+1];
            DistanceResult dr = shortestDistance(pt,Segment{a,b});
            if(dr.distance<mindist){
                mindist = dr.distance;
                mindr=dr;
            }
        }
        // fprintf(stderr, "dist2: %.3f\n",mindist);
        return mindr;
    }
}
DistanceResult Polygon::distance(Segment line){
    // fprintf(stderr,"dist (%.3f,%.3f,%.3f)\n",line.a.x,line.a.y,line.a.z);
    // // fprintf(stderr,"testing (%.3f,%.3f,%.3f) (%.3f,%.3f,%.3f)\n",)
    // return distance(line.a);

    float distA = (dot(line.a-locus,normal));
    float distB = (dot(line.b-locus,normal));

    // line is fully to one side of the plane.
    if(distA>0 && distB>0){
        // fprintf(stderr," case I : >0\n");
        if(distA<distB){
            return distance(line.a);
        }else{
            return distance(line.b);
        }
    }
    if(distA<0 && distB<0){
        // fprintf(stderr," case I : <0\n");
        if(distA<distB){
            return distance(line.b);
        }else{
            return distance(line.a);
        }
    }

    // determine intersection of plane and line (https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection)
    float num = dot((points3t[0]-line.a),normal);
    float den = dot((line.b-line.a),normal);

    if(fabsf(den) <= 0.00000001){
        // fprintf(stderr," case II : fully in plane\n");
        // line is completely contained within the plane.
        return DistanceResult{line.a,line.a,0};
        // the other case (ie. no intersection) is not possible at this point. because
        // we know that the line is not fully to one side of the plane.
    }

    float d = num/den;  // distance.
    vec3 intersection = d*(line.b-line.a) + line.a;
    bool hits_polygon = pointInPolygon(intersection);
    if(hits_polygon){
        // fprintf(stderr," case II : hits polygon\n");
        // intersection point is on the polygon.
        if(fabsf(distA)<fabsf(distB)){
            // line.a is closer to the plane than line.b
            float distA = dot(line.a-locus,normal);
            return DistanceResult{(line.a-normal*distA), line.a,-fabsf(distA)};
        }else{
            // line.b is closer to the plane than line.a
            float distB = dot(line.b-locus,normal);
            return DistanceResult{(line.b-normal*distB),line.b,-fabsf(distB)};
        }
    }else{
        // fprintf(stderr," case II : misses polygon\n");
        // does not intersect polygon. test boundary edges.
        DistanceResult mindr;
        float mindist = std::numeric_limits<float>::infinity();

        // check distance against each segment of edge.
        for(int i=0;i<n;i++){
            vec3 a = points3t[i];
            vec3 b = i==n?points3t[0]:points3t[i+1];
            DistanceResult dr = shortestDistance(line,Segment{a,b});
            if(dr.distance<mindist){
                mindist = dr.distance;
                mindr=dr;
            }
        }
        return DistanceResult{mindr.a,mindr.b,mindist};
    }


    // bool inA = pointInPolygon(line.a);
    // bool inB = pointInPolygon(line.b);
    // if(inA && inB){
    //     fprintf(stderr,"both in poly\n");
    //     // both points in polygon. test segment endpoints.
    //     float distA = dot(line.a-locus,normal);
    //     float distB = dot(line.b-locus,normal);

    //     // check if line intersects plane.
    //     bool intersection = (distA>0 && distB<0) || (distA<0 && distB>0);
    //     float mul = (intersection)?(-1.f):(1.f);
    //     if(fabsf(distA)<fabsf(distB)){
    //         // line.a is closer to the plane than line.b
    //         return DistanceResult{line.a,(line.a-normal*distA),distA*mul};
    //     }else{
    //         // line.b is closer to the plane than line.a
    //         return DistanceResult{line.b,(line.b-normal*distB),distB*mul};
    //     }
    // }
    // else if(!inA && !inB){
    //     fprintf(stderr,"noot in poly\n");
    //     // otherwise, return minimum distance from all segments.
    //     DistanceResult mindr;
    //     float mindist = std::numeric_limits<float>::infinity();

    //     // check distance against each segment of edge.
    //     for(int i=0;i<n;i++){
    //         vec3 a = points3t[i];
    //         vec3 b = i==n?points3t[0]:points3t[i+1];
    //         DistanceResult dr = shortestDistance(line,Segment{a,b});
    //         if(dr.distance<mindist){
    //             mindist = dr.distance;
    //             mindr=dr;
    //         }
    //     }
    //     return mindr;
    // }else{
        
    // }
    //     DistanceResult dr1 = distance(line.a);
    //     if(dr1.distance<mindist){
    //         mindist = dr1.distance;
    //         mindr=dr1;
    //     }
    //     DistanceResult dr2 = distance(line.b);
    //     if(dr2.distance<mindist){
    //         mindist = dr2.distance;
    //         mindr=dr2;
    //     }

    //     return mindr;
    // }
}
Polygon::~Polygon(){
    // delete points;
}