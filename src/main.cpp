#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "camera.hpp"
#include "objects.hpp"
#include "shader.hpp"
#include "stb_image.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std::string_literals;

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// clang-format off
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = static_cast<float>(SCR_WIDTH) / 2.0f;
float lastY = static_cast<float>(SCR_HEIGHT) / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

glm::vec3 pointLightPositions[] = {
  glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};
// clang-format on

void processInput(GLFWwindow *pWindow);
void mouse_callback(GLFWwindow *pWindow, double xpos, double ypos);
void scroll_callback(GLFWwindow *pWindow, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow *pWindow, int width, int height);

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto pWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window",
                                  nullptr, nullptr);
  if (pWindow == nullptr) {
    char description[1024];
    glfwGetError(reinterpret_cast<const char **>(&description));
    throw std::runtime_error("GLFW::WINDOW::ERROR "s + description);
  }
  glfwMakeContextCurrent(pWindow);

  glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);
  glfwSetCursorPosCallback(pWindow, mouse_callback);
  glfwSetScrollCallback(pWindow, scroll_callback);

  glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Loaded OpenGL: " << glGetString(GL_VERSION) << std::endl;

  {
    Shader shader("shaders/shader.vs", "shaders/shader.fs");
    Shader lampShader("shaders/light.vs", "shaders/light.fs");

    VAO vao;
    VBO vbo(vertices);

    vao.bind();
    vao.linkAttrib(vbo, 0, 3, 8 * sizeof(float), (void *)0);
    vao.linkAttrib(vbo, 1, 3, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    vao.linkAttrib(vbo, 2, 2, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    VAO lampVao;

    lampVao.bind();
    lampVao.linkAttrib(vbo, 0, 3, 8 * sizeof(float), (void *)0);

    Texture diffuse("textures/container2.png", 0);
    Texture specular("textures/container2_specular.png", 1);

    shader.use();
    // set texture units
    diffuse.setTexUnit(shader, "material.diffuse");
    specular.setTexUnit(shader, "material.specular");

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(pWindow)) {
      // per frame time logic
      // --------------------
      float currentFrame = static_cast<float>(glfwGetTime());
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      // input
      // -----
      processInput(pWindow);

      // render
      // ------
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // activate shader
      shader.use();
      shader.setVec3("viewPos", camera.Position);

      // directional light properties
      shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
      shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
      shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
      shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

      // point light properties
      for (int i = 0; i < 4; ++i) {
        shader.setVec3(std::format("pointLights[{}].position", i),
                       pointLightPositions[i]);
        shader.setVec3(std::format("pointLights[{}].ambient", i), 0.05f, 0.05f,
                       0.05f);
        shader.setVec3(std::format("pointLights[{}].diffuse", i), 0.8f, 0.8f,
                       0.8f);
        shader.setVec3(std::format("pointLights[{}].specular", i), 1.0f, 1.0f,
                       1.0f);
        shader.setFloat(std::format("pointLights[{}].constant", i), 1.0f);
        shader.setFloat(std::format("pointLights[{}].linear", i), 0.09f);
        shader.setFloat(std::format("pointLights[{}].quadratic", i), 0.032f);
      }

      // spot light properties
      shader.setVec3("spotLight.position", camera.Position);
      shader.setVec3("spotLight.direction", camera.Front);

      shader.setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
      shader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
      shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

      shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
      shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
      // attenuation properties
      shader.setFloat("spotLight.constant", 1.0f);
      shader.setFloat("spotLight.linear", 0.09f);
      shader.setFloat("spotLight.quadratic", 0.032f);

      // material properties
      shader.setFloat("material.shininess", 0.25f);

      glm::mat4 projection = glm::perspective(
          glm::radians(camera.Zoom),
          static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f,
          100.0f);
      glm::mat4 view = camera.GetViewMatrix();

      shader.setMat4("projection", projection);
      shader.setMat4("view", view);

      // render boxes
      vao.bind();
      diffuse.bind();
      specular.bind();
      for (unsigned int i = 0; i < 10; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * static_cast<float>(i);
        model = glm::rotate(model, glm::radians(angle),
                            glm::vec3(1.0f, 0.3f, 0.5f));
        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      lampShader.use();
      lampShader.setMat4("projection", projection);
      lampShader.setMat4("view", view);

      // render lamps
      lampVao.bind();
      for (unsigned int i = 0;
           i < sizeof(pointLightPositions) / sizeof(glm::vec3); ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      glfwSwapBuffers(pWindow);
      glfwPollEvents();
    }
  }

  glfwDestroyWindow(pWindow);
  glfwTerminate();
}

void processInput([[maybe_unused]] GLFWwindow *pWindow) {
  if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(pWindow, 1);

  if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_RELEASE)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *pWindow, int width,
                               int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback([[maybe_unused]] GLFWwindow *pWindow, double xposIn,
                    double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback([[maybe_unused]] GLFWwindow *pWindow,
                     [[maybe_unused]] double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
