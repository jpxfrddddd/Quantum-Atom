#include "renderer.h"
#include "../physics/orbital.h"
#include <GL/gl.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "../physics/atom.h"

static float g_maxDensity = 0.0f; // global variable to track maximum density for scaling

float Renderer::getMaxDensity(const Atom& atom, float box_size){
    float max_d = 0.0001f;
    for(int i = 0; i < 20000; i ++){
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
}

void Renderer::render(const Atom& atom){
    float box_size = 15.0f;
    static std::string last_atom = "";
    static float auto_max_density = 1.0f;

    if(last_atom != atom.name){
        auto_max_density = getMaxDensity(atom, box_size);
        last_atom = atom.name;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPointSize(1.2f);
    glBegin(GL_POINTS);

    for(int i = 0; i < 800000; i++){
        float x = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float y = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float z = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float density = 0.0f;

        for(const auto& orb : atom.orbitals){
            float Zeff = atom.getZeff(orb.n, orb.l);
            float psi = orb.psi(x, y, z, Zeff);
            density += orb.electrons *psi*psi;
        }
        if(std::isnan(density || std::isinf(density))){
            continue; //skip NaN values
        }
        float p = (float)rand() / RAND_MAX * auto_max_density;
        if(p < density){
            float r_dist = sqrt(x*x + y*y + z*z);
            float intensity = exp(-r_dist / 3.0f);
            glColor3f(intensity, 0.4f * intensity, 1.0f - intensity);
            glVertex3f(x, y, z);
        }
    }

    glEnd();
    glDisable(GL_BLEND);
} 