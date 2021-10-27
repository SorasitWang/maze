#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <algorithm> 
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "../header/shader_m.h"
#include "../header/camera.h"
#include <map>
#include <string>
#include "../header/Light/Light.h"
#include <vector>
#include "../header/Light/SpotLight.h"
class Wall {

public :
	Wall(float s) {
        this->size = s;
        //outer wall
        border.push_back(std::vector<float> {-size, size, size, size });  //x z , x z            --------        z = 1
        border.push_back(std::vector<float> {-size, size, -size, -size });  //x z , x z          |               x = -1
        border.push_back(std::vector<float> {-size, -size, size, -size });      //               ----------      z = -1
        border.push_back(std::vector<float> {size, -size, size, size });//                                   |   x = 1

        border.push_back(std::vector<float> {8.0f * size / 10, 10.0f * size / 10, 8.0f * size / 10, 4.0f * size / 10});
        border.push_back(std::vector<float> {8.0f * size / 10, 2.0f * size / 10, 8.0f * size / 10, -8.0f * size / 10});

        border.push_back(std::vector<float> {6.0f * size / 10, 10.0f * size / 10, 6.0f * size / 10, 6.0f * size / 10});
        border.push_back(std::vector<float> {6.0f * size / 10, 4.0f * size / 10, 6.0f * size / 10, -4.0f * size / 10});

        border.push_back(std::vector<float> {4.0f * size / 10, 8.0f * size / 10, 4.0f * size / 10, -6.0f * size / 10});

        border.push_back(std::vector<float> {2.0f * size / 10, 8.0f * size / 10, 2.0f * size / 10, -4.0f * size / 10});

        border.push_back(std::vector<float> {0.0f * size / 10, 8.0f * size / 10, 0.0f * size / 10, -6.0f * size / 10});

        border.push_back(std::vector<float> {-2.0f * size / 10, 10.0f * size / 10, -2.0f * size / 10, 2.0f * size / 10});
        border.push_back(std::vector<float> {-2.0f * size / 10, 0.0f * size / 10, -2.0f * size / 10, -8.0f * size / 10});

        border.push_back(std::vector<float> {-4.0f * size / 10, 2.0f * size / 10, -4.0f * size / 10, -6.0f * size / 10});

        border.push_back(std::vector<float> {-6.0f * size / 10, 10.0f * size / 10, -6.0f * size / 10, 6.0f * size / 10});
        border.push_back(std::vector<float> {-6.0f * size / 10, 4.0f * size / 10, -6.0f * size / 10, -8.0f * size / 10});

        border.push_back(std::vector<float> {-8.0f * size / 10, -10.0f * size / 10, -8.0f * size / 10, 8.0f * size / 10});

        border.push_back(std::vector<float> {0.0f * size / 10, -8.0f * size / 10, 6.0f * size / 10, -8.0f * size / 10});
        border.push_back(std::vector<float> {-8.0f * size / 10, -8.0f * size / 10, -4.0f * size / 10, -8.0f * size / 10});

        border.push_back(std::vector<float> {0.0f * size / 10, -6.0f * size / 10, 8.0f * size / 10, -6.0f * size / 10});
        //inner wall
	};
    float size = 8, hight = 2, thin = 0.4;
    std::vector<std::vector<float>>border;
    std::vector<std::vector<float>> innerWall;
    
    unsigned int VAO, VBO, EBO, texture;
    Material property = Material();
 
   
    int idx = 0, idx2 = 0, piece = 0;

	void init(Shader shader) {

        property.diffuse = glm::vec3(0.1f, 0.4f, 0.6f);
        property.specular = glm::vec3(0.7f, 0.7f, 0.7f);

        int size;
        float base = -0.5f;
        float vertices[5*8*4*20];// = new float[2*3*4*mapping.size()];
        int index[30*20];// = new int[6 * mapping.size()];

        for (int j = 0; j < border.size(); j++) {
            std::vector<std::vector<float>> face;   //Top view : แต่ละขอบ บน ซ้าย ล่าง ขวา
            if (border[j][1]  == border[j][3]) {
                face.push_back(std::vector<float> {border[j][0], border[j][1] + thin / 2, border[j][2], border[j][1] + thin / 2 });
                face.push_back(std::vector<float> {border[j][0], border[j][1] + thin / 2, border[j][0], border[j][1] - thin / 2 });
                face.push_back(std::vector<float> {border[j][0], border[j][1] - thin / 2, border[j][2], border[j][1] - thin / 2 });
                face.push_back(std::vector<float> {border[j][2], border[j][1] + thin / 2, border[j][2], border[j][1] - thin / 2 });
            }
            else{
                face.push_back(std::vector<float> {border[j][0] - thin / 2, border[j][1], border[j][0] + thin / 2, border[j][1] });
                face.push_back(std::vector<float> {border[j][0] - thin / 2, border[j][1], border[j][0] - thin / 2, border[j][3] });
                face.push_back(std::vector<float> {border[j][0] - thin / 2, border[j][3], border[j][2] + thin / 2, border[j][3] });
                face.push_back(std::vector<float> {border[j][2] + thin / 2, border[j][1], border[j][2] + thin / 2, border[j][3] });
            }
            
            float x, z = 0;
            for (int i = 0; i < face.size(); i++) {

                if (i % 4 == 0) {
                    x = 0; z = 1;
                }
                if (i % 4 == 2) {
                    x = 0; z = -1;
                }
                if (i % 4 == 1) {
                    x = -1; z = 0;
                }
                if (i % 4 == 3) {
                    x = 1; z = 0;
                }
                vertices[idx] = face[i][0];     vertices[idx + 1] = base;         vertices[idx + 2] = face[i][1]; //ซ้ายล่าง
                vertices[idx + 3] = x;             vertices[idx + 4] = base;                 vertices[idx + 5] = z;
                vertices[idx + 6] = 0.0f;             vertices[idx + 7] = 0.0f;

                vertices[idx + 8] = face[i][0]; vertices[idx + 9] = base + hight; vertices[idx + 10] = face[i][1]; //ซ้ายบน
                vertices[idx + 11] = x;              vertices[idx + 12] = base;                  vertices[idx + 13] = z;
                vertices[idx + 14] = 0.0f;             vertices[idx + 15] = 1.0f;


                vertices[idx + 16] = face[i][2];     vertices[idx + 17] = base;         vertices[idx + 18] = face[i][3]; //ขวาล่าง
                vertices[idx + 19] = x;                 vertices[idx + 20] = base;                     vertices[idx + 21] = z;
                vertices[idx + 22] = 1.0f;             vertices[idx + 23] = 0.0f;

                vertices[idx + 24] = face[i][2]; vertices[idx + 25] = base + hight; vertices[idx + 26] = face[i][3]; //ขวาบน
                vertices[idx + 27] = x;              vertices[idx + 28] = base;                 vertices[idx + 29] = z;
                vertices[idx + 30] = 1.0f;             vertices[idx + 31] =1.0f;

                index[idx2] = piece;    index[idx2 + 1] = piece + 1;      index[idx2 + 2] = piece + 2;
                index[idx2 + 3] = piece + 1;  index[idx2 + 4] = piece + 2;  index[idx2 + 5] = piece + 3;

                idx2 += 6;
                idx += 32;
                piece += 4;
            }

            //gen roof
            vertices[idx] = face[2][0];     vertices[idx + 1] = base + hight;         vertices[idx + 2] = face[2][1]; //ซ้ายล่าง
            vertices[idx + 3] = 0;             vertices[idx + 4] = 1;                 vertices[idx + 5] = 0;
            vertices[idx + 6] = 0.0f;             vertices[idx + 7] = 0.0f;

            vertices[idx + 8] = face[0][0]; vertices[idx + 9] = base + hight; vertices[idx + 10] = face[0][1]; //ซ้ายบน
            vertices[idx + 11] = 0;              vertices[idx + 12] = 1;                  vertices[idx + 13] = 0;
            vertices[idx + 14] = 0.0f;             vertices[idx + 15] = 1.0f;

            vertices[idx + 16] = face[2][2];     vertices[idx + 17] = base + hight;         vertices[idx + 18] = face[2][3]; //ขวาล่าง
            vertices[idx + 19] = 0;                 vertices[idx + 20] = 1;                     vertices[idx + 21] = 0;
            vertices[idx + 22] = 1.0f;             vertices[idx + 23] = 0.0f;

            vertices[idx + 24] = face[0][2]; vertices[idx + 25] = base + hight; vertices[idx + 26] = face[0][3]; //ขวาบน
            vertices[idx + 27] = 0;              vertices[idx + 28] = 1;                 vertices[idx + 29] = 0;
            vertices[idx + 30] = 1.0f;             vertices[idx + 31] = 1.0f;

            index[idx2] = piece;    index[idx2 + 1] = piece + 1;      index[idx2 + 2] = piece + 2;
            index[idx2 + 3] = piece + 1;  index[idx2 + 4] = piece + 2;  index[idx2 + 5] = piece + 3;


            idx2 += 6;
            idx += 32;
            piece += 4;

        }

        texture = loadTexture("C:\\Users\\LEGION\\source\\repos\\maze\\Res\\wall.jpg");
       
       
        shader.use();
        shader.setInt("material.diffuse", 0);
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);
        //เชื่อม

        
        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        //coord
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        
	}


    void draw(Light light, SpotLight spotLight, Shader &shader,glm::mat4 lightSpaceMatrix, glm::mat4 projection, glm::mat4 view, Camera cam)
    {
       setupShader(shader,projection,view,light,spotLight,cam,property,lightSpaceMatrix,false);
        
        // render Cube
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
        glDrawElements(GL_TRIANGLES, 30 * 20, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void draw(Shader shader) {
        // initialize (if necessary)
        glm::mat4 model = glm::mat4(1.0f);
       
        shader.use();
        shader.setMat4("model", model);
        shader.setBool("draw", false);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 30 * 20, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

   
    // x y z is in the center , sizeX Y Z is half of actually SIZE*/
    bool isCol(glm::vec3 position,float sizeX, float sizeY,float sizeZ) {
        float x,y,z,base=-0.5f;
        float offset = 0.07;
        x = position.x;  y = position.y; z = position.z;
      
        if ( (y + sizeY < base) || (y - sizeY > base + hight+1)) return false;
        
        bool tmp = false;
        for (int i = 0; i < border.size(); i++) {
            //check wall along z
            if (border[i][0] == border[i][2]) {
                //check x
                if ((abs(x + sizeX - border[i][0]) < thin/2+ offset) || (abs(x - sizeX - border[i][0]) < thin/2+ offset)) {
                    //check z
                    //std::cout << z << " " << sizeZ << " " << border[i][1] << " " << border[i][3] << " " << thin << std::endl;
                    if ( (z - sizeZ > std::max(border[i][1], border[i][3])) || (z + sizeZ < std::min(border[i][1], border[i][3]) ) ){
                    }
                    else {
                        std::cout << "x" << std::endl;
                        return true;
                    }
                }
            }
            //check wall along x
            else {
                //check z
                if ((abs(z + sizeZ - border[i][1]) < thin/2 + offset) || (abs(z - sizeZ - border[i][1]) < thin/2 + offset)) {
                    //check x
                    if ((x - sizeX > std::max(border[i][0], border[i][2])) || (x + sizeX < std::min(border[i][0], border[i][2]))) {
                    }
                    else {
                        //std::cout << "z" << std::endl;
                        return true;
                    }
                }
            }
        }
        return false;
    }
    

};