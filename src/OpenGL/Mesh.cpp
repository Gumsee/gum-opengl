#include "Mesh.h"


Mesh::Mesh() 
{
    offsetMatrix = mat4(1);
    
    hasUVs = false;
    hasNormals = false;
    iMatIndex = -1;
    name = "";
};


void Mesh::addVertex(const Vertex& vertex)
{
    this->vVertices.push_back(vertex);
}

void Mesh::addIndex(const unsigned int& index)
{
    this->vIndices.push_back(index);
}

void Mesh::addTriangle(const int& a, const int& b, const int& c)
{
    vIndices.push_back(a);
    vIndices.push_back(b);
    vIndices.push_back(c);
}

void Mesh::addQuad(const int& a, const int& b, const int& c, const int& d)
{
    vIndices.push_back(a);
    vIndices.push_back(b);
    vIndices.push_back(c);
    vIndices.push_back(a);
    vIndices.push_back(c);
    vIndices.push_back(d);
}



void Mesh::addMesh(Mesh *mesh)
{
    int IndexOffset = this->vVertices.size();
    for(size_t i = 0; i < mesh->vVertices.size(); i++)
    {
        this->vVertices.push_back(mesh->vVertices[i]);
    }
    for(size_t i = 0; i < mesh->vIndices.size(); i++)
    {
        this->vIndices.push_back(mesh->vIndices[i] + IndexOffset);
    }
}


Mesh* Mesh::generateUVSphere(const int& parallels, const int& meridians)
{
    Mesh* mesh = new Mesh();
    mesh->vVertices.push_back(Vertex(vec3(0.0f, 1.0f, 0.0f), vec2(0,0), vec3(0,1,0)));
	for (int j = 0; j < parallels - 1; ++j)
	{
		double const polar = M_PI * double(j+1) / double(parallels);
		double const sp = std::sin(polar);
		double const cp = std::cos(polar);
		for (int i = 0; i < meridians; ++i)
		{
			double const azimuth = 2.0 * M_PI * double(i) / double(meridians);
			double const sa = std::sin(azimuth);
			double const ca = std::cos(azimuth);
			double const x = sp * ca;
			double const y = cp;
			double const z = sp * sa;
			mesh->vVertices.push_back(Vertex(vec3(x, y, z), vec2(0,0), vec3(x,y,z)));
		}
	}
	mesh->vVertices.push_back(Vertex(vec3(0.0f, -1.0f, 0.0f), vec2(0,0), vec3(0,-1,0)));

	for (int i = 0; i < meridians; ++i)
	{
		int const a = i + 1;
		int const b = (i + 1) % meridians + 1;
		mesh->addTriangle(0, b, a);
	}

	for (int j = 0; j < parallels - 2; ++j)
	{
		int aStart = j * meridians + 1;
		int bStart = (j + 1) * meridians + 1;
		for (int i = 0; i < meridians; ++i)
		{
			const int a = aStart + i;
			const int a1 = aStart + (i + 1) % meridians;
			const int b = bStart + i;
			const int b1 = bStart + (i + 1) % meridians;
			mesh->addQuad(a, a1, b1, b);
		}
	}

	for (int i = 0; i < meridians; ++i)
	{
		int const a = i + meridians * (parallels - 2) + 1;
		int const b = (i + 1) % meridians + meridians * (parallels - 2) + 1;
		mesh->addTriangle(mesh->vVertices.size() - 1, a, b);
	}

    return mesh;
}

Mesh* Mesh::generateCube(const vec3& dimensions)
{
    Mesh* mesh = new Mesh();
    mesh->vVertices = { 
        Vertex(vec3(-dimensions.x,  dimensions.y,  dimensions.z), vec2(1, 1), vec3( 0,  0,  1), vec3(-1, 0, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x, -dimensions.y,  dimensions.z), vec2(0, 0), vec3( 0,  0,  1), vec3(-1, 0, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x,  dimensions.y,  dimensions.z), vec2(0, 1), vec3( 0,  0,  1), vec3(-1, 0, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x, -dimensions.y,  dimensions.z), vec2(0, 0), vec3( 0, -1,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x, -dimensions.y, -dimensions.z), vec2(1, 1), vec3( 0, -1,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x, -dimensions.y, -dimensions.z), vec2(0, 1), vec3( 0, -1,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x, -dimensions.y,  dimensions.z), vec2(1, 0), vec3(-1,  0,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x,  dimensions.y, -dimensions.z), vec2(0, 1), vec3(-1,  0,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x, -dimensions.y, -dimensions.z), vec2(1, 1), vec3(-1,  0,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x,  dimensions.y, -dimensions.z), vec2(1, 1), vec3( 0,  0, -1), vec3( 1, 0, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x, -dimensions.y, -dimensions.z), vec2(0, 0), vec3( 0,  0, -1), vec3( 1, 0, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x,  dimensions.y, -dimensions.z), vec2(0, 1), vec3( 0,  0, -1), vec3( 1, 0, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x,  dimensions.y,  dimensions.z), vec2(0, 0), vec3( 1,  0,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x, -dimensions.y, -dimensions.z), vec2(1, 1), vec3( 1,  0,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x,  dimensions.y, -dimensions.z), vec2(0, 1), vec3( 1,  0,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x,  dimensions.y,  dimensions.z), vec2(1, 0), vec3( 0,  1,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x,  dimensions.y, -dimensions.z), vec2(0, 1), vec3( 0,  1,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x,  dimensions.y, -dimensions.z), vec2(1, 1), vec3( 0,  1,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x, -dimensions.y,  dimensions.z), vec2(1, 0), vec3( 0,  0,  1), vec3(-1, 0, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x, -dimensions.y,  dimensions.z), vec2(1, 0), vec3( 0, -1,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x,  dimensions.y,  dimensions.z), vec2(0, 0), vec3(-1,  0,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x, -dimensions.y, -dimensions.z), vec2(1, 0), vec3( 0,  0, -1), vec3( 1, 0, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x, -dimensions.y,  dimensions.z), vec2(1, 0), vec3( 1,  0,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x,  dimensions.y,  dimensions.z), vec2(0, 0), vec3( 0,  1,  0), vec3( 0, 0, 1), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0)
    };
    mesh->vIndices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 18, 1, 3, 19, 4, 6, 20, 7, 9, 21, 10, 12, 22, 13, 15, 23, 16};


	return mesh;
}


Mesh* Mesh::generatePlane(const vec2& dimensions)
{
    Mesh* mesh = new Mesh();
    mesh->vVertices = { 
        Vertex(vec3( dimensions.x, 0.0f,  dimensions.y), vec2(1, 1), vec3( 0,  1,  0), vec3( 0, 1, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3( dimensions.x, 0.0f, -dimensions.y), vec2(1, 0), vec3( 0,  1,  0), vec3( 0, 1, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x, 0.0f,  dimensions.y), vec2(0, 1), vec3( 0,  1,  0), vec3( 0, 1, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0), 
        Vertex(vec3(-dimensions.x, 0.0f, -dimensions.y), vec2(0, 0), vec3( 0,  1,  0), vec3( 0, 1, 0), vec3(0, 0, 0), ivec3(0, 0, 0), vec3(0, 0, 0), 0)
    };
    mesh->vIndices = {0, 1, 2, 1, 3, 2};

	return mesh;
}


//Getters
unsigned int Mesh::numVertices() const                       { return this->vVertices.size(); }
unsigned int Mesh::numIndices() const                        { return this->vIndices.size(); }
Vertex Mesh::getVertex(const unsigned int& index) const      { return this->vVertices[index]; }
Vertex* Mesh::getVertexPtr(const unsigned int& index)        { return &this->vVertices[index]; }
unsigned int Mesh::getIndex(const unsigned int& index) const { return this->vIndices[index]; }
std::vector<Vertex> Mesh::getVertexBuffer() const            { return this->vVertices; }
std::vector<unsigned int> Mesh::getIndexBuffer() const       { return this->vIndices; }