#pragma once
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Camera
{
    float x, y, z;
    float pitch;
    float yaw = 690.0f;
    double lastX = 960.0;
    double lastY = 540.0;

    Camera(float px, float py, float pz) : x(px), y(py), z(pz), pitch(0.0f), yaw(0.0f) {}
    
    void getForward(float &fx, float &fy, float &fz){
        float pitchRad = pitch*M_PI/180.0f;
        float yawRad = yaw*M_PI/180.0f;
        fx = cosf(pitchRad)*sinf(yawRad);
        fy = -sinf(pitchRad);
        fz = cosf(pitchRad)*cosf(yawRad);
    }

    void getRight(float &rx, float &ry, float &rz){
        float yawRad = yaw*M_PI/180.0f;
        rx = cosf(yawRad);
        ry = 0.0f;
        rz = -sinf(yawRad);
    }

    void rotate(float deltaPitch, float deltaYaw){
        pitch += deltaPitch;
        yaw += deltaYaw;
        if(pitch > 89.0f){
            pitch = 89.0f;
        }
        if(pitch < -89.0f){
            pitch = -89.0f;
        }
    }
};

void createViewMatrix(float *m, Camera &cam);
void perspective(float *m, float fov, float aspect, float near, float far); 


