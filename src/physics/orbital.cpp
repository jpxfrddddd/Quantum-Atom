#include "orbital.h"
#include "../app/app.h"
#include "atom.h"
#include <cmath>
#include <GL/gl.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

static const float a0 = 1.0f; // Bohr radius

float psi_1s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<0.0001f){
        r=0.0001f;
    }
    float a0 = 1.0f;

    return 1.0f/sqrtf(M_PI)*powf(Zeff/a0, 1.5f)*expf(-Zeff*r/a0);
}
float psi_2s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<0.0001f){
        r=0.0001f;
    }
    float a0 = 1.0f;

    return (1.0f/4.0f*sqrtf(2.0f*M_PI))*powf(Zeff/a0, 1.5f)*(2.0f-(Zeff*r/a0))*expf(-Zeff*r/(2.0f*a0));
}
float psi_2p_x(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<0.0001f){
        r=0.0001f;
    }
    float a0 = 1.0f;

    float rho = sqrtf(x*x + y*y + z*z); //spherical coordinates
    float theta = acosf(z/rho);
    float phi = atan2f(y,x);

    return (1.0f/(4.0f*sqrtf(2.0f*M_PI)))*powf(Zeff/a0, 2.5f)*expf(-Zeff*r/2.0f*a0)*r*sinf(theta)*cosf(phi);
}
float psi_2p_y(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<0.0001f){
        r=0.0001f;
    }
    float a0 = 1.0f;

    float rho = sqrtf(x*x + y*y + z*z);
    float theta = acosf(z/rho);
    float phi = atan2f(y,x);

    return (1.0f/4.0f*sqrtf(2.0f*M_PI))*powf(Zeff/a0, 2.5f)*expf(-Zeff*r/2.0f*a0)*r*sinf(theta)*sinf(phi);
}
float psi_2p_z(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<0.0001f){
        r=0.0001f;
    }
    float a0 = 1.0f;

    float rho = sqrtf(x*x + y*y + z*z);
    float theta = acosf(z/rho);

    return (1.0f/4.0f*sqrtf(2.0f*M_PI))*powf(Zeff/a0, 2.5f)*expf(-Zeff*r/2.0f*a0)*r*cosf(theta);
}
float psi_3s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<0.0001f){
        r=0.0001f;
    }
    float a0 = 1.0f;

    return (1.0f/81.0f*sqrtf(3.0f*M_PI))*powf(Zeff/a0, 1.5f)*(27.0f-18.0f*((Zeff*r/a0)+(2.0f*((Zeff*Zeff)*(r*r))/a0*a0)))*expf(-Zeff*r/3.0f*a0);
}

