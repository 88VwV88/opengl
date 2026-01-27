#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "stb_image.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std::string_literals;

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// gamepad
GLFWgamepadstate gpadState;

// clang-format off
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

  if (gladLoadGL() != GL_TRUE) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Loaded OpenGL: " << glGetString(GL_VERSION) << std::endl;

  stbi_set_flip_vertically_on_load(true);
  {
    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/shader.vs", "shaders/shader.fs");
    Model backpack("models/backpack/backpack.obj");

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
      // light properties
      shader.setVec3("light.position", lightPos);
      shader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
      shader.setVec3("light.diffuse", 0.9f, 0.9f, 0.9f);
      shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
      shader.setFloat("light.constant", 1.0f);
      shader.setFloat("light.linear", 0.09f);
      shader.setFloat("light.quadratic", 0.032f);

      glm::mat4 projection = glm::perspective(
          glm::radians(camera.Zoom),
          static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f,
          100.0f);
      glm::mat4 view = camera.GetViewMatrix();
      shader.setMat4("projection", projection);
      shader.setMat4("view", view);

      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      shader.setMat4("model", model);

      backpack.Draw(shader);

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

  // Gamepad input
  const double DEADZONE = 0.15;
  if (glfwJoystickPresent(GLFW_JOYSTICK_1) &&
      glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &gpadState)) {
      double input = 0.0;
      input = gpadState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
      if (input) {
        if (input > DEADZONE)
          camera.ProcessKeyboard(FORWARD, deltaTime);
        else if (input < -DEADZONE)
          camera.ProcessKeyboard(BACKWARD, deltaTime);
      }
      input = gpadState.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
      if (input) {
        if (input > DEADZONE)
          camera.ProcessKeyboard(RIGHT, deltaTime);
        else if (input < -DEADZONE)
          camera.ProcessKeyboard(LEFT, deltaTime);
      }
      input = gpadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
      if (input < -DEADZONE || input > DEADZONE) {
        camera.ProcessMouseScroll(-static_cast<float>(input));
      }
      input = gpadState.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
      if (input < -DEADZONE || input > DEADZONE) {
        camera.ProcessMouseMovement(static_cast<float>(input), 0.0);
      }
    }
  }

  // Keyboard input
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
