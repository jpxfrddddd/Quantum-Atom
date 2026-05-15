#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <iostream>
#include <cstring>

#include "app/app.h"
#include "physics/atom.h"
#include "renderer/renderer.h"
#include "physics/orbital.h"
#include "camera/camera.h"
#include "physics/orb.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

const char* vertexShader = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 pointColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_PointSize = 1.2;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    pointColor = aColor;
}
)";

const char* fragmentShader = R"(
#version 330 core

in vec4 pointColor;
out vec4 FragColor;

void main(){
    FragColor = pointColor;
}
)";

bool displayed1s = true;
bool displayed2s = false;
bool displayed2p = false;
bool displayed3s = false;
bool displayed3p = false;
bool displayed3d = false;
bool last3d = false;

Camera cam(0.0f, 0.0f, 0.0f);
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

unsigned int compileShader(unsigned int type, const char* source){
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    int succes;
    glGetShaderiv(id, GL_COMPILE_STATUS, &succes);
    if(!succes){
        char infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cout << "Shader compilation error: " << infoLog << std::endl;
    }
    return id;
}

unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource){
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main(){
    if(!glfwInit()){
        std::cout << "Window error\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    unsigned int program = createShaderProgram(vertexShader, fragmentShader);
    //app.renderer.render(currentAtom, app.cam);
    //app.renderer.generateCloud(currentAtom);
    app.renderer.shaderProgram = program;
    app.renderer.initOpenGL();
    
    app.renderer.renderOrbital(currentOrbital, app.cam);
    app.renderer.generateOrbital(currentOrbital);

    double lastTime = glfwGetTime();
    //double x, y;
    //glfwGetCursorPos(window, &x, &y);
    //app.cam.lastX = x;
    //app.cam.lastY = y;
    //app.cam.z = 50.0f;

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_PROGRAM_POINT_SIZE);
        
        float time = (float)(glfwGetTime());
        float dt = (time - lastTime);
        lastTime = time;
        float speed = 10.0f * dt;
        
        /*if(keys[GLFW_KEY_D]){ //debugg camera position and rotation
            std::cout << "Camera position: (" << app.cam.x << ", " << app.cam.y << ", " << app.cam.z << ")\n";
            std::cout << "Camera rotation: (Pitch: " << app.cam.pitch << ", Yaw: " << app.cam.yaw << ")\n";
            keys[GLFW_KEY_D] = false;
        } */

        //std::cout << "Yaw: " << cam.yaw << std::endl;
        //std::cout << cam.z << std::endl;
        if(keys[GLFW_KEY_LEFT_CONTROL]){
            speed *= 5.0f;
        }

        float fx, fy, fz, rx, ry, rz;

        cam.getForward(fx, fy, fz);
        cam.getRight(rx, ry, rz);

        //movement

        /*if(keys[GLFW_KEY_SPACE]){
            app.cam.y += speed;
        }
        if(keys[GLFW_KEY_LEFT_SHIFT]){
            app.cam.y -= speed;
        }

        if(keys[GLFW_KEY_UP]){
            app.cam.x -= fx*speed;
            app.cam.z -= fz*speed;
        }

        if(keys[GLFW_KEY_DOWN]){
            app.cam.x += fx*speed;
            app.cam.z += fz*speed;
        }*/

        if(keys[GLFW_KEY_ESCAPE]){
            glfwTerminate();
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
            displayed3d = false;
            last3d = false;
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
            displayed3d = false;
            last3d = false;

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
            displayed3d = false;
            last3d = false;

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
            last3d = false;
            displayed3d = false;
            displayed3p = false;
            displayed3s = true;
            displayed2p = false;
            displayed2s = false;
            displayed1s = false;
        }
        if(displayed3s && keys[GLFW_KEY_P]){
            currentOrbital = Orbital_3p();
            app.renderer.generateOrbital(currentOrbital);
            keys[GLFW_KEY_P] = false;
            displayed1s = false;
            displayed2s = false;
            displayed2p = false;
            displayed3s = false;
            displayed3p = true;
            displayed3d = false;
            last3d = false;
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

        /*if(displayed3s || displayed3p && keys[GLFW_KEY_LEFT]){
            currentOrbital = Orbital_3d();
            app.renderer.generateOrbital(currentOrbital);
            keys[GLFW_KEY_LEFT] = false;

            displayed3s = false; 
            displayed3p = false;
            displayed3d = true;
        } */

        if(displayed3d){
            displayed1s = false;
            displayed2s = false;
            displayed2p = false;
            displayed3s = false;
            displayed3p = false;

            if(keys[GLFW_KEY_LEFT]){ currentOrbital = Orbital_3d(); app.renderer.generateOrbital(currentOrbital); keys[GLFW_KEY_LEFT] = false;}
            if(keys[GLFW_KEY_LEFT]){ currentOrbital = Orbital_3d_x2y2(); app.renderer.generateOrbital(currentOrbital); keys[GLFW_KEY_LEFT] = false;}
            if(keys[GLFW_KEY_LEFT]){ currentOrbital = Orbital_3d_xz(); app.renderer.generateOrbital(currentOrbital); keys[GLFW_KEY_LEFT] = false;}
            if(keys[GLFW_KEY_LEFT]){ currentOrbital = Orbital_3d_yz(); app.renderer.generateOrbital(currentOrbital); keys[GLFW_KEY_LEFT] = false;}
            if(keys[GLFW_KEY_LEFT]){ currentOrbital = Orbital_3d_xy(); app.renderer.generateOrbital(currentOrbital); keys[GLFW_KEY_LEFT] = false; last3d = true;}
        }

        if(last3d){
            if(keys[GLFW_KEY_LEFT]){
                currentOrbital = Orbital_3d();
                app.renderer.generateOrbital(currentOrbital);

                keys[GLFW_KEY_LEFT] = false;
                last3d = false;
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