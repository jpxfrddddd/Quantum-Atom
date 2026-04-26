#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer/renderer.h"
#include "physics/orbital.h"
#include <iostream>
#include <GL/gl.h>
#include "app/app.h"
#include "physics/atom.h"
#include <cstring>
#include "renderer/camera.h"
#include "physics/orb.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

bool displayed1s = true;
bool displayed2s = false;
bool displayed2p = false;
bool displayed3s = false;
bool displayed3p = false;
bool displayed3d = false;

Camera cam(0.0f, 0.0f, 60.0f);
bool keys[1024] = {false};
bool mouseCaptured = true;
extern App* appPtr;

void keyCallback(GLFWwindow *w, int key, int s, int action, int m){
    
    if(key >= 0 && key < 1024){
        if(action == GLFW_PRESS){
            keys[key] = true;
        }
        else if(action == GLFW_RELEASE){
            keys[key] = false;
        }
    }
}

void mouseCallback(GLFWwindow *w, double xpos, double ypos){
    static bool firstMouse = true;
    if(!mouseCaptured){
        return;
    }
    if(firstMouse){
        appPtr->cam.lastX = xpos;
        appPtr->cam.lastY = ypos;
        firstMouse = false;
        return;
    }
    double xoffset = appPtr->cam.lastX - xpos;
    double yoffset = appPtr->cam.lastY - ypos;

    appPtr->cam.lastX = xpos;
    appPtr->cam.lastY = ypos;
    

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    appPtr->cam.yaw += xoffset;
    appPtr->cam.pitch += yoffset;

    if(appPtr->cam.pitch > 89.0f){
        appPtr->cam.pitch = 89.0f;
    }
    if(appPtr->cam.pitch < -89.0f){
        appPtr->cam.pitch = -89.0f;
    }
    
} 

int main(){
    if(!glfwInit()){
        std::cout << "Window error\n";
        return -1;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWwindow* window = glfwCreateWindow(
        1920,
        1080,
        "Quantum Atom",
        NULL,
        NULL
    );
    //glfwSetWindowMonitor(window, monitor, 0, 0, 1920, 1080, GLFW_REFRESH_RATE);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    Atom currentAtom = Chlorine();
    orbitalGroup currentOrbital = Orbital_1s();
    App app(currentAtom, cam, currentOrbital);
    appPtr = &app;
    //glfwSetWindowUserPointer(window, &app);
    app.init();
    //app.renderer.render(currentAtom, app.cam);
    //app.renderer.generateCloud(currentAtom);
    app.renderer.renderOrbital(currentOrbital, app.cam);
    app.renderer.generateOrbital(currentOrbital);

    double lastTime = glfwGetTime();
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    app.cam.lastX = x;
    app.cam.lastY = y;
    app.cam.z = 50.0f;

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        float time = (float)(glfwGetTime());
        float dt = (time - lastTime);
        lastTime = time;
        float speed = 10.0f * dt;
        
        //std::cout << "Yaw: " << cam.yaw << std::endl;
        //std::cout << cam.z << std::endl;
        if(keys[GLFW_KEY_LEFT_CONTROL]){
            speed *= 5.0f;
        }

        float fx, fy, fz, rx, ry, rz;

        cam.getForward(fx, fy, fz);
        cam.getRight(rx, ry, rz);
        /*if(keys[GLFW_KEY_Z] || keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]){
            app.cam.x -= fx*speed;
            app.cam.z -= fz*speed;
        }

        if(keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]){
            app.cam.x += fx*speed;
            app.cam.z += fz*speed;
        }
        if(keys[GLFW_KEY_Q] || keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]){
            app.cam.x -= rx*speed;
            app.cam.z -= rz*speed;
        }
        if(keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]){
            app.cam.x += rx*speed;
            app.cam.z += rz*speed; 
        } */


        //movement

        if(keys[GLFW_KEY_SPACE]){
            app.cam.y += speed;
        }
        if(keys[GLFW_KEY_LEFT_SHIFT]){
            app.cam.y -= speed;
        }
        if(keys[GLFW_KEY_ESCAPE]){
            glfwTerminate();
        } 

        if(keys[GLFW_KEY_UP]){
            app.cam.x -= fx*speed;
            app.cam.z -= fz*speed;
        }

        if(keys[GLFW_KEY_DOWN]){
            app.cam.x += fx*speed;
            app.cam.z += fz*speed;
        }

        //Orbital switching

        if(!displayed1s && keys[GLFW_KEY_1]){
            currentOrbital = Orbital_1s();
            app.renderer.generateOrbital(currentOrbital);
            keys[GLFW_KEY_1] = false;
            displayed1s = true;
            displayed2s = false;
            displayed2p = false;
            displayed3s = false;
            displayed3p = false;
            
        }


        if(keys[GLFW_KEY_2]){
            currentOrbital = Orbital_2s();
            app.renderer.generateOrbital(currentOrbital);
            keys[GLFW_KEY_2] = false;
            displayed2s = true;
        }
        if(displayed2s){
            displayed1s = false;
            displayed2p = false;
            displayed3s = false;
            displayed3p = false;

            if(keys[GLFW_KEY_P]){
                currentOrbital = Orbital_2p();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_P] = false;
                displayed2p = true;
            }
        }
        if(displayed2p){
            displayed1s = false;
            displayed2s = false;
            displayed3s = false;
            displayed3p = false;

            if(keys[GLFW_KEY_X]){
                currentOrbital = Orbital_2p_x();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_X] = false;
            }
            if(keys[GLFW_KEY_Z] || keys[GLFW_KEY_W]){
                currentOrbital = Orbital_2p_z();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_Z] = false;
                keys[GLFW_KEY_W] = false;
            }
            if(keys[GLFW_KEY_Y]){
                currentOrbital = Orbital_2p_y();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_Y] = false;
            }
            if(keys[GLFW_KEY_S]){
                currentOrbital = Orbital_2s();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_S] = false;
                displayed2s = true;
                displayed2p = false;
            }
            else if(keys[GLFW_KEY_P]){
                currentOrbital = Orbital_2p();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_P] = false;
                displayed2p = true;
                displayed2s = false;
            }

        }


        if(!displayed3s && keys[GLFW_KEY_3]){
            currentOrbital = Orbital_3s();
            app.renderer.generateOrbital(currentOrbital);
            keys[GLFW_KEY_3] = false;
            displayed3p = false;
            displayed3s = true;
            displayed2s = false;
            displayed1s = false;
        }
        if(displayed3s && keys[GLFW_KEY_P]){
            currentOrbital = Orbital_3p();
            app.renderer.generateOrbital(currentOrbital);
            keys[GLFW_KEY_P] = false;
            displayed3p = true;
        }
        if(displayed3p){
            if(keys[GLFW_KEY_X]){
                currentOrbital = Orbital_3p_x();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_X] = false;
            }
            if(keys[GLFW_KEY_Z] || keys[GLFW_KEY_W]){
                currentOrbital = Orbital_3p_z();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_Z] = false;
                keys[GLFW_KEY_W] = false;
            }
            if(keys[GLFW_KEY_Y]){
                currentOrbital = Orbital_3p_y();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_Y] = false;
            }
            if(keys[GLFW_KEY_S]){
                currentOrbital = Orbital_3s();
                app.renderer.generateOrbital(currentOrbital);
                keys[GLFW_KEY_S] = false;
                displayed3s = true;
                displayed3p = false;
            }
        }

        app.update();
        app.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}