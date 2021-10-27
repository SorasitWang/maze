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
#include "stb_image.h"
#include "./Light/Light.h"
#include "./Light/SpotLight.h"


struct Material {
    glm::vec3 diffuse = glm::vec3(0.1f, 0.4f, 0.6f);
    glm::vec3 specular = glm::vec3(0.7f, 0.7f, 0.7f);
    float shininess = 32;
};
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void setupShader(Shader& shader, glm::mat4 projection, glm::mat4 view, Light light, SpotLight spotLight, Camera cam, Material property,glm::mat4 lightSpaceMatrix,bool shadow) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setBool("drawShadow", shadow);
    shader.setVec3("viewPos", cam.Position);
    shader.setVec3("lightPos", cam.Position);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    //shader.setVec3("material.diffuse", property.diffuse);
    shader.setVec3("material.specular", property.specular);
    shader.setFloat("material.shininess", property.shininess);

    shader.setVec3("light.position", cam.Position);

    if (cam.view == 0) shader.setVec3("light.direction", cam.Front);
    else shader.setVec3("light.direction", glm::vec3(0.0f, -1.0f, 0.0f));

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
}

void setupShader(Shader& shader, glm::mat4 projection, glm::mat4 view, Light light, SpotLight spotLight, Camera cam, Material property, bool shadow) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setBool("drawShadow", shadow);
    shader.setVec3("viewPos", cam.Position);
    shader.setVec3("lightPos", cam.Position);

    //shader.setVec3("material.diffuse", property.diffuse);
    shader.setVec3("material.specular", property.specular);
    shader.setFloat("material.shininess", property.shininess);

    shader.setVec3("viewPos", cam.Position);
    shader.setVec3("light.position", cam.Position);

    if (cam.view == 0) shader.setVec3("light.direction", cam.Front);
    else shader.setVec3("light.direction", glm::vec3(0.0f, -1.0f, 0.0f));

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
}


void setupShader(Shader& shader, Light light, SpotLight spotLight, Camera cam, Material property, glm::mat4 lightSpaceMatrix, bool shadow) {
    shader.use();
    /*shader.setMat4("projection", projection);
    shader.setMat4("view", view);*/
    // set light uniforms
    shader.setVec3("viewPos", cam.Position);
    shader.setVec3("lightPos", cam.Position);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shader.setBool("drawShader", shadow);
    // initialize (if necessary)
    glm::mat4 model = glm::mat4(1.0f);
    /*model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));*/
    shader.setMat4("model", model);

    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    // shader.setMat4("model", glm::mat4(1.0f));
    // shader.setMat4("projection", projection);
     //shader.setMat4("view", view);

     //shader.setVec3("material.diffuse", property.diffuse);
    shader.setVec3("material.specular", property.specular);
    shader.setFloat("material.shininess", property.shininess);

    shader.setVec3("viewPos", cam.Position);
    shader.setVec3("light.position", cam.Position);

    if (cam.view == 0) shader.setVec3("light.direction", cam.Front);
    else shader.setVec3("light.direction", glm::vec3(0.0f, -1.0f, 0.0f));

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
}
