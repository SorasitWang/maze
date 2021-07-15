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
#include <string>
#include "model.h"
#include <time.h>
#include "camera.h"
#include <map>
#include "Light.h"
#include "Cam.h"
#include "SpotLight.h"
#include "Wall.h"
class Star {

public:
    Star(int num,float area) {
        this->num = num;
        this->area = area;
    };

    unsigned int VAO, VBO, EBO, texture;
    Model ourModel = Model("C:\\Users\\LEGION\\source\\repos\\maze\\Res\\star.obj");
    std::vector<glm::vec3> positions;
    std::vector<float> sizes;

    float shininess = 8;
    int num = 5;
    float mul = 2, area, base = -0.2f;

    void init(Shader shader,Wall wall) {

        //gen positions
        float randX, randZ, randS;
        glm::vec3 tmpPos;
        srand(time(0));
        for (int i = 0; i < num; i++) {
            randX = ((float)std::rand() / RAND_MAX - 0.5) * mul;
            randZ = ((float)std::rand() / RAND_MAX - 0.5) * mul;
            tmpPos = glm::vec3(randX * area, base, randZ * area);
            while (wall.isCol(tmpPos, 0.1,0.1,0.1)) {
                randX = ((float)std::rand() / RAND_MAX - 0.5) * mul;
                randZ = ((float)std::rand() / RAND_MAX - 0.5) * mul;
                tmpPos = glm::vec3(randX * area, base, randZ * area);
            }
            positions.push_back(tmpPos);
            std::cout << tmpPos.x << " " << tmpPos.z << std::endl;
            sizes.push_back(1);
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on currently bound texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load("C:\\Users\\LEGION\\source\\repos\\maze\\Res\\star.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        shader.use();
        shader.setInt("texture_diffuse", 0);
        stbi_image_free(data);
    }



    //void draw(Camera camera, glm::vec3 lightPos) {
    bool draw(Shader shader, glm::mat4 projection, glm::mat4 view, Light light, SpotLight spotLight, Camera camera,Character character) {
        
        
        shader.use();


        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        //shader.setVec3("material.diffuse", property.diffuse);
        shader.setFloat("material.shininess", shininess);

        shader.setVec3("viewPos", camera.Position);

        shader.setVec3("spotLight.position", spotLight.property.position);
        shader.setVec3("spotLight.direction", spotLight.property.direction);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(spotLight.property.cutoff)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotLight.property.outerCutoff)));
        shader.setVec3("spotLight.ambient", spotLight.property.ambient);
        shader.setVec3("spotLight.specular", spotLight.property.specular);
        shader.setVec3("spotLight.diffuse", spotLight.property.diffuse);


        if (camera.view == 0) {
            shader.setVec3("light.position", camera.Position);
            shader.setVec3("light.direction", camera.Front);
        }
        else {
            shader.setVec3("light.position", camera.Position);
            shader.setVec3("light.direction", glm::vec3(0.0f, -1.0f, 0.0f));
        }
        shader.setFloat("light.cutOff", glm::cos(glm::radians(light.property.cutoff)));
        shader.setFloat("light.outerCutOff", glm::cos(glm::radians(light.property.outerCutoff)));

        shader.setVec3("light.ambient", light.property.ambient);
        shader.setVec3("light.specular", light.property.specular);
        shader.setVec3("light.diffuse", light.property.diffuse);

        glBindVertexArray(this->VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        for (int i = 0; i < this->num; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            //std::cout << i << std::endl;
            model = glm::translate(model, positions[i]);

            
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.02, 0.02, 0.02));
            shader.setMat4("model", model);
            ourModel.Draw(shader);
        }

        glBindVertexArray(0);
        return isCollect(character);
    }

    bool isCollect(Character character) {

        for (int i = 0; i < num; i++){
            //std::cout << i << " " << glm::distance(character.position, positions[i]) << std::endl;
            if (glm::distance(character.position, positions[i]) < 0.25) {
                
                positions.erase(positions.begin()+i);
                num--;
                return true;
            }
        }
        return false;
        
        


    }
};