#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "shader.h"


class object
{
public:
      
    std::vector<float> vertices;
    unsigned int VAO;
    unsigned int VBO;
    int no_of_vertices;
    glm::mat4 model;
    

    object ()
    {
        vertices = { 
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f};

        no_of_vertices = vertices.size()/3;
        model = glm::mat4(1.0f);
        setup();
    }

    object(std::vector<float> v)
    {
        vertices = v;
        no_of_vertices = vertices.size() / 3;
        model = glm::mat4(1.0f);
        setup();
    }

    object(const char *file_name, float scale = 1)
    {
        readfile(file_name,scale);
        
        no_of_vertices = vertices.size() / 3;
        model = glm::mat4(1.0f);
        setup();
    }

    bool readfile(const char* file_name, float scale)
    {
     
        std::vector<float> temp_vertices;
        std::vector<int> vertexIndices;
        std::vector<float> final_vertices;
        FILE* file = fopen(file_name, "r");
     

        if (file == NULL) {
            printf("Impossible to open the file !\n");
        
            return false;
            
        }

        while (1) {

            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            // else : parse lineHeader
            if (strcmp(lineHeader, "v") == 0) {
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex.x);
                temp_vertices.push_back(vertex.y);
                temp_vertices.push_back(vertex.z);
            }
            else if (strcmp(lineHeader, "f") == 0) {

                int vertexIndex[3];
                int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                if (matches != 3) {
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return false;
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
            }
        }

        for (int i = 0; i < vertexIndices.size(); i++)
        {
            int index = (vertexIndices[i] - 1) * 3;
            final_vertices.push_back(temp_vertices[index]*scale);
            final_vertices.push_back(temp_vertices[index+1]*scale);
            final_vertices.push_back(temp_vertices[index+2]*scale);
            
        }
        vertices = final_vertices;
    }

    void setup()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, no_of_vertices * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
    }

   
   
    void translate(glm::vec3 translation)

    {
        model = glm::translate(model, translation);
    }

    void rotate(float angle, glm::vec3 axis)
    {
        model = glm::rotate(model, glm::radians(angle), axis);
    }

    void draw(Shader ourShader)
    {
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, no_of_vertices);
    }

};