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
class Plane {

public:
    Plane() {
        
    };

    unsigned int VAO, VBO, EBO;
    struct material {
        glm::vec3 diffuse = glm::vec3(0.7f,0.1f,0.0f);
        glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
        float shininess = 32;
    } property ;


    void init(Shader shader) {
        
        float vertices[] = {
           10.0f, -0.5f, 10.0f,     0.0f,1.0f,0.0f, // top right
           10.0f, -0.5f, -10.0f,    0.0f,1.0f,0.0f, // bottom right
          -10.0f, -0.5f,-10.0f,     0.0f,1.0f,0.0f, // bottom left
          -10.0f, -0.5f, 10.0f,     0.0f,1.0f,0.0f
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
    void draw(Shader shader,glm::mat4 projection , glm::mat4 view,Light light,Cam camera){
        shader.use();
        shader.setMat4("model", glm::mat4(1.0f));
        shader.setMat4("projection", projection);
        shader.setMat4("view",view);

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
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }


};




