#pragma once
#include <vector>
#include <thread>
#include <iostream>
#include <GLFW/glfw3.h>
#include "GLM/glm.hpp"

struct DH_Var {
    float a = 0.0f,
          alf = 0.0f,
          d = 0.0f,
          th = 0.0f,
          b = 0.0f,
          bt = 0.0f;
};

struct joint
{
    DH_Var Dh;
    glm::fmat4 T;
    glm::fvec2 Limits;
    glm::fmat4 DHM_I;
    std::vector<float> AngBuffer;
};

class Kinematic {
private:
    unsigned int m_CountJoint;
    float m_Sence = 0.5f;

public:
    glm::fvec3 EofPos;
    std::vector<joint> Joints;
    std::vector<float> EofPosBuffer;
    std::vector<unsigned int> PosBufferIndex;
    std::vector<std::vector<float>> vEixo;
    const std::vector <unsigned int> iEixo = {3, 0, 3, 1, 3, 2};

    Kinematic();
    ~Kinematic();
    void update();

    //void EofMoveTo(glm::vec3 Pos);
    //glm::mat3 getJacobian(float th1, float th2, float th3);
    void addJont(DH_Var& HD, glm::vec2 Limits);
    std::vector<float> getGLJointMatix(int i);
    glm::mat4 getJointMatix(int Numjoint);
    glm::mat4 getAxes(int Numjoint);
    glm::mat4 HDMatix(DH_Var& HD);

private:
    void checkLimits(int& i);
    glm::mat4 GetRotMatix(std::string axis, const float& O);
    glm::mat4 GetTransMatix(std::string axis, const float& T);
};