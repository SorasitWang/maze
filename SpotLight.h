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
#include <string>

class SpotLight {

public:
    SpotLight() {

    };


    struct property {
        glm::vec3 position = glm::vec3(-5.3, 3.0, -5.65);
        glm::vec3 ambient = glm::vec3(0.0f);
        glm::vec3 diffuse = glm::vec3(0.0f);
        glm::vec3 specular = glm::vec3(0.0f);
        glm::vec3 direction = glm::vec3(0.0f);
        float cutoff = 5.0f;
        float outerCutoff = 25.0f;
    }property;

    void on() {
        glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
        glm::vec3 diffuse = glm::vec3(3.3f, 3.3f, 3.3f);
        glm::vec3 specular = glm::vec3(0.9f, 0.9f, 0.9f);
    }

    void off() {
        glm::vec3 ambient = glm::vec3(0.0f);
        glm::vec3 diffuse = glm::vec3(0.0f);
        glm::vec3 specular = glm::vec3(0.0f);
    }
};