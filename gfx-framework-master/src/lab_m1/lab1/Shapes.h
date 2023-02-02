#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace shapes2D
{   
    void line(std::pair<float, float> start,int length,float angle , std::vector<std::pair<float,float>> &line)
    {
        float x = start.first; 
        float y = start.second;
        for (float i = 0; i < length ;i = i + 0.1)
        {
            x += cos(angle) * 0.1;
            y += sin(angle) * 0.1;
            line.push_back({ x,y});
        }
    };

    void curve(std::pair<float, float> start, int radius,int lineAngle, int angle, std::vector<std::pair<float, float>> &line)
    {
        float len = radius * sin(glm::radians(6.f / radius));
        float x = start.first;
        float y = start.second;
        for (float i = 0; i < abs(angle); i = i + 6.f / radius)
        {
            x += cos(glm::radians((float)(lineAngle + angle/abs(angle) * i))) * len;
            y += sin(glm::radians((float)(lineAngle + angle /abs(angle) * i))) * len;
            line.push_back({ x,y });
        }
    };

    std::vector<std::pair<float, float>> TrackLine()
    {
        std::vector<std::pair<float, float>> TLine;
        TLine.push_back({ 0.f,0.f });
        line({ 0.f,0.f }, 10, glm::radians(0.f), TLine);
        curve(TLine[TLine.size() - 1], 5, 0, -180, TLine);
        line(TLine[TLine.size() - 1], 20, glm::radians(180.f), TLine);
        curve(TLine[TLine.size() - 1], 5, 180 , -85, TLine);
        line(TLine[TLine.size() - 1], 20,glm::radians(95.f), TLine);
        curve(TLine[TLine.size() - 1], 5, 95, 155, TLine);
        line(TLine[TLine.size() - 1], 5, glm::radians(250.f), TLine);
        curve(TLine[TLine.size() - 1], 20, 250, 110, TLine);
        line(TLine[TLine.size() - 1], 25, glm::radians(0.f), TLine);
        curve(TLine[TLine.size() - 1], 10, 0, 90, TLine);
        line(TLine[TLine.size() - 1], 5, glm::radians(90.f), TLine);
        curve(TLine[TLine.size() - 1], 10, 90, 5, TLine);
        line(TLine[TLine.size() - 1], 20, glm::radians(95.f), TLine);
        curve(TLine[TLine.size() - 1],5, 95, 180, TLine);
        line(TLine[TLine.size() - 1], 10, glm::radians(275.f), TLine);
        curve(TLine[TLine.size() - 1], 5, 275, -180, TLine);
        line(TLine[TLine.size() - 1], 10, glm::radians(95.f), TLine);
        curve(TLine[TLine.size() - 1], 4, 95, 175, TLine);
        line(TLine[TLine.size() - 1], 10, glm::radians(270.f), TLine);
        curve(TLine[TLine.size() - 1], 2, 270, -180, TLine);
        line(TLine[TLine.size() - 1], 10, glm::radians(90.f), TLine);
        curve(TLine[TLine.size() - 1], 2, 90, 180, TLine);
        line(TLine[TLine.size() - 1], 13, glm::radians(270.f), TLine);
        curve(TLine[TLine.size() - 1], 5, 270, 90, TLine);
        line(TLine[TLine.size() - 1], 7, glm::radians(0.f), TLine);
        return TLine;
    }

    std::vector<std::pair<float, float>> TLine;
    Mesh* CreateRaceTrack(const std::string& name = "Track") {
        
        TLine = TrackLine();
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        glm::vec3 up(0, 1, 0);
        glm::vec3 color(0.1f, 0.1f, 0.1f);
        int k = 0;
        for (int i = 0; i < TLine.size() - 1; i++)
        {
            glm::vec3 A = glm::vec3(TLine[i].first, 0, TLine[i].second);
            glm::vec3 B = glm::vec3(TLine[i+1].first, 0, TLine[i+1].second);
            glm::vec3 C = A - B;
            glm::vec3 P = cross(C, up);
            glm::vec3 R = A + (P * 3.f);
            glm::vec3 L = A - (P * 3.f);
            
            vertices.push_back(VertexFormat(R, color));
            vertices.push_back(VertexFormat(L, color));
            k += 2;
        }
        
        Mesh* Track = new Mesh(name);

        for (int i = 0; i < k - 2; i+= 2)
        {
                indices.push_back(i);
                indices.push_back(i + 1);
                indices.push_back(i + 3);
                indices.push_back(i + 3);
                indices.push_back(i + 2);
                indices.push_back(i);
        }
        indices.push_back(k - 2);
        indices.push_back(k - 1);
        indices.push_back(1);
        indices.push_back(1);
        indices.push_back(0);
        indices.push_back(k-2);

        Track->InitFromData(vertices, indices);
        return Track;
    };

    std::vector<glm::vec3> GetParralel(float distance,int density)
    {
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        glm::vec3 up(0, 1, 0);
        for (int i = 0; i < TLine.size() - 1; i = i + density)
        {
            glm::vec3 A = glm::vec3(TLine[i].first, 0, TLine[i].second);
            glm::vec3 B = glm::vec3(TLine[i + 1].first, 0, TLine[i + 1].second);
            glm::vec3 C = A - B;
            glm::vec3 P = cross(C, up);
            glm::vec3 R = A + (P * distance);
            glm::vec3 L = A - (P * distance);

            vertices.push_back(R);
            vertices.push_back(L);
        }
        return vertices;
    }

}   

