#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <algorithm> 
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "Header/shader_m.h"
#include "Header/stb_image.h"
#include "Header/camera.h"
#include <map>
#include <string>

class Plane {

public:
    Plane();
    unsigned int VAO, VBO, EBO;
    Shader shader;


    void init() {
        shader = Shader("plane.vs","plane.fs");
        float vertices[] = {
            10.0f , 0.0f , 10.0f,
            -10.0f , 0.0f , 10.0f,
            -10.0f , 0.0f , -10.0f,
            10.0f , 0.0f , -10.0f
        };
        int idx[] = {
            0,1,2,
            1,2,3
        };
       
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

      

    }

    void clear() {
        glDeleteVertexArrays(1, &this->VAO);  
        glDeleteBuffers(1, &this->VBO);  
        glDeleteBuffers(1, &this->EBO);
    }

    //void draw(Camera camera, glm::vec3 lightPos) {
    void draw(){
       
        glBindVertexArray(this->VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }


};




