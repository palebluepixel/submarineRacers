#ifndef _PHYSICS_HXX_
#define _PHYSICS_HXX_
namespace Physics{
  struct CollisionMode{
    int hardness;       // if 0, no collision response. if 1, elastic collision.
  };
  const CollisionMode ELASTIC={1};
  const CollisionMode NONE={0};
}

#endif