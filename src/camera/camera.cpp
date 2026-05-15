#include <cmath>
#include <cstring>

#include "camera.h"

void perspective(float *m, float fov, float aspect, float near, float far){
    float f = 1.0f/tanf(fov*0.5f*PI/180.0f);
    memset(m, 0, 16*sizeof(float));
    m[0] = f/aspect;
    m[5] = f;
    m[10] = (far+near)/(near-far);
    m[11] = -1.0f;
    m[14] = (2.0f*far*near)/(near-far);
}

void createViewMatrix(float *m, Camera &cam){
    float pitchRad = cam.pitch*PI/180.0f;
    float yawRad = cam.yaw*PI/180.0f;

    float cosPitch = cosf(pitchRad);
    float sinPitch = sinf(pitchRad);
    float cosYaw = cosf(yawRad);
    float sinYaw = sinf(yawRad);

    float xAxis[3] = {cosYaw, 0.0f, -sinYaw};
    float yAxis[3] = {sinYaw*sinPitch, cosPitch, cosYaw*sinPitch};
    float zAxis[3] = {sinYaw*cosPitch, -sinPitch, cosYaw*cosPitch};

    m[0] = xAxis[0]; m[1] = yAxis[0]; m[2] = zAxis[0]; m[3] = 0.0f;
    m[4] = xAxis[1]; m[5] = yAxis[1]; m[6] = zAxis[1]; m[7] = 0.0f;
    m[8] = xAxis[2]; m[9] = yAxis[2]; m[10] = zAxis[2]; m[11] = 0.0f;
    m[12] = -(xAxis[0]*cam.x + xAxis[1] * cam.y + xAxis[2] * cam.z);
    m[13] = -(yAxis[0]*cam.x + yAxis[1] * cam.y + yAxis[2] * cam.z);
    m[14] = -(zAxis[0]*cam.x + zAxis[1] * cam.y + zAxis[2] * cam.z);
    m[15] = 1.0f;
} 