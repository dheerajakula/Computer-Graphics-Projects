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
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,        
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,     
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,   
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f      
        };

        no_of_vertices = vertices.size()/6;
        model = glm::mat4(1.0f);
        setup();
    }

    object(std::vector<float> v)
    {
        vertices = v;
        no_of_vertices = vertices.size() / 6;
        model = glm::mat4(1.0f);
        setup();
    }

    object(const char *file_name, float scale = 1)
    {
        readfile(file_name,scale);
        
        no_of_vertices = vertices.size() / 6;
        model = glm::mat4(1.0f);
        setup();
    }

    bool readfile(const char* file_name, float scale)
    {
     
        std::vector<float> temp_vertices;
        std::vector<float> temp_normals;
        std::vector<int> vertexIndices;
        std::vector<int> normalIndices;
        std::vector<float> final_vertices;
        FILE* file = fopen(file_name, "r");
     

        if (file == NULL) {
            printf("Impossible to open the file !\n");
        
            return false;
            
        }

        while (1) {

            char lineHeader[256];
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
            else if (strcmp(lineHeader, "vn") == 0) {
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_normals.push_back(vertex.x);
                temp_normals.push_back(vertex.y);
                temp_normals.push_back(vertex.z);
            }
            else if (strcmp(lineHeader, "f") == 0) {

                int vertexIndex[3];
                int normalIndex;
                int matches = fscanf(file, "%d//%d %d//%*d %d//%*d\n", &vertexIndex[0], &normalIndex, &vertexIndex[1], &vertexIndex[2]);
                if (matches != 4) {
                    printf("File can't be read\n");
                    return false;
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                normalIndices.push_back(normalIndex);
                normalIndices.push_back(normalIndex);
                normalIndices.push_back(normalIndex);
            }
        }
        std::cout << final_vertices.size() << std::endl;
        std::cout << normalIndices.size() << std::endl;
        std::cout << temp_normals.size() << std::endl;
        std::cout << temp_normals.size() << std::endl;
        int c = 0;
        for (int i = 0; i < vertexIndices.size(); i++)
        {
            int index = (vertexIndices[i] - 1) * 3;
            int index2 = (normalIndices[i] - 1) * 3;

            final_vertices.push_back(temp_vertices[index]*scale);
          
            final_vertices.push_back(temp_vertices[index+1]*scale);
            
            final_vertices.push_back(temp_vertices[index+2]*scale);
 

            final_vertices.push_back(temp_normals[index2]);

            final_vertices.push_back(temp_normals[index2+1]);

            final_vertices.push_back(temp_normals[index2+2]);

            //final_vertices.push_back(temp_normals[index] * scale);
            //final_vertices.push_back(temp_normals[index + 1] * scale);
            //final_vertices.push_back(temp_normals[index + 2] * scale);
         
        }
        vertices = final_vertices;
        std::cout << vertices.size() << std::endl;
    }

    void setup()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, no_of_vertices * 6 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        // link vertex attributes
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

   
   
    void translate(glm::vec3 translation)

    {
        model = glm::translate(model, translation);
    }

    void rotate(float angle, glm::vec3 axis)
    {
        model = glm::rotate(model, glm::radians(angle), axis);
    }

    void scale(float s)
    {
        model = glm::scale(model, glm::vec3(s));
    }

    void draw(Shader ourShader)
    {
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, no_of_vertices);
    }

};