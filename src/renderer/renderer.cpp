#include "renderer.h"
#include "../physics/orbital.h"
#include <GL/gl.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "../physics/atom.h"
#include "camera.h"
#include "../physics/orb.h"

/*float Renderer::getMaxDensity(const Atom& atom, float box_size){
    float max_d = 0.0001f;
    for(int i = 0; i < 2000000; i ++){
        float x = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float y = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float z = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;

        float current_d = 0.0f;
        for(const auto& orb : atom.orbitals){
            float Zeff = atom.getZeff(orb.n, orb.l);
            float p = orb.psi(x, y, z, Zeff);

            current_d += orb.electrons * p*p;
        }
        if(current_d > max_d){
            max_d = current_d;
        }
    }
    return max_d;
}*/

float Renderer::getMaxDensity(const Atom& atom, float box_size){
    float max_d = 1.0f;

    if(atom.Z > 0 && atom.Z < 3){
        max_d = 0.1f;
    }
    if(atom.Z > 2 && atom.Z < 5){
        max_d = 2.0f;
    }
    if(atom.Z > 4 && atom.Z < 8){
        max_d = 10.0f;
    }
    if(atom.Z > 7 && atom.Z < 10){
        max_d = 20.0f;
    }
    if(atom.Z >= 10 && atom.Z <= 15){
        max_d = 30.0f;
    }
    if(atom.Z > 15 && atom.Z <= 20){
        max_d = 32.0f;
    }
    return max_d;
}

float Renderer::getBoxSize(const Atom& atom){
    float box_size = 8.0f;

    if(atom.Z > 0 && atom.Z < 3){
        box_size = 8.0f;
    }
    if(atom.Z > 2 && atom.Z < 8){
        box_size = 14.0f;
    }
    if(atom.Z > 7 && atom.Z < 10){
        box_size = 16.0f;
    }
    if(atom.Z >= 10 && atom.Z <= 15){
        box_size = 50.0f;
    }
    if(atom.Z > 15 && atom.Z <= 20){
        box_size = 55.0f;
    }


    

    return box_size;
}

void Renderer::generateCloud(const Atom& atom){
    pointCloud.clear();

    float box_size = getBoxSize(atom);
    float auto_max_density = getMaxDensity(atom, box_size);

    for(int i = 0; i < 2000000; i++){
        float x = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float y = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float z = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;

        float density = 0.0f;

        for(const auto& orb : atom.orbitals){
            float Zeff = atom.getZeff(orb.n, orb.l);
            float psi = orb.psi(x, y, z, Zeff);
            density += orb.electrons * psi*psi; // psi^2: probabilty of presence
        }
  

        if(std::isnan(density) || std::isinf(density)){
            continue; //skip NaN values
        }

        float p = (float)rand() / RAND_MAX * auto_max_density;

        if(p < density){
            float r_dist = sqrtf(x*x + y*y + z*z);
            float intensity = expf(-r_dist/3.0f);
            pointCloud.push_back({x, y, z, intensity, 0.4f * intensity, 1.0f - intensity, 1.0f});
        }
    }
    //std::cout << "Points: " << pointCloud.size() << std::endl;
    //std::cout << auto_max_density << std::endl;
}

float getOrbitalBoxSize(const orbitalGroup& orbital){
    float box_size;

    if(orbital.name == "orbital_1s"
    || orbital.name == "orbital_2p_x"
    || orbital.name == "orbital_2p_z"){
        box_size = 8.0f;
    }

    if(orbital.name == "orbital_2s"
    || orbital.name == "orbital_2p"
    || orbital.name == "orbital_3p_z"
    || orbital.name == "orbital_3p_y"){
        box_size = 12.0f;
    }

    if(orbital.name == "orbital_2p_y"){
        box_size = 15.0f;
    }

    if(orbital.name == "orbital_3s"
    || orbital.name == "orbital_3p"
    || orbital.name == "orbital_3p_x"){
        box_size = 20.0f;
    }



    return box_size;
}

void Renderer::generateOrbital(const orbitalGroup& orbitalGenerated){
    orbitalCloud.clear();

    float box_size = getOrbitalBoxSize(orbitalGenerated);
    float auto_max_density = 0.1f;

    for(int i = 0; i < 2000000; i++){
        float x = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float y = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float z = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;

        float density = 0.0f;
        for(const auto& orb : orbitalGenerated.orbitalVec){
            float Zeff = orbitalGenerated.Z;
            float psi = orb.psi(x, y, z, Zeff);
            density += orb.electrons * psi*psi;
        }

        if(std::isnan(density) || std::isinf(density)){
            continue; // skip NaN values
        }

        float p = (float)rand() / RAND_MAX * auto_max_density;

        if(p < density){
            float r_dist = sqrtf(x*x + y*y + z*z);
            float intensity = expf(-r_dist/3.0f);
            orbitalCloud.push_back({x, y, z, intensity, 0.4f * intensity, 1.0f - intensity, 1.0f});
        }
    }
    //std::cout << "points: " << orbitalCloud.size() << std::endl;
}

void Renderer::render(const Atom& atom, const Camera& cam){
    float proj[16];
    float fov = 45.0f;
    float aspect = 1920.0f/1080.0f;
    float zNear = 0.1f;
    float zFar = 1000.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(proj, fov, aspect, zNear, zFar);
    glLoadMatrixf(proj);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-cam.pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-cam.yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-cam.x, -cam.y, -cam.z); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPointSize(1.2f);
    glBegin(GL_POINTS);

    for(const auto& pt : pointCloud){
        glColor4f(pt.r, pt.g, pt.b, pt.a);
        glVertex3f(pt.x, pt.y, pt.z);
    }

    glRotatef(time * 20.0f, 0, 1, 0);
    glEnd();
    glDisable(GL_BLEND);
} 

void Renderer::renderOrbital(const orbitalGroup& generatedOrbital, const Camera& cam){
    float proj[16];
    float fov = 45.0f;
    float aspect = 1920.0f/1080.0f;
    float zNear = 0.1f;
    float zFar = 1000.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(proj, fov, aspect, zNear, zFar);
    glLoadMatrixf(proj);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-cam.pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-cam.yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-cam.x, -cam.y, -cam.z); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPointSize(1.2f);
    glBegin(GL_POINTS);

    for(const auto& pt : orbitalCloud){
        glColor4f(pt.r, pt.g, pt.b, pt.a);
        glVertex3f(pt.x, pt.y, pt.z);
    }

    glEnd();
    glDisable(GL_BLEND);
} 
