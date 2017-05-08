#include "Checkpoint.hxx"

vec3 hexCenter(hexagon hex)
{
	return (hex.Lt + hex.Lb + hex.Mt + hex.Mb + hex.Rt + hex.Rb) / 6.0f;
}

vec3 displace(vec3 point, vec3 axis, float displace)
{
	vec3 result = point + (axis * displace);
	return result;
}

Mesh * hexMesh(hexagon hex)
{
	/* Get the normal for the first triangle of the hex */
	vec3 center = hexCenter(hex);;
	vec3 v1 = normalize(hex.Lt - center);
	vec3 v2 = normalize(hex.Mt - center);
	vec3 normal = normalize(cross(v1, v2));

	float width = 0.2;

	/* Iinitalize Vertex and Index array for flat hex */
	int nVecs = 14;
	vec3 *v = (vec3*)malloc(nVecs*sizeof(vec3));
	v[0] = displace(center, normal, width);
	v[1] = displace(hex.Lt, normal, width);
	v[2] = displace(hex.Mt, normal, width);
	v[3] = displace(hex.Rt, normal, width);
	v[4] = displace(hex.Rb, normal, width);
	v[5] = displace(hex.Mb, normal, width);
	v[6] = displace(hex.Lb, normal, width);
	v[7] = displace(center, normal, -width);
	v[8] = displace(hex.Lt, normal, -width);
	v[9] = displace(hex.Mt, normal, -width);
	v[10] = displace(hex.Rt, normal, -width);
	v[11] = displace(hex.Rb, normal, -width);
	v[12] = displace(hex.Mb, normal, -width);
	v[13] = displace(hex.Lb, normal, -width);

	int j, nIndices = 96;
	uint32_t stack_i[nIndices] = 
	{2,1,0,  3,2,0,   4,3,0,   5,4,0,   6,5,0,   1,6,0,  //front face
	 7,8,9,  7,9,10,  7,10,11, 7,11,12, 7,12,13, 7,13,8, //back face
	 1,9,2,  2,9,10,  2,10,3,  3,10,11, 3,11,4,  4,11,12, //sides
	 4,12,5, 5,12,6,  6,12,13, 6,13,8,  6,8,1,   1,8,9};

	uint32_t *i = (uint32_t*)malloc(nIndices*sizeof(uint32_t));
	for(j=0; j<nIndices; j++)
		i[j] = stack_i[j];

	// Create the mesh
	Mesh *m = new Mesh(GL_TRIANGLES);
    m->loadVertices (nVecs, v);
    m->loadIndices (nIndices, i);

    return m;
}


Checkpoint::Checkpoint(int ID, vec3 initial_position, quaternion initial_orientation,
        std::string name, EntityType type, EntityStatus status, float tick_interval)
:Entity(ID, initial_position, initial_orientation, name, type, status, tick_interval)
{ }

Checkpoint::~Checkpoint() { }


FinishLine::FinishLine(int ID, vec3 initial_position, quaternion initial_orientation,
        std::string name, EntityType type, EntityStatus status, float tick_interval)
:Checkpoint(ID, initial_position, initial_orientation, name, type, status, tick_interval)
{ }

FinishLine::~FinishLine() { }


SeekPoint::SeekPoint(int ID, vec3 initial_position, quaternion initial_orientation,
	    std::string name, EntityType type, EntityStatus status, float tick_interval,
	    hexagon hex, int mandatory)
:Entity(ID, initial_position, initial_orientation, name, type, status, tick_interval)
{
	this->mandatory = mandatory;

	/* TODO: check if hex is valid */
	this->hex = hex;

	this->initalizeVisualData();
}

SeekPoint::~SeekPoint() {};


void SeekPoint::initalizeVisualData(){
	this->initalizeTextures("../assets/textures/cubetex.png");
	this->initalizeMeshes();
}

void SeekPoint::initalizeMeshes()
{
    Mesh *mesh = hexMesh(this->hex);
    //Mesh *mesh = new Mesh(GL_TRIANGLES);
    //mesh->loadOBJ("../assets/models/cube.obj");
    vec3 col = vec3(1.0f, 0.5f, 0.5f);
    mesh->data.color = vec4(col, 1.0); // transparent pink
    mesh->data.tex = this->tex;
    this->meshes.push_back(TransformedMesh(TransformedMesh::MeshInfo(mesh,mat4())));
}

void SeekPoint::initalizeTextures(const char* texfile)
{
	//load texture
    image2d * image = new image2d(texfile, true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}