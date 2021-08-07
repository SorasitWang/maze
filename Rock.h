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
class Rock {

public:
    /*Rock(float area,float y) {
        this->area = area;
        this->base = y;
    };*/
    Rock(float area) {
        this->area = area;
    };
    unsigned int VAO, VBO, EBO, texture , buffer;
    Model ourModel = Model("C:\\Users\\LEGION\\source\\repos\\maze\\Res\\rock.obj");
    std::vector<glm::vec3> positions;
    std::vector<float> sizes;
    int count = 100;
    float shininess = 8;
    float mul = 2,area,base=-0.5f;
    glm::mat4 *modelMatrices;

    void init(Shader shader) {

        modelMatrices = new glm::mat4[count];
        srand(glfwGetTime()); // initialize random seed
        float radius = 150.0;
        float offset = 25.0f;
        float randX, randZ, randS, mul = 2, base = -0.5;
        for (unsigned int i = 0; i < count; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            // 1. translation: displace along circle with 'radius' in range [-offset, offset]

           // model = glm::translate(model, glm::vec3(x, y, z));
            randX = ((float)std::rand() / RAND_MAX - 0.5) * 6 *mul;
            randZ = ((float)std::rand() / RAND_MAX - 0.5) * 6* mul;
            randS = ((float)std::rand() / RAND_MAX) * 2;

            model = glm::translate(model, glm::vec3(randX, base, randZ));
            // 2. scale: Scale between 0.05 and 0.25f
            float scale = 0.0002;
            model = glm::scale(model, glm::vec3(scale));

            // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rotAngle = (rand() % 360);
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. now add to list of matrices
            modelMatrices[i] = model;
        }

        // configure instanced array
        // -------------------------
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

        // set transformation matrices as an instance vertex attribute (with divisor 1)
        // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
        // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
        // -----------------------------------------------------------------------------------------------------------------------------------
        for (unsigned int i = 0; i < ourModel.meshes.size(); i++)
        {
            unsigned int mVAO = ourModel.meshes[i].VAO;
            glBindVertexArray(mVAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);

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
        unsigned char* data = stbi_load("C:\\Users\\LEGION\\source\\repos\\maze\\Res\\rock_diffuse.png", &width, &height, &nrChannels, 0);
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
        shader.setInt("texture_diffuse", 0);
    }



    //void draw(Camera camera, glm::vec3 lightPos) {
    void draw(Shader shader, glm::mat4 projection, glm::mat4 view, Light light, SpotLight spotLight
            , Camera camera, glm::mat4 lightSpaceMatrix){
        shader.use();

        
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        //shader.setVec3("material.diffuse", property.diffuse);
        shader.setFloat("material.shininess", shininess);

        shader.setVec3("viewPos", camera.Position);
        shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
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
     
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture); // note: we also made the textures_loaded vector public (instead of private) from the model class.
        for (unsigned int i = 0; i < ourModel.meshes.size(); i++)
        {
            glBindVertexArray(ourModel.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, ourModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, count);
            glBindVertexArray(0);
        }


        glBindVertexArray(0);
    }
};