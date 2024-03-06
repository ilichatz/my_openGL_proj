#include <iostream>
#include "./glad.h"
#include "GLFW/glfw3.h"



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}



int main() {
    std::cout << "Hello World" << std::endl;
    std::cout << "________" << std::endl;
    std::cout << "We start glfw below..." << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1; 

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }



    /* FInd and load OpenGL defintions*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Couldn't load opengl (glad didn't initialize)" << std::endl;
        glfwTerminate();
        return -1;

    }

    /* Change background color */
    glClearColor(0.75f, 0.5f, 0.15f, 1.0f);
       
   
    glfwSetKeyCallback(window, key_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();  // make sure it is first in the loop else 100% cpu-core usage
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}