#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../header/shader_m.h"
#include "../header/stb_image.h"
#include "../header/camera.h"
#include "../header/Util.h"

#include <iostream>

#include "../header/Plane/plane.h"
#include "../header/Model.h"
#include "../header/Wall/wall.h"
#include "../header/Light/Light.h"
#include "../header/Compass/Compass.h"
#include "../header/Character.h"
#include "../header/Light/SpotLight.h"
#include "../header/Rock/Rock.h"
#include "../header/Skybox/Skybox.h"
#include "../header/Score/Score.h"

#include "../header/Star.h"
//#include "../Character.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void initDepthMap(unsigned int& depthFBO, unsigned int& depthMap);
unsigned int loadTexture(const char* path);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float SIZE = 6;


const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
int winScore = 5;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Camera cam = Camera();

Character character = Character(0);

Light light = Light();
Wall wall = Wall(SIZE);


SpotLight spotLight = SpotLight();
int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    
    Rock rock = Rock(0);
    Star star = Star(5, SIZE);
    
    Shader lightShader("header/Light/light.vs", "header/Light/light.fs");
    Shader depthShader("header/shadow.vs","header/shadow.fs");
    
    Shader planeShader("header/Plane/plane.vs", "header/Plane/plane.fs");
    Plane plane = Plane(SIZE);

    Shader wallShader = Shader("header/Plane/plane.vs", "header/Plane/plane.fs");

    Shader charShader("header/Plane/plane.vs", "header/Plane/plane.fs");
    character = Character(SIZE);

    Shader compassShader("header/Compass/compass.vs", "header/Compass/compass.fs");
    Compass compass = Compass();

    spotLight = SpotLight();

    Shader rockShader("header/Rock/rock.vs", "header/Rock/rock.fs");
   
    Shader skyboxShader("header/Skybox/skybox.vs", "header/Skybox/skybox.fs");
    SkyBox skybox = SkyBox();

    Shader scoreShader("header/Score/score.vs", "header/Score/score.fs");
    Score score = Score(winScore);


    Shader shader("header/Plane/plane.vs", "header/Plane/plane.fs");
    Shader simpleDepthShader("header/shadow.vs", "header/shadow.fs");

 
    
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO, depthMapFBOStar;
    // create depth texture
    unsigned int depthMap, depthMapStar;
    initDepthMap(depthMapFBO, depthMap);
    initDepthMap(depthMapFBOStar, depthMapStar);
   glEnable(GL_DEPTH_TEST);
   
   
    light.init(lightShader);
    plane.init(planeShader);
    character.init(charShader);
    compass.init(compassShader);
    //wall.init(wallShader);
    rock.init(rockShader);
    skybox.init(skyboxShader);
    score.init(scoreShader);
    
 
    shader.use();
    //shader.setInt("diffuseTexture", 0);
    shader.setInt("shadowMapStar",2);
    shader.setInt("shadowMap", 1);
    
   
    
    star.init(shader, wall);
    wall.init(shader);
    


    //star.init(rockShader, wall);
    while (!glfwWindowShouldClose(window))
    {   
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.001f, 100.0f);
        glm::mat4 view = cam.GetViewMatrix(character.position,character.front,character.up);
        
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 0.1f, far_plane = 7.5f;
        lightProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);
            //glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
            //glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, near_plane, far_plane);
          //glm::perspective(glm::radians(5.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
           
        lightView = glm::lookAt(cam.Position, cam.Position + glm::vec3(cam.Front.x,0,cam.Front.z), cam.Up);
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5));
        simpleDepthShader.setMat4("model", model);
       // glBindVertexArray(planeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        wall.draw(simpleDepthShader);
        //star.draw(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOStar);
        glClear(GL_DEPTH_BUFFER_BIT);
        model = glm::scale(glm::mat4(1.0f), glm::vec3(5));
        simpleDepthShader.setMat4("model", model);
        wall.draw(simpleDepthShader);
        star.draw(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        
       /* glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);*/
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthMapStar);
        //basic.use();
        model = glm::scale(glm::mat4(1), glm::vec3(5));
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set light uniforms
        shader.setVec3("viewPos", cam.Position);
        shader.setVec3("lightPos", cam.Position);
        shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shader.setMat4("model", model);

        shader.setBool("drawShadow", true);
        plane.draw(shader, projection, view, light, spotLight, cam, lightSpaceMatrix);
       //glDrawArrays(GL_TRIANGLES, 0, 6);
        wall.draw(light,spotLight,shader, lightSpaceMatrix, projection, view, cam);
        model = glm::scale(glm::mat4(1), glm::vec3(0.01, 0.01, 0.01));
        star.draw(shader, lightSpaceMatrix, projection, view, cam, light, spotLight);
       
  
        //ight.draw(lightShader, projection, view, cam);
        character.draw(wallShader, projection, view, light,spotLight, cam, deltaTime);
        compass.draw(compassShader, character.position, cam.Yaw);
        score.draw(scoreShader, projection, view);
        rock.draw(rockShader, projection, view, light, spotLight, cam, lightSpaceMatrix);
   
        if (star.isCollect(character)== true) {
            score.update();

            if (score.win()) {
                //std::cout << "win" << std::endl;
                spotLight.on();
            
         }
        }
        
        
        glDepthFunc(GL_LEQUAL);
        skybox.draw(skyboxShader, cam, character, projection);
        glDepthFunc(GL_LESS); // set depth function back to default*/

       

        glfwSwapBuffers(window);
        glfwPollEvents();

    
    }


    
    //plane.clear();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        character.move(FORWARD, deltaTime,wall);

        cam.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        character.move(BACKWARD, deltaTime, wall);
        cam.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        character.move(LEFT, deltaTime, wall);
        cam.ProcessKeyboard(LEFT, deltaTime);
    }
        
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        character.move(RIGHT, deltaTime, wall);
        cam.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        character.setJump();
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        
        light.changeView();
      cam.changeView();
        character.changeView(light.view);
    }
}
void initDepthMap(unsigned int &depthFBO, unsigned int &depthMap) {
   
    glGenFramebuffers(1, &depthFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
        SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
        depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    character.processMovement(xoffset, yoffset,cam.view);
    cam.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.ProcessMouseScroll(yoffset);
}
