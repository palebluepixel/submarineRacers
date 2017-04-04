#ifdef I_DONT_WANT_MY_CODE_TO_COMPILE

class Mesh{	
	// camera passed for LOD optimizations
	virtual void render(Camera &camera) = 0;
};

class PolygonMesh : public Mesh{
	
};
class ParticleMesh : public Mesh{
	
};
class BillboardMesh : public Mesh{
	
}
class HeightmapMesh : public Mesh{
		
};
class QuadricMesh : public Mesh{
	
};

namespace Space{
	using namespace glm;
	class Volume{
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
	}
	class SphereVolume : public Volume{
		inline const char *type(){ return "cylinder"; }
		CylinderVolume(vec3 pp, vec3 rad);
		vec3 p;
		double r;
	}
	class PhysicsEngine{
		void test_collision(Collidable *a, Collidable *b){

		}
	}
}
#endif