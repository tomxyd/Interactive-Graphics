#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>

#define BUFFER_OFFSET(bytes) ((GLvoid*) (bytes))
typedef glm::vec3 vec3;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void processInput(GLFWwindow* window);
void init();
void display(const Shader& shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


unsigned int VBO;
unsigned int VAO;


vec3 points[3];
int w = SCR_WIDTH;
int h = SCR_HEIGHT;
int count = 0;
bool readyToDraw = false;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Interaction", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    Shader ourShader(RESOURCES_PATH "vertex.vc", RESOURCES_PATH "fragment.fc");
    ourShader.use();
    //init();


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);
        display(ourShader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// processes all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void init() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
}

void display(const Shader& shader) {

    glClearColor(0.f, 0.f, 0.f, 1.f);

    float const PI = 22 / 7;
    const float DegreesToRadians = PI / 180.f;
    float angle = 0.001 * DegreesToRadians;

    for (int i = 0; i < 3; ++i) {
        float x = std::cos(angle) * points[i].x - std::sin(angle) * points[i].y;
        float y = std::sin(angle) * points[i].x + std::cos(angle) * points[i].y;
        points[i].x = x;
        points[i].y = y;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        exit(0);

    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << count << std::endl;
        points[count].x = (float)xPos / (w / 2) - 1.0;
        points[count].y = (float)(h - yPos) / (h / 2) - 1.0;
        count++;
    }

    if (count == 3)
    {
        init();
        readyToDraw = true;
        count = 0;
    }
}

