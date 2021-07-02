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


class Cam {

public:
	Cam() {

	}
	Camera cam = Camera(position);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	int view = 0;


	void init() {

	}


	void changeView() {

	}
};