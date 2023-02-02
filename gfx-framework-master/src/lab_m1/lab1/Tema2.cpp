#include "lab_m1/lab1/Tema2.h"
#include "lab_m1/lab1/Shapes.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
    // TODO(student): Never forget to initialize class variables!

}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new implemented::CameraTema();
    camera->Set(glm::vec3(0.9, 0.45, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(glm::radians(15.f), window->props.aspectRatio, 0.01f, 200.0f);
    cameraAngle = 60.f;

    camera2 = new implemented::CameraTema();
    camera2->Set(glm::vec3(0.1, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    Mesh* mesh = new Mesh("plane");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    Shader* shader = new Shader("ShaderTema2");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    Mesh* Track = shapes2D::CreateRaceTrack("Track");
    AddMeshToList(Track);
    
    Trees = shapes2D::GetParralel(5, 10);

    srand(time(NULL));

    for (int i = 0; i < Trees.size(); i++)
    {
        Treesangle.push_back(RADIANS((float)(rand() % 90)));
    }

    for (int i = 0; i <= 3; i++)
    {
        routes.push_back(shapes2D::GetParralel(i, 1));
    }
    for (int i = 0; i < 50; i++)
    {
        int track = rand() % 3;
        int start = rand() % (routes[track].size() - 1);
        float r = (rand() % 10) / 10.f;
        float g = (rand() % 10) / 10.f;
        float b = (rand() % 10) / 10.f;
        cars.push_back({ VertexFormat(routes[track][start],glm::vec3(r,g,b)),start * 10 + track });
        carangles.push_back(0);
    }
    acceleration = 1;

}


void Tema2::FrameStart()
{
}


void Tema2::Update(float deltaTimeSeconds)
{ 
    glm::ivec2 resolution = window->props.resolution;
    glViewport(0, 0, resolution.x, resolution.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projection = glm::perspective(glm::radians(cameraAngle), window->props.aspectRatio, 0.01f, 200.0f);
    RenderScene(deltaTimeSeconds, projection);

    
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(50,resolution.y - 200, resolution.x/5,resolution.y/5 );
    projection = glm::ortho(-5.f, 5.f, -5.f, 5.f, 0.f, 100.f);
    RenderMinimapScene(deltaTimeSeconds, projection);
}


void Tema2::RenderScene(float deltaTimeSeconds, glm::mat4 perspective)
{
    glm::ivec2 resolution = window->props.resolution;
    projectionMatrix = perspective;
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.8f, 1, 1, 1);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0,0.01,0));
    RenderSimpleMesh(meshes["Track"], shaders["ShaderTema2"], modelMatrix, camera->GetViewMatrix(),glm::vec3(0.1f, 0.1f, 0.1f));
    
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
    RenderSimpleMesh(meshes["plane"],shaders["ShaderTema2"], modelMatrix, camera->GetViewMatrix(),glm::vec3(0.6f,0.9f,0.3f));

    //generate Trees
    
    for (int i = 0; i < Trees.size(); i++)
    {   
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, Trees[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1,3, 1));
        modelMatrix = glm::translate(modelMatrix,glm::vec3(0,0.04,0));
        modelMatrix = glm::rotate(modelMatrix, Treesangle[i], glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, camera->GetViewMatrix(),glm::vec3(0.2f, 0, 0));

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, Trees[i]);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.4, 0));
        modelMatrix = glm::rotate(modelMatrix, Treesangle[i], glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix,camera->GetViewMatrix(),glm::vec3(0.3f, 0.5f, 0.3f));
    }

    //generate cars
    for (int i = 0; i < cars.size(); i++)
    { 
        glm::vec3 a, b;
        a = cars[i].first.position;
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, cars[i].first.position);
        modelMatrix = glm::rotate(modelMatrix, carangles[i], glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 1, 1));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.05, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, camera->GetViewMatrix(), cars[i].first.color);
        int track = cars[i].second % 10;
        int start = cars[i].second / 10;
        if (start + 2 >= routes[track].size())
        {
            if (start % 2 == 0)
            {
                cars[i].first.position = routes[track][0];
                cars[i].second = track;
                b = routes[track][0];
            }
            else
            {
                cars[i].first.position = routes[track][1];
                cars[i].second = track + 10;
                b = routes[track][1];
            }
        }
        else
        {
            cars[i].first.position = routes[track][start + 2];
            cars[i].second += 20;
            b = routes[track][start + 2];
        }
        glm::vec3 c = a - b;
        carangles[i] = -atanf(c.z/c.x);

        
    }

    //genarate player

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, carAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 1, 1));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.05, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix,camera->GetViewMatrix(),glm::vec3(red,green,blue));

}



