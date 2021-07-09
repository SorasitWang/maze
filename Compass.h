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
#include <map>
#include <string>
#include "model.h"

const float PI = 3.1415926;
class Compass {

public:
    Compass() {

    };
    unsigned int VAO, VBO, EBO,texture;
    Model ourModel = Model("C:\\Users\\LEGION\\source\\repos\\maze\\Res\\Arrow5.obj");
    glm::vec3 destination = glm::vec3(-5.3, 0,-5.7);


    void init(Shader shader) {
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on currently bound texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load("C:\\Users\\LEGION\\source\\repos\\maze\\Res\\diffuse.png", &width, &height, &nrChannels, 0);
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
        shader.setInt("texture_diffuse1", 0);
        stbi_image_free(data);
    }



    //void draw(Camera camera, glm::vec3 lightPos) {
    void draw(Shader shader,glm::vec3 now,float yaw) {
        shader.use();

        // view/projection transformations
        glm::vec3 dis = glm::normalize(glm::vec3(destination.x - now.x, 0, destination.z - now.z));
        glm::vec3 front = glm::normalize(glm::vec3(now.x, 0, now.z - 1));

       
        // render the loaded model
        float tmp = glm::atan(dis.z / dis.x);
        if (dis.x > 0)
            tmp +=  PI;
        /*if (dis.z >0)
            tmp -= PI/2.0f;*/
       
        //std::cout << dis.x << " " << dis.z << " " << glm::degrees(tmp) << " " << glm::atan(dis.z / dis.x) << std::endl;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.8f, 0.8f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model,glm::radians(55.0f),glm::vec3(1.0f,0.0f, 0.0f));
        model = glm::rotate(model, (tmp)+PI-glm::radians(yaw+90) ,
            glm::vec3(0.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture);
        ourModel.Draw(shader);
    }
};