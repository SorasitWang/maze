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
#include "Cam.h"
#include <map>
#include "Light.h"
#include <string>

class Character {

public:

    Character(float idx) {
        this->idx = idx;
        this->position = startPos;
    }
    float idx = 4;
    unsigned int VAO, VBO;

    struct material {
        glm::vec3 diffuse = glm::vec3(0.1f, 0.7f, 0.3f);
        glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
        float shininess = 32;
    } property;

    struct movement {
        bool jump = false;
        float speed = 2.5;
        float jumpAcc = 5;
        float jumpTimeMax = 1;
        float jumpTime = 0;
        float jumpHight = 1.0;
        float tureRate = 0;
        float sensitivity = 0.1f;
    } movement;

    glm::vec3 position;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    float yaw= -90.0f;
    float pitch = -0.1f;

    glm::vec3 startPos = glm::vec3(5.6, 0,5.6);

    void init(Shader shader) {
        this->worldUp = up;
        float vertices[] = {
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f
        };

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        //เชื่อม

        shader.use();

        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void draw(Shader shader, glm::mat4 projection, glm::mat4 view, Light light, Camera camera, float deltatime) {
        if (movement.jump == true) {
            jump(deltatime);
        }
        shader.use();
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, this->position);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        shader.setVec3("material.diffuse", property.diffuse);
        shader.setVec3("material.specular", property.specular);
        shader.setFloat("material.shininess", property.shininess);

        shader.setVec3("viewPos", camera.Position);

        shader.setVec3("light.direction", camera.Front);
        shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        shader.setVec3("light.ambient", light.property.ambient);
        shader.setVec3("light.position", light.property.position);
        shader.setVec3("light.specular", light.property.specular);
        shader.setVec3("light.diffuse", light.property.diffuse);

        if (camera.view == 1) {
            glBindVertexArray(this->VAO);

            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
       
    }

    void move(Camera_Movement direction, float deltatime,Wall wall) {
        glm::vec3 tmpPos, tmpZ, tmpX;
        bool x = false, z = false;
        float velocity = movement.speed * deltatime;

        if (direction == FORWARD) {
            tmpPos = glm::vec3(front.x,0,front.z) * velocity;
        }
        if (direction == LEFT) {
            tmpPos = glm::vec3(-1, -1,-1) *right * velocity;
        }
        if (direction == RIGHT) {
            tmpPos = right* velocity;
        }
        if (direction == BACKWARD) {
            tmpPos = glm::vec3(-1, -1, -1) *glm::vec3(front.x, 0, front.z) * velocity;
        }
        /*tmpX = glm::vec3(tmpPos.x*glm::sin(glm::radians(yaw - 90.0f)), 0, 0);
        tmpZ = glm::vec3(0, 0, tmpPos.z* glm::cos(glm::radians(yaw - 90.0f)));

        if (!wall.isCol(position + glm::vec3(1.1)*tmpX, 0.05, 0.05, 0.05))
            x = true;
        if (!wall.isCol(position + glm::vec3(1.1) *tmpZ, 0.05, 0.05, 0.05))
            z = true;
        std::cout << x << " " << z << std::endl;
        std::cout << tmpX.x << " " << tmpZ.z << " " << glm::sin(glm::radians(yaw - 90.0f)) << " " << glm::cos(glm::radians(yaw - 90.0f)) <<std::endl;
        if (x && z) {
            position += tmpPos;
        }
        else if (x) {
            position += tmpX;
        }
        else if (z) {
            position += tmpZ;
        }*/
      
        if(wall.isCol(position + glm::vec3(tmpPos.x,0,0), 0.05, 0.05, 0.05) == false) {
            position += glm::vec3(tmpPos.x, 0, 0);
        }
        if (wall.isCol(position + glm::vec3(0, 0, tmpPos.z), 0.05, 0.05, 0.05) == false) {
            position += glm::vec3(0, 0, tmpPos.z);
        }
    }

    void jump(float deltatime) {
        
        
        if (movement.jumpTime >= movement.jumpTimeMax) {
            //movement.jump = false;
            if (movement.jumpAcc < 0) { //falling down
                movement.jump = false;
                position.y = startPos.y;
            }
            else {
                movement.jumpTime = movement.jumpTimeMax;
                movement.jumpTime += deltatime;
                float _veclocity = movement.jumpAcc * movement.jumpTime;
                float velocity = _veclocity * deltatime;
                position += glm::vec3(0, 1, 0) * velocity;
            }
            movement.jumpAcc *= -1;
            movement.jumpTime = 0;
            return;
        }
        movement.jumpTime += deltatime;
        float _veclocity = movement.jumpAcc * movement.jumpTime;
        float velocity = _veclocity * deltatime;
        position += glm::vec3(0,1,0) * velocity;
        std::cout << movement.jumpTime << " - " << movement.jumpTimeMax << " - " << movement.jumpAcc << std::endl;

    }

    void processMovement(float xoffset, float yoffset, int view, GLboolean constrainPitch = true)
    {
        //std::cout << pitch << " " << yaw << std::endl;
        if (view == 1) {
            updateVectors();
            return;
        }
        //return;
        
        xoffset *= movement.sensitivity;
        yoffset *= movement.sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateVectors();
    }

    void changeView(int view) {
        if (view == 1) {
            yaw = -89.5f;
            pitch = -0.1f;
            updateVectors();
        }
    }
    void setJump() {
        if (movement.jump == false) {
            movement.jump = true;
        }
    }
    private :

        void updateVectors()
        {
            // calculate the new Front vector
            glm::vec3 _front;
            _front.x = cos(glm::radians(yaw)) *cos(glm::radians(pitch));
            _front.y = sin(glm::radians(pitch));
            _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(_front);
            // also re-calculate the Right and Up vector
            right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            up = glm::normalize(glm::cross(right, front));
        }


};
