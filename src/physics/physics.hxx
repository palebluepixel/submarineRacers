#ifndef PHYSICS_HXX
#define PHYSICS_HXX

#include <glm/glm.hpp>

namespace Space{
    using namespace glm;
    /**
     *  a Position object represents everything that there
     *  is to know about the position of an object in our
     *  submarine world: ie. depth (integer), 3D position
     *  in space, 3D orientation, etc...
     */
    struct Pos{
        vec3 pos;
        mat3 orient;
        int depth;
    };
    
    /**
     *  abstract class Volume object representing an oriented
     *  3D volume in space.
     *
     *
     */
    class Volume{

        Pos pos;    // where is this object in space?

        virtual inline const char *type() =0;
        /**
         *  returns shortest distance between this volume
         *  and another (ie. the distance between the 
         *  closest points on this volume and on the other)
         */
        virtual double distance(Volume *other) =0;
        /**
         *  returns the shortest vector amount by which this
         *  object must be pushed so that it no longer
         *  intersects with other.
         */
        virtual vec3 push(Volume *other) =0;
        /**
         *  returns the shortest vector amount ALONG the direction
         *  'direction' by which this object must be pushed so that
         *  it no longer intersects with other. This is useful for
         *  collision detection calls which take into account
         *  velocity.
         */
        virtual vec3 pushAlong(Volume *other, vec3 direction) =0;
        /**
         *  test collision between objects
         */
        virtual bool collision(Volume *other) =0;
    };
    class CylinderVolume : public Volume{
        inline const char *type(){ return "cylinder"; }
        CylinderVolume(vec3 p1, vec3 p2);
        vec3 p1,p2;
    };
    class SphereVolume : public Volume{
        inline const char *type(){ return "sphere"; }
        SphereVolume(vec3 pp, vec3 rad);
        vec3 p;
        double r;
    };
    class HeightmapVolume : public Volume{
        inline const char *type(){ return "heightmap"; }
        HeightmapVolume(vec3 pp, int width, int height);
        vec3 p;
        int width;
        int height;
    };
    class PhysicsEngine{
        // void test_collision(Collidable *a, Collidable *b);
    };
}

#endif