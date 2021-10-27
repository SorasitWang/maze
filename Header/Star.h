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
#include "../header/Light/Light.h"
//#include "Character.h"
//#include "Camera.h"
#include "../header/Light/SpotLight.h"
#include "../header/Wall/Wall.h"
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
    Material property = Material();

    int num = 5;
    float mul = 2, area, base = -0.2f;

    unsigned int  init(Shader &shader, Wall wall) {

        property.specular = glm::vec3(0.7f, 0.7f, 0.4f);

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
            //std::cout << tmpPos.x << " " << tmpPos.z << std::endl;
            sizes.push_back(1);
        }

        texture = loadTexture("C:\\Users\\LEGION\\source\\repos\\maze\\Res\\orange.png");

        shader.use();
        shader.setInt("texture_diffuse", 0);
        /*
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);*/

        return VAO;
    }


    void draw(Shader &shader) {
        // initialize (if necessary)
        glm::mat4 model = glm::mat4(1.0f);


        /*model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));*/
        shader.use();
        //shader.setMat4("model", model);
        shader.setBool("draw", true);
        // render Cube
        glBindVertexArray(VAO);
        for (int i = 0; i < this->num; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            //std::cout << i << std::endl;
            model = glm::translate(model, positions[i]);


            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.02, 0.02, 0.02));
            shader.setMat4("model", model);
           // glDrawArrays(GL_TRIANGLES, 0, 36);
            ourModel.Draw(shader);
        }
        glBindVertexArray(0);
    }

    void draw(Shader &shader,glm::mat4 projection, glm::mat4 view, glm::mat4 lightSpaceMatrix,Camera cam,Light light,SpotLight spotLight) {

       //setupShader(shader, projection, view, light, cam, property, lightSpaceMatrix);
        /*shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set light uniforms
        shader.setVec3("viewPos", cam.Position);
        shader.setVec3("lightPos", cam.Position);
        shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);*/

        // initialize (if necessary)
       // glm::mat4 model = glm::mat4(1.0f);

        shader.use();
        //shader.setVec3("material.diffuse", property.diffuse);
        shader.setVec3("material.specular", property.specular);
        shader.setFloat("material.shininess", property.shininess);

       // shader.setVec3("viewPos", viewPos);

        //shader.setBool("drawShadow", true);
        shader.setVec3("light.position", cam.Position);
        shader.setVec3("light.direction", cam.Front);


        shader.setFloat("light.cutOff", glm::cos(glm::radians(light.property.cutoff)));
        shader.setFloat("light.outerCutOff", glm::cos(glm::radians(light.property.outerCutoff)));

        shader.setVec3("light.ambient", light.property.ambient);
        shader.setVec3("light.specular", light.property.specular);
        shader.setVec3("light.diffuse", light.property.diffuse);

        shader.setVec3("spotLight.position", spotLight.property.position);
        shader.setVec3("spotLight.direction", spotLight.property.direction);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(spotLight.property.cutoff)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotLight.property.outerCutoff)));
        shader.setVec3("spotLight.ambient", spotLight.property.ambient);
        shader.setVec3("spotLight.specular", spotLight.property.specular);
        shader.setVec3("spotLight.diffuse", spotLight.property.diffuse);
        //glm::mat4 model = glm::mat4(1.0f);
        // render Cube
       glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        for (int i = 0; i < this->num; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            //std::cout << i << std::endl;
            model = glm::translate(model, positions[i]);


            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.02, 0.02, 0.02));
   
             shader.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
               ourModel.Draw(shader);
        }
        glBindVertexArray(0);
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