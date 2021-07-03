#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <algorithm> 
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "shader_m.h"
#include "camera.h"
#include <map>
#include <string>
#include "Light.h"
#include "Cam.h"
#include <vector>
class Wall {

public :
	Wall(float s) {
        this->size = s;
        mapping.push_back(std::vector<float> {-size, size, size, size });  //x z , x z
        mapping.push_back(std::vector<float> {-size, size, -size, -size });  //x z , x z
        mapping.push_back(std::vector<float> {-size, -size, size, -size });
        mapping.push_back(std::vector<float> {size, -size, size, size });
	};
    float size = 4, hight = 4;
    std::vector<std::vector<float>> mapping;
    
    unsigned int VAO, VBO, EBO;
    struct material {
        glm::vec3 diffuse = glm::vec3(0.7f, 0.1f, 0.0f);
        glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
        float shininess = 32;
    } property;
    int idx = 0, idx2 = 0, piece = 0;

	void init(Shader shader) {
        int size;
        float vertices[24*4];// = new float[2*3*4*mapping.size()];
        int index[6*4];// = new int[6 * mapping.size()];
        
        float y = -0.5f;
        float x, z = 0;
        for (int i = 0; i < mapping.size(); i++) {
            if (mapping[i][0] == mapping[i][2]) { // พิกัด x เท่ากัน, normal ตามแกน x
                x = 1; z = 0;
            }
            else {// พิกัด z เท่ากัน, normal ตามแกน z
                x = 0; z = -1;
            }
            vertices[idx] = mapping[i][0];     vertices[idx + 1] = y;         vertices[idx + 2] = mapping[i][1]; //ซ้ายล่าง
            vertices[idx + 3] = x;             vertices[idx + 4] = y;                 vertices[idx + 5] = z; 

            vertices[idx + 6] = mapping[i][0]; vertices[idx + 7] = y + hight; vertices[idx + 8] = mapping[i][1]; //ซ้ายบน
            vertices[idx + 9] = x;              vertices[idx + 10] = y;                  vertices[idx + 11] = z;


            vertices[idx + 12] = mapping[i][2];     vertices[idx + 13] = y;         vertices[idx + 14] = mapping[i][3]; //ขวาล่าง
            vertices[idx + 15] = x;                 vertices[idx + 16] = y;                     vertices[idx + 17] = z;

            vertices[idx + 18] = mapping[i][2]; vertices[idx + 19] = y + hight; vertices[idx + 20] = mapping[i][3]; //ขวาบน
            vertices[idx + 21] = x;              vertices[idx + 22] = y;                 vertices[idx + 23] = z;

            index[idx2] = piece;    index[idx2 + 1] = piece+1;      index[idx2 + 2] = piece+2;
            index[idx2+3] = piece+1;  index[idx2 + 4] = piece + 2;  index[idx2 + 5] = piece + 3;

            idx2 += 6;
            idx += 24;
            piece += 4;
        }
        for (int i = 0; i < idx; i++) {
            if (i % 6 == 0) std::cout << std::endl;
            std::cout << vertices[i] << " ";
            
        }
        float v[] = {
            -0.5,0,0.5,0,0,1,
            -0.5,0.5,0.5,0,0,1,
            0.5,0,0.5,0,0,1,
            0.5,0.5,0.5,0,0,1
             /*-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,*/
        };
        float ind[] = {
            0,1,2,
            1,2,3
        };
        /*float vertice[] = {
          -0.5f, -0.5f, 0.0f, // left
           0.5f, -0.5f, 0.0f, // right
           0.0f,  0.5f, 0.0f  // top
           };*/
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);
        //เชื่อม

        shader.use();

        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
	}

    void draw(Shader shader, glm::mat4 projection, glm::mat4 view, Light light, Cam camera) {
        shader.use();
        shader.setMat4("model", glm::mat4(1.0f));
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        shader.setVec3("material.diffuse", property.diffuse);
        shader.setVec3("material.specular", property.specular);
        shader.setFloat("material.shininess", property.shininess);

        shader.setVec3("viewPos", camera.position);


        shader.setVec3("light.ambient", light.property.ambient);
        shader.setVec3("light.position", light.property.position);
        shader.setVec3("light.specular", light.property.specular);
        shader.setVec3("light.diffuse", light.property.diffuse);

        glBindVertexArray(this->VAO);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6*4, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }



};