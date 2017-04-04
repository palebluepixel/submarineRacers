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

#endif