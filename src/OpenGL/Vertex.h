#pragma once
#include <gum-maths.h>

struct Vertex
{
    vec3 position;
    vec2 textureCoord;
    vec3 normal, tangent, bitangent;

    //Skeletal (bone info)
    ivec3 JointIDs;
    vec3 Weights;
    int BaseVertex;


    Vertex() 
    {
        this->position = vec3(0,0,0);
        this->textureCoord = vec2(0,0);
        this->normal = vec3(0,0,0);
        this->tangent = vec3(0,0,0);
        this->bitangent = vec3(0,0,0);
        this->JointIDs = vec3(0,0,0);
        this->Weights = vec3(0,0,0);
        this->BaseVertex = 0;
    }

    
    Vertex(vec3 pos, vec2 textureCoord, vec3 normal, vec3 tangent, vec3 bitangent, ivec3 jointIDs, vec3 weights, int basevertex)
    {
        this->position = pos;
        this->textureCoord = textureCoord;
        this->normal = normal;
        this->tangent = tangent;
        this->bitangent = bitangent;
        this->JointIDs = jointIDs;
        this->Weights = weights;
        this->BaseVertex = basevertex;
    }
    

    Vertex(vec3 pos, vec2 textureCoord, vec3 normal, vec3 tangent) 
    {
        this->position = pos;
        this->textureCoord = textureCoord;
        this->normal = normal;
        this->tangent = tangent;
        this->bitangent = vec3(0,0,0);
        this->JointIDs = vec3(0,0,0);
        this->Weights = vec3(0,0,0);
        this->BaseVertex = 0;
    }


    Vertex(vec3 pos, vec2 textureCoord, vec3 normal) 
    {
        this->position = pos;
        this->textureCoord = textureCoord;
        this->normal = normal;
        this->tangent = vec3(0,0,0);
        this->bitangent = vec3(0,0,0);
        this->JointIDs = vec3(0,0,0);
        this->Weights = vec3(0,0,0);
        this->BaseVertex = 0;
    }


    std::string toString(std::string vec3Prefix = "", std::string vec2Prefix = "")
    {
        return "Vertex(" + 
            position.toString(vec3Prefix) + ", " +
            textureCoord.toString(vec2Prefix)         + ", " +
            normal.toString(vec3Prefix)               + ", " +
            tangent.toString(vec3Prefix)              + ", " +
            bitangent.toString(vec3Prefix)            + ", " +
            JointIDs.toString("i" + vec3Prefix)       + ", " +
            Weights.toString(vec3Prefix)              + ", " +
            std::to_string(BaseVertex)                +
        ")";
    }
};