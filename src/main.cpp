#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer/renderer.h"
#include "physics/orbital.h"
#include <iostream>
#include <GL/gl.h>
#include "app/app.h"
#include "physics/atom.h"

int main(){
    if(!glfwInit()){
        std::cout << "Window error\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 800, "Quantum Atom", NULL, NULL);
    if(!window){
        std::cout << "Window error\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    float ratio = 800.0f / 1280.0f;

    Atom currentAtom = Magnesium();
    App app(currentAtom);
    app.init();

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
  

        //float time = (float)(glfwGetTime());
        app.update();
        app.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

   
    glfwTerminate();
    return 0;
}