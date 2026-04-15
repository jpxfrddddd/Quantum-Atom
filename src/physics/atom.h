#pragma once
#include <vector>
#include <string>
#include "orbital.h"

float psi_1s(float x, float y, float z, float Zeff);
float psi_2s(float x, float y, float z, float Zeff);
float psi_2p_x(float x, float y, float z, float Zeff);
float psi_2p_y(float x, float y, float z, float Zeff);
float psi_2p_z(float x, float y, float z, float Zeff);
float psi_3s(float x, float y, float z, float Zeff);
float psi_3p_x(float x, float y, float z, float Zeff);
float psi_3p_y(float x, float y, float z, float Zeff);
float psi_3p_z(float x, float y, float z, float Zeff);
float psi_4s(float x, float y, float z, float Zeff);

struct Atom{
    std::string name;
    int Z;
    std::vector<Orbital> orbitals;

    float getZeff(int n, int l) const{ //simplified Slator rule
        float sigma = 0.0f;
        for(const auto& orb : orbitals){
            if(orb.n < n){
                sigma += 0.85f * orb.electrons;
            }
            else if(orb.n == n && !(orb.n == n && orb.l == l)){
                sigma += 0.35f * orb.electrons;
            }
        }
        float Zeff = Z - sigma;
        return Zeff;
    }

    Atom(std::string n, int z, std::vector<Orbital> o) : name(n), Z(z), orbitals(o) {}
};

Atom Hydrogen();
Atom Lithium();
Atom Magnesium();