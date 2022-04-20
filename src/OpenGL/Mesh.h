#pragma once
#include "Vertex.h"
#include <vector>
#include <string>


class Mesh
{
private:
    std::vector<Vertex> vVertices;
    std::vector<unsigned int> vIndices;

public:
    Mesh();
    ~Mesh() {};

    mat4 offsetMatrix; //For scene imports
	bool hasUVs;
	bool hasNormals;
	int iMatIndex;
    std::string name;

    void addVertex(const Vertex& vertex);
    void addIndex(const unsigned int& index);
    void addTriangle(const int& a, const int& b, const int& c);
    void addQuad(const int& a, const int& b, const int& c, const int& d);
    void addMesh(Mesh *mesh);

    //Static methods
    static Mesh* generateUVSphere(const int& parallels, const int& meridians);
    static Mesh* generateCube(const vec3& dimensions);
    static Mesh* generatePlane(const  vec2& dimensions);


    //Getter
    unsigned int numVertices() const;
    unsigned int numIndices() const;
    Vertex getVertex(const unsigned int& index) const;
    Vertex* getVertexPtr(const unsigned int& index);
    unsigned int getIndex(const unsigned int& index) const;
    std::vector<Vertex> getVertexBuffer() const;
    std::vector<unsigned int> getIndexBuffer() const;
};