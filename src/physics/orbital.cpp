#include "orbital.h"
#include "../app/app.h"
#include "atom.h"
#include <cmath>
#include <GL/gl.h>
#include <iostream>

static const float a0 = 1.0f; // Bohr radius

float psi_1s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    return 1.0f/sqrtf(PI)*powf(Zeff/a0, 1.5f)*expf(-Zeff*r/a0);
}
float psi_2s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    return (1.0f/4.0f*sqrtf(2.0f*PI))*powf(Zeff/a0, 1.5f)*(2.0f-(Zeff*r/a0))*expf(-Zeff*r/(2.0f*a0));
}
float psi_2p_x(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    float R = (1.0f/(4.0f*sqrtf(2.0f*PI)))*powf(Zeff/a0, 2.5f)*expf(-Zeff*r/2.0f*a0)*r;
    float Y = (sqrtf(x*x + y*y)/r)*(x/sqrtf(x*x + z*z)); //sin(theta)*cos(phi)

    return R*Y;
}
float psi_2p_y(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    float R = (1.0f/4.0f*sqrtf(2.0f*PI))*powf(Zeff/a0, 2.5f)*expf(-Zeff*r/2.0f*a0)*r;
    float Y =(sqrtf(x*x + y*y)/r)*(y/sqrtf(x*x + z*z)); //sin(theta)*sin(phi)

    return R*Y;
}
float psi_2p_z(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    float R = (1.0f/(4.0f*sqrtf(2.0f*PI)))*powf(Zeff/a0, 2.5f)*expf((-Zeff*r)/(2.0f*a0))*r;
    float Y = z/r; //cos(theta)

    return R*Y; 
}
float psi_3s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0;
    }

    return (1.0f/81.0f*sqrtf(3.0f*PI))*powf(Zeff/a0, 1.5f)*(27.0f-18.0f*((Zeff*r/a0)+(2.0f*((Zeff*Zeff)*(r*r))/a0*a0)))*expf(-Zeff*r/3.0f*a0);
}
float psi_3p_x(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    float R = (1.0f/81.0f*sqrtf(PI))*powf(Zeff/a0, 2.5f)*(Zeff*r - ((Zeff*r*r)/a0))*expf(-Zeff*r/3.0f*a0)*r;
    float Y = (sqrtf(x*x + y*y)/r)*(x/sqrtf(x*x + y*y)); //sin(theta)*cos(phi)

    return R*Y;
}
float psi_3p_y(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    float R = (1.0f/81.0f*sqrtf(PI))*powf(Zeff/a0, 2.5f)*(6*r - ((3.0f*r*r)/a0))*expf(-3.0f*r/3.0f*a0)*r;
    float Y = (sqrtf(x*x + y*y)/r)*(x/sqrtf(x*x + y*y)); //sin(theta)*cos(phi)

    return R*Y;
}
float psi_3p_z(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    float R = (sqrtf(2.0f)/81.0f*sqrtf(PI))*powf(Zeff/a0, 2.5f)*(3.0f*r - (3.0f*r*r)/a0)*expf(-3.0f*r / 3.0f*a0)*r;
    float Y = z/r; //cos(theta)
    return R*Y;
}
float psi_3d_x2y2(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    float rho = sqrtf(x*x + y*y + z*z);
    float theta = acosf(z/rho);
    float phi = atan2f(x, y);

    if (rho < 0.05f) rho = 0.05f;

    float R = (1.0f/162.0f*sqrtf(PI))*powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*a0)*r*r;
    float Y = sinf(theta)*sinf(theta)*cosf(2.0f*phi);

    return R*Y;
}
float psi_3d_xz(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<0.0001f){
        r = 0.000f;
    }

    float rho = sqrtf(x*x + y*y + z*z);
    float theta = acosf(z/rho);
    float phi = atan2f(x, y);

    if (rho < 0.05f) rho = 0.05f;

    float R = (1.0f/81.0f*sqrtf(PI))*powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*a0)*r*r;
    float Y = sinf(theta)*cosf(theta)*cosf(phi);

    return R*Y;
}
float psi_3d_z2(float x, float y, float z, float Zeff){
    float r =sqrtf(x*x + y*y + z*z);
    if(r=0.0001f){
        r=0.0001f;
    }

    float rho = sqrtf(x*x + y*y + z*z);
    float theta = acosf(z/rho);
    float phi = atan2f(x, y);

    if (rho < 0.05f) rho = 0.05f;

    float R = (1.0f/81.0f*sqrtf(6.0f*PI))*powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*a0)*r*r;
    float Y = 3.0*cosf(theta)*cosf(theta) - 1.0f;

    return R*Y;
}
float psi_3d_yz(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<0.0001f){
        r=0.0001f;
    }

    float rho = sqrtf(x*x + y*y + z*z);
    float theta = acosf(z/rho);
    float phi = atan2f(x, y);

    if (rho < 0.05f) rho = 0.05f;

    float R = (1.0f/81.0f*sqrtf(PI))*powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*r)*r*r;
    float Y = sinf(theta)*cosf(theta)*sinf(phi);

    return R*Y;
}
float psi_3d_xy(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<0.0001f){
        r=0.0001f;
    }

    float rho = sqrtf(x*x + y*y + z*z);
    float theta = acosf(z/rho);
    float phi = atan2f(x, y);

    if (rho < 0.05f) rho = 0.05f;

    float R = (1.0f/162.0f*sqrtf(PI))*powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*a0)*r*r;
    float Y = sinf(theta)*sinf(theta)*sinf(2.0f*phi);

    return R*Y;
}