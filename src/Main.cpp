// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Window.h"
#include "Camera3D.h"
#include "LoadMesh.h"
#include "kinematics.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

Window window("Rob - The Arm");
Camera3D Camera;
LoadMesh mesh("res/objects/robotic_arm_up.obj");
Kinematic Robot;

int main(void)
{
    {   
        VertexBufferLayout layout;
        layout.AddFloat(3);
        layout.AddFloat(3);
        layout.AddFloat(2);
        layout.AddFloat(3);

        VertexBufferLayout layoutE;
        layoutE.AddFloat(4);
        layoutE.AddFloat(4);

        VertexArray va;
        VertexBuffer vb;
        IndexBuffer ib;

        Shader shader("res/shaders/default.vert","res/shaders/default.frag");
        Shader shaderE("res/shaders/Axes.vert", "res/shaders/Axes.frag");
        Renderer renderer;

        //IMGUI_CHECKVERSION();
        const char* glsl_version = "#version 130";

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window.getGLFWwindow(), true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        glm::vec3 translation(-0.5, -1.5, 0);
        glm::mat4 model = glm::mat4(glm::translate(glm::mat4(1.0f), translation));

        Camera.Position[2] = -30;

        glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));

        static int switchTabs = 0;
        std::vector<bool> ShowE(mesh.Lmesh.vmesh.size(), false);

        float ang[7] = {0,0,0,0,0,0,0};
        glm::vec3 axis(0);

        glm::mat4 DHMat;
        mesh.update();
        glm::vec3 VecEof = Robot.EofPos;

        do {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            window.clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            static bool ShowR = true;
            static bool ShowE[10]{ 1 };
            static glm::vec3 color = { 1,1,1 };
            static std::string btext ="";
            static std::string text = "";

            if (ImGui::Button("base")) {
                switchTabs = 0;
                btext = "Base";

            }
            for (int i = 0; i < mesh.Lmesh.vmesh.size()-1; i++)
            {
                ImGui::SameLine(0.0, 2.0f);
                btext = std::string("Eixo ") + std::to_string(i + 1);
                if (ImGui::Button(btext.data())) {
                    switchTabs = i+1;
                    text = std::string("Eixo ") + std::to_string(i + 1);
                }
            }
            //int ButtonsCount;
            //const unsigned char* Bt = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &ButtonsCount);
            //int b[16] /*= { Bt[0],Bt[1] ,Bt[2] ,Bt[3] ,Bt[4] ,Bt[5] ,Bt[6]
            //    ,Bt[7] ,Bt[8] ,Bt[9] ,Bt[10] ,Bt[11],Bt[12],Bt[13],Bt[14],Bt[15] }*/;
            DHMat = Robot.getJointMatix(switchTabs);

            {
                ImGui::Text("Translcao camera: %.1f ,y: %.1f, z: %.1f", Camera.Position.x, Camera.Position.y, Camera.Position.z);
                ImGui::SliderFloat3("Translcao Robo", &translation.x, -10.0f, 10.0f);
                ImGui::Text("\nVisibilidade");
                ImGui::Checkbox("Exibir robo", &ShowR);
                ImGui::Checkbox("Exibir eixo", &ShowE[switchTabs]);
                ImGui::Text("\n Posicao EoF");
                ImGui::InputFloat3("Eof", &Robot.EofPos.x, "%.3f");
                ImGui::Text("\nParametros %s", text.c_str());
                ImGui::InputFloat("Tx", &Robot.Joints[switchTabs].Dh.a, 0.01f, 1.0f, "%.3f");
                ImGui::InputFloat("Ty", &Robot.Joints[switchTabs].Dh.d, 0.01f, 1.0f, "%.3f");
                ImGui::InputFloat("Tz", &Robot.Joints[switchTabs].Dh.b, 0.01f, 1.0f, "%.3f");
                ImGui::InputFloat("ALF", &Robot.Joints[switchTabs].Dh.alf, 0.1f, 5.0f, "%.1f");
                ImGui::InputFloat("TH", &Robot.Joints[switchTabs].Dh.th, 0.1f, 5.0f, "%.1f");
                ImGui::Text("\nMatriz do %s", text.c_str());
                ImGui::InputFloat4("0", &DHMat[0].x, "%.3f");
                ImGui::InputFloat4("1", &DHMat[1].x, "%.3f");
                ImGui::InputFloat4("2", &DHMat[2].x, "%.3f");
                ImGui::InputFloat4("3", &DHMat[3].x, "%.3f");
                ImGui::Text("\n");
                ImGui::InputFloat4("0", &Robot.Joints[switchTabs].T[0][0], "%.3f");
                ImGui::InputFloat4("1", &Robot.Joints[switchTabs].T[1][0], "%.3f");
                ImGui::InputFloat4("2", &Robot.Joints[switchTabs].T[2][0], "%.3f");
                ImGui::InputFloat4("3", &Robot.Joints[switchTabs].T[3][0], "%.3f");
                //ImGui::Text("\n");
                //ImGui::InputInt4("0", &b[0]);
                //ImGui::InputInt4("1", &b[4]);
                //ImGui::InputInt4("2", &b[8]);
                //ImGui::InputInt4("3", &b[12]);
                //ImGui::InputFloat4("coisa", &color.x, "%.3f");
            }

            // model matrix
            model = glm::translate(glm::mat4(1.0f), translation);

            shader.Use();
            shader.setUniformMat4f("u_proj", window.ProjMat(120.0f, 1.0f, 200.0f));
            shader.setUniformMat4f("u_view", Camera.GetViewMatrix());
            shader.setUniformMat4f("u_model", model);
            shader.setUniform3f("u_viewPos", Camera.Position);
            shader.setUniform3f("u_color", color);
            float AngC = 0;
            for (int i = 0; i < mesh.Lmesh.vmesh.size(); i++) {
                if (ShowR) {
                    AngC += Robot.Joints[i].Dh.alf;
                    glm::mat4 auxDHMat= Robot.getJointMatix(i);
                    shader.setUniformMat4f("u_DHM", auxDHMat);
                    shader.setUniformMat4f("u_DHM_I", Robot.Joints[i].DHM_I);
                    shader.setUniform1f("u_AngC", AngC);
                    vb.AddData(mesh.Lmesh.vmesh[i].data(), mesh.Lmesh.vmesh[i].size() * sizeof(float));
                    ib.AddData(mesh.Lmesh.imesh[i].data(), mesh.Lmesh.imesh[i].size());
                    va.AddBuffer(vb, layout);
                    renderer.Draw(va, ib, shader);
                }
            }
            
            shaderE.Use();
            shaderE.setUniformMat4f("u_proj", window.ProjMat(120.0f, 1.0f, 200.0f));
            shaderE.setUniformMat4f("u_view", Camera.GetViewMatrix());
            shaderE.setUniformMat4f("u_model", model);
            for (int i = 0; i < Robot.vEixo.size(); i++) {
                if (ShowE[i]) {
                    vb.AddData(Robot.vEixo[i].data(), Robot.vEixo[i].size() * sizeof(float));
                    ib.AddData(Robot.iEixo.data(), Robot.iEixo.size());
                    va.AddBuffer(vb, layoutE);
                    renderer.DrawLines(va, ib, shaderE);
                }
            }

            vb.AddData(Robot.EofPosBuffer.data(), Robot.EofPosBuffer.size() * sizeof(float));
            ib.AddData(Robot.PosBufferIndex.data(), Robot.PosBufferIndex.size());
            va.AddBuffer(vb, layoutE);
            renderer.DrawLines(va, ib, shaderE);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            Camera.Update(window);
            window.update();
            mesh.update();
            Robot.update();

        } // Check if the ESC key was pressed or the window was closed
        while (window.closed() == 0);
    }

    // Close OpenGL window and terminate GLFW
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}

