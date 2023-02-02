#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/lab1/lab_camera.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,glm::mat4 &ViewMatrix, const glm::vec3& color = glm::vec3(1));
        void Tema2::RenderScene(float deltaTimeSeconds, glm::mat4 perspective);
        void Tema2::RenderMinimapScene(float deltaTimeSeconds, glm::mat4 perspective);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::vector<glm::vec3> Trees;
        std::vector<float> Treesangle;
        std::vector<std::vector<glm::vec3>> routes;
        std::vector<std::pair<VertexFormat,int>>cars;
        std::vector<float> carangles;
        
        bool renderCameraTarget;
        glm::mat4 projectionMatrix;
        implemented::CameraTema* camera,* camera2;
        float cameraAngle;
        float red = (rand() % 10) / 10.f;
        float green = (rand() % 10) / 10.f;
        float blue = (rand() % 10) / 10.f;
        float carAngle = 0;
        float acceleration;
        int position;
    };
}   // namespace m1