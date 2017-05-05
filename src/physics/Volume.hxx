
#ifndef PHYSICS_HXX
#define PHYSICS_HXX

#include <glm/glm.hpp>
#include <graphics/mesh.hxx>
#include <ent/Entity.hxx>
#include <vector>
#include "CollisionUtils.hxx"


namespace Space{

};

typedef glm::tquat<float> quaternion;
using namespace glm;
/**
 *  a Position object represents everything that there
 *  is to know about the position of an object in our
 *  submarine world: ie. depth (integer), 3D position
 *  in space, 3D orientation, etc...
 */
class Entity;


/**
 *  abstract class Volume object representing an oriented
 *  3D volume in space.
 *
 *
 */
class Volume{
protected:
public:
    class Pos{
    public:
        Pos(Entity *in);
        void set(Entity *in);
        mat4 transform;
        mat4 transformInv;
        bool transformInv_calc;
        vec3 pos;
        quaternion ori;
        float depth;
    };
    Volume(Pos ps);
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

    virtual bool containsPoint(vec3 pt) =0;

    virtual TransformedMesh collisionMesh() = 0;
};


class SphereVolume : public Volume{
public:
    SphereVolume(Pos pos, double rad);
    const char *type();
    double distance(Volume *other);

    vec3 push(Volume *other);
    vec3 pushAlong(Volume *other, vec3 direction);
    bool collision(Volume *other);
    bool containsPoint(vec3 pt);

    float R();

    TransformedMesh collisionMesh();
    static Mesh* mesh;
protected:
    double r;
};

class CylinderVolume : public Volume{
public:
    CylinderVolume(Pos pos, double rad, double height, glm::mat4 rotation);
    const char *type();
    double distance(Volume *other);

    vec3 push(Volume *other);
    vec3 pushAlong(Volume *other, vec3 direction);
    bool collision(Volume *other);
    bool containsPoint(vec3 pt);
    float R();

    TransformedMesh collisionMesh();
    static Mesh* meshcyl;
    static Mesh* meshcap;
protected:
    double r,h;
    glm::mat4 rotation;
};

class HeightmapVolume : public Volume{
public:
    HeightmapVolume(Pos pos, mat4 scale, int width, int height, float *data);
    const char *type();

    double distance(Volume *other);

    vec3 push(Volume *other);
    vec3 pushAlong(Volume *other, vec3 direction);
    bool collision(Volume *other);
    bool containsPoint(vec3 pt);

    TransformedMesh collisionMesh();
protected:
    int width;
    int height;
    mat4 scale;
    mat4 scaleInv;
    float *data;
};

class UnionVolume : public Volume{
    UnionVolume(std::vector<Volume> volumes);
    std::vector<Volume> volumes;
};

#endif