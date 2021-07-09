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
#include "SpotLight.h"
class Plane {

public:
    Plane(float s) {
        this->size = s;
    };
    float size;
    unsigned int VAO, VBO, EBO,texture;
    struct material {
        glm::vec3 diffuse = glm::vec3(0.7f,0.1f,0.0f);
        glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
        float shininess = 8;
    } property ;


    void init(Shader shader) {
        
        float vertices[] = {
           size, -0.5f, size,     0.0f,1.0f,0.0f,    1,1,// top right
           size, -0.5f, -size,    0.0f,1.0f,0.0f,   1,0,// bottom right
          -size, -0.5f,-size,     0.0f,1.0f,0.0f,   0,0,// bottom left
          -size, -0.5f, size,     0.0f,1.0f,0.0f,    0,1
        };
        /*float vertices[] = {
       -0.5f, -0.5f, 0.0f, // left  
        0.5f, -0.5f, 0.0f, // right 
        0.0f,  0.5f, 0.0f  // top   
        };*/
        int idx[] = {
            0,1,3,
            1,2,3
        };
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on currently bound texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load("C:\\Users\\LEGION\\source\\repos\\maze\\Res\\planeDark1.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        shader.use();
        shader.setInt("material.diffuse", 0);

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);
        //เชื่อม

        shader.use();

        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

     
    }

    void clear() {
        glDeleteVertexArrays(1, &this->VAO);  
        glDeleteBuffers(1, &this->VBO);  
        glDeleteBuffers(1, &this->EBO);
    }

    //void draw(Camera camera, glm::vec3 lightPos) {
    void draw(Shader shader,glm::mat4 projection , glm::mat4 view,Light light,SpotLight spotLight,Camera camera){
        shader.use();
        shader.setMat4("model", glm::mat4(1.0f));
        shader.setMat4("projection", projection);
        shader.setMat4("view",view);

        shader.setVec3("material.diffuse", property.diffuse);
        shader.setVec3("material.specular", property.specular);
        shader.setFloat("material.shininess", property.shininess);

        shader.setVec3("viewPos", camera.Position);

        shader.setVec3("spotLight.position", spotLight.property.position);
        shader.setVec3("spotLight.direction", spotLight.property.direction);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(spotLight.property.cutoff)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotLight.property.outerCutoff)));
        shader.setVec3("spotLight.ambient", spotLight.property.ambient);
        shader.setVec3("spotLight.specular", spotLight.property.specular);
        shader.setVec3("spotLight.diffuse", spotLight.property.diffuse);
        
        shader.setFloat("light.cutOff", glm::cos(glm::radians(light.property.cutoff)));
        shader.setFloat("light.outerCutOff", glm::cos(glm::radians(light.property.outerCutoff)));

        shader.setVec3("light.ambient", light.property.ambient);
        if (camera.view == 0) {
            shader.setVec3("light.position", camera.Position);
            shader.setVec3("light.direction", camera.Front);
        }
        else {
            shader.setVec3("light.position", camera.Position);
            shader.setVec3("light.direction", glm::vec3(0.0f, -1.0f, 0.0f));
        }
        shader.setVec3("light.specular", light.property.specular);
        shader.setVec3("light.diffuse", light.property.diffuse);

        glBindVertexArray(this->VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }


};




