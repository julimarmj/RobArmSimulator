#pragma once

#include <iostream>
#include "GLM/glm.hpp"
#include "OBJLoader.h"

//#include <math.h>
using namespace objl;

struct s_mesh {


    std::vector<std::vector<float>> vmesh;
    std::vector<std::vector<unsigned int>> imesh;
};

class LoadMesh {
public:
    LoadMesh(std::string File) {
        bool loadout = Loader.LoadFile(File);
        init();
    };
     
    ~LoadMesh() {

    };

    s_mesh Lmesh;;

private:
    objl::Loader Loader;
    
    void init() {

        for (int j = 0; j < Loader.LoadedMeshes.size(); j++) {

            std::vector<float> positions;
            for (int i = 0; i < Loader.LoadedMeshes[j].Vertices.size(); i++)
            {
                positions.push_back(Loader.LoadedMeshes[j].Vertices[i].Position.X / 10);
                positions.push_back(Loader.LoadedMeshes[j].Vertices[i].Position.Y / 10);
                positions.push_back(Loader.LoadedMeshes[j].Vertices[i].Position.Z / 10);
                positions.push_back(Loader.LoadedMeshes[j].Vertices[i].Normal.X / 10);
                positions.push_back(Loader.LoadedMeshes[j].Vertices[i].Normal.Y / 10);
                positions.push_back(Loader.LoadedMeshes[j].Vertices[i].Normal.Z / 10);
                positions.push_back(Loader.LoadedMeshes[j].Vertices[i].TextureCoordinate.X);
                positions.push_back(Loader.LoadedMeshes[j].Vertices[i].TextureCoordinate.Y);
                positions.push_back(0.5f);
                positions.push_back(0.5f);
                positions.push_back(0.5f);
            }

            std::vector<unsigned int> indices;
            for (int i = 0; i < Loader.LoadedMeshes[j].Indices.size(); i++)
            {
                indices.push_back(Loader.LoadedMeshes[j].Indices[i]);
            }
            
            Lmesh.vmesh.push_back(positions);
            Lmesh.imesh.push_back(indices);

        }

    };
    void FindMin(glm::vec3& val1, const objl::Vector3& val2) const {
        val1.x = glm::min(val1.x, val2.X);
        val1.y = glm::min(val1.y, val2.Y);
        val1.z = glm::min(val1.z, val2.Z);
    };
    void FindMax(glm::vec3& val1, const objl::Vector3& val2) const {
        val1.x = glm::max(val1.x, val2.X);
        val1.y = glm::max(val1.y, val2.Y);
        val1.z = glm::max(val1.z, val2.Z);
    };
    glm::mat4 RotateMatrix(glm::vec3& thetaD) {
        glm::vec3 theta = glm::radians(thetaD);
        glm::mat3 R_x(1,             0,              0,
                      0, cos(theta[0]), -sin(theta[0]),
                      0, sin(theta[0]),  cos(theta[0]) );

        glm::mat3 R_y(cos(theta[1]), 0, sin(theta[1]),
                                  0, 1,             0,
                      -sin(theta[1]), 0, cos(theta[1]) );

        glm::mat3 R_z(cos(theta[2]), -sin(theta[2]), 0,
                      sin(theta[2]),  cos(theta[2]), 0,
                                  0,              0, 1 );

        glm::mat3 Mat_R = R_z * R_y * R_x;
        return glm::mat4(Mat_R);
    }

public:
    void update() {

    }; 
};