void Tema2::RenderMinimapScene(float deltaTimeSeconds, glm::mat4 perspective)
{
    glm::ivec2 resolution = window->props.resolution;
    projectionMatrix = perspective;  
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.8f, 1, 1, 1);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01, 0));
    RenderSimpleMesh(meshes["Track"], shaders["ShaderTema2"], modelMatrix,camera2->GetViewMatrix(),glm::vec3(0.1f, 0.1f, 0.1f));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
    RenderSimpleMesh(meshes["plane"], shaders["ShaderTema2"], modelMatrix,camera2->GetViewMatrix(),glm::vec3(0.6f, 0.9f, 0.3f));

    //generate Trees

    for (int i = 0; i < Trees.size(); i++)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, Trees[i]);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.4, 0));
        modelMatrix = glm::rotate(modelMatrix, Treesangle[i], glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix,camera2->GetViewMatrix(),glm::vec3(0.3f, 0.5f, 0.3f));
    }

    //generate cars
    for (int i = 0; i < cars.size(); i++)
    {
        glm::vec3 a, b;
        a = cars[i].first.position;
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, cars[i].first.position);
        modelMatrix = glm::rotate(modelMatrix, carangles[i], glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 1, 1));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.05, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix,camera2->GetViewMatrix(), cars[i].first.color);
        
        
    }
 
    //genarate player

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera2->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, carAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 1, 1));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.05, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix,camera2->GetViewMatrix(), glm::vec3(red, green, blue));

}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,glm::mat4& ViewMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    //bind color
    int location = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(location, 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = ViewMatrix;
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Bind carPosition
    glm::mat4 modelMatrix2 = glm::mat4(1);
    modelMatrix2 = glm::translate(modelMatrix2, camera2->GetTargetPosition());
    location = glGetUniformLocation(shader->program, "carPosition");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix2));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT && 0))
    {
        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(deltaTime );
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) 
        {
            camera->TranslateRight(deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(deltaTime);
        }
    }
    //check if out of bounds
    bool outOfbounds = false;
    glm::vec3 pos =  camera->GetTargetPosition();
    float mindist = 9999;
    for (int i = 0; i < routes[0].size(); i = i+2)
    {
        float dist = glm::distance(pos, routes[0][i]);
        if (dist < mindist)
        {
            mindist = dist;
        }
    }
    if (mindist > 0.3)
    {
        outOfbounds = true;
        acceleration = 1;
    }
    //check car colision
    bool colision = false;
    float carDist = 9999;
    for (int i = 0; i < cars.size(); i++)
    {
        float dist = glm::distance(pos, cars[i].first.position);
        if (dist < carDist)
        {
            carDist = dist;
        }
    }
    
    if (carDist < 0.11)
    {
        colision = true;
        acceleration = 0;
    }

    if(window->KeyHold(GLFW_KEY_W) && !outOfbounds && !colision)
    {
        camera->MoveForward(deltaTime * acceleration);
        camera2->MoveForward(deltaTime * acceleration);
        if (acceleration < 2.5)
        {
            acceleration += deltaTime;
        } 
    }

    if (window->KeyHold(GLFW_KEY_A) && !colision) {
        
        camera->RotateThirdPerson_OY(deltaTime );
        camera2->RotateThirdPerson_OY(deltaTime);
        carAngle += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S) && !colision) {
   
        camera->MoveForward(-deltaTime * acceleration);
        camera2->MoveForward(-deltaTime * acceleration);
        if (acceleration < 2.5)
        {
            acceleration += deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_D)&& !colision)
    {
        camera->RotateThirdPerson_OY(-deltaTime);
        camera2->RotateThirdPerson_OY(-deltaTime);
        carAngle -= deltaTime;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        
        camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
        camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
        
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema2::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
