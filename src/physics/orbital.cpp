#include <cmath>
#include <iostream>

#include "../app/app.h"
#include "orbital.h"
#include "atom.h"
#include "../maths/maths.h"

int getAtomicNumber(){
    extern App* appPtr;
    if(appPtr != nullptr){
        int AtomicNumber = appPtr->currentAtom.Z;
        std::cout << "Atomic number: " << AtomicNumber << std::endl;
        //printf("atomic number: %d\n", AtomicNumber);
        return AtomicNumber;
    }
    else{
        #warning App pointer is null. Cannot get atomic number
        return 30; //default to zinc
    }
}
//Schrödinger equation solutions for hydrogen-like atoms
//Zeff is the effective nuclear charge, r is the distance from the nucleus, x, y, z are the Cartesian coordinates of the point where we want to evaluate the wavefunction
//R is the radial part of the wavefunction, Y is the angular part (spherical harmonics)
//Can't use spherical coordinates with trigonometric functions to avoid NaN values due to the float precision limitation

float psi_1s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 29.70f;

    float R = 1.0f/sqrtf(PI)*powf(Zeff/a0, 1.5f)*expf(-Zeff*r/a0);

    return R;
}
float psi_2s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 25.85f;

    float R = (1.0f/4.0f*sqrtf(2.0f*PI))*powf(Zeff/a0, 1.5f)*(2.0f-(Zeff*r/a0))*expf(-Zeff*r/(2.0f*a0));
    return R;
}
float psi_2p_x(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 29.70f;

    float R = (1.0f/(4.0f*sqrtf(2.0f*PI)))*powf(Zeff/a0, 2.5f)*expf(-Zeff*r/2.0f*a0)*r;
    float Y = (sqrtf(x*x + y*y)/r)*(x/sqrtf(x*x + z*z)); //sin(theta)*cos(phi)

    return R*Y;
}
float psi_2p_y(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 29.70f;

    float R = (1.0f/4.0f*sqrtf(2.0f*PI))*powf(Zeff/a0, 2.5f)*expf(-Zeff*r/2.0f*a0)*r;
    float Y =(sqrtf(x*x + y*y)/r)*(y/sqrtf(x*x + z*z)); //sin(theta)*sin(phi)

    return R*Y;
}
float psi_2p_z(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 29.70f;

    float R = (1.0f/(4.0f*sqrtf(2.0f*PI)))*powf(Zeff/a0, 2.5f)*expf((-Zeff*r)/(2.0f*a0))*r;
    float Y = z/r; //cos(theta)

    return R*Y; 
}
float psi_3s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0;
    }

    //Zeff = 18.75f;

    float R = (1.0f/81.0f*sqrtf(3.0f*PI))*powf(Zeff/a0, 1.5f)*(27.0f-18.0f*((Zeff*r/a0)+(2.0f*((Zeff*Zeff)*(r*r))/a0*a0)))*expf(-Zeff*r/3.0f*a0);
    return R;
}
float psi_3p_x(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 18.75f;

    float R = (1.0f/81.0f*sqrtf(PI))*powf(Zeff/a0, 2.5f)*(Zeff*r - ((Zeff*r*r)/a0))*expf(-Zeff*r/3.0f*a0)*r;
    float Y = (sqrtf(x*x + y*y)/r)*(x/sqrtf(x*x + y*y)); //sin(theta)*cos(phi)

    return R*Y;
}
float psi_3p_y(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 18.75f;

    float norm = (1.0f/81.0f*sqrtf(PI));
    float laguerrePolynomial = powf(Zeff/a0, 2.5f)*(6.0f*r - ((3.0f*r*r)/a0))*expf(-3.0f*r/3.0f*a0)*r;
    float R = norm*laguerrePolynomial;
    float Y = (sqrtf(x*x + y*y)/r)*(x/sqrtf(x*x + y*y)); //sin(theta)*cos(phi)

    return R*Y;
}
float psi_3p_z(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 18.75f;

    float norm = (sqrtf(2.0f)/81.0f*sqrtf(PI));
    float laguerrePolynomial = powf(Zeff/a0, 2.5f)*(3.0f*r - (3.0f*r*r)/a0)*expf(-3.0f*r / 3.0f*a0)*r;
    float R = norm*laguerrePolynomial;
    float Y = z/r; //cos(theta)

    return R*Y;
}
float psi_3d_x2y2(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 8.85f;

    float norm = (1.0f/162.0f*sqrtf(PI));
    float laguerrePolynomial = powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*a0)*r*r;
    float R = norm*laguerrePolynomial;
    float Y = (powf(sqrtf(x*x + y*y)/r, 2.0f))*((x*x - y*y)/x*x + y*y); //sin^2(theta)*cos(2*phi)

    return R*Y;
}
float psi_3d_xz(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 8.85f;

    float norm = (1.0f/81.0f*sqrtf(PI));
    float laguerrePolynomial = powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*a0)*r*r;
    float R = norm*laguerrePolynomial;
    float Y = (sqrtf(x*x + y*y)/r)*(z/r)*(x/sqrtf(x*x + y*y)); //sin(theta)*cos(theta)*cos(phi)

    return R*Y;
}
float psi_3d_z2(float x, float y, float z, float Zeff){
    float r =sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 8.85f;

    float norm = (1.0f/81.0f*sqrtf(6.0f*PI));
    float laguerrePolynomial = powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*a0)*r*r;
    float R = norm*laguerrePolynomial;
    float Y = 3.0f*(z/r)*(z/r) - 1.0f; //3*cos^2(theta) - 1

    return R*Y;
}
float psi_3d_yz(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 8.85f;

    float norm = (1.0f/81.0f*sqrtf(PI));
    float laguerrePolynomial = powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*a0)*r*r;
    float R = norm*laguerrePolynomial;
    float Y = (sqrtf(x*x + y*y)/r)*(z/r)*(y/sqrtf(x*x + y*y)); //sin(theta)*cos(theta)*sin(phi)

    return R*Y;
}
float psi_3d_xy(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }

    //Zeff = 8.85f;
    
    float norm = (1.0f/162.0f*sqrtf(PI));
    float laguerrePolynomial = powf(Zeff/a0, 3.5f)*expf(-Zeff*r / 3.0f*a0)*r*r;
    float R = norm*laguerrePolynomial;
    float Y = powf(sqrtf(x*x + y*y)/r, 2.0f)*(y/sqrtf(x*x + y*y)); //sin^2(theta)*sin(phi)

    return R*Y;
}
float psi_4s(float x, float y, float z, float Zeff){
    float r = sqrtf(x*x + y*y + z*z);
    if(r<1e-8f){
        return 0.0f;
    }
    
    //Zeff = 9.45f;

    int AtomicNumber = getAtomicNumber();

    float norm = (1.0f/768.0f*sqrtf(PI))*powf(Zeff/a0, 1.5f);
    float laguerrePolynomial = (24.0f - ((18.0f*Zeff*Zeff)/a0) + ((3.0f*powf(AtomicNumber, 4.0f)*r*r)/a0*a0) - (powf(AtomicNumber, 3.0f)*r*r*r)/4.0f*powf(a0, 4.0f))*expf((-Zeff)/(4.0f*a0));
    float R = norm*laguerrePolynomial;

    return R;
}