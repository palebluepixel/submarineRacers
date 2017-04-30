#ifndef _PHYSICSENGINE_HXX_
#define _PHYSICSENGINE_HXX_

#include <ent/Entity.hxx>
#include <vector>

class PhysicsEngine{
public:
  std::vector<Entity*> entities;

  void update(double elapsed);

};


#endif