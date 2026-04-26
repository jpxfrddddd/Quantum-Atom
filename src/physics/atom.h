#pragma once
#include <vector>
#include <string>
#include "orbital.h"
#include <iostream>

float psi_1s(float x, float y, float z, float Zeff);

float psi_2s(float x, float y, float z, float Zeff);

float psi_2p_x(float x, float y, float z, float Zeff);
float psi_2p_y(float x, float y, float z, float Zeff);
float psi_2p_z(float x, float y, float z, float Zeff);

float psi_3s(float x, float y, float z, float Zeff);

float psi_3p_x(float x, float y, float z, float Zeff);
float psi_3p_y(float x, float y, float z, float Zeff);
float psi_3p_z(float x, float y, float z, float Zeff);

float psi_3d_x2y2(float x, float y, float z, float Zeff);
float psi_3d_xy(float x, float y, float z, float Zeff);
float psi_3d_z2(float x, float y,float z, float Zeff);
float psi_3d_yz(float x, float y, float z, float Zeff);
float psi_3d_xy(float x, float y, float z, float Zeff);

float psi_4s(float x, float y, float z, float Zeff);

struct Atom{
    std::string name;
    int Z;
    std::vector<Orbital> orbitals;

    float getZeff(int n, int l) const{
        float sigma = 2.0f;
        float test = 1.0f;
        float Zeff = sigma - test;
        return Zeff;
    }

    Atom(std::string n, int z, std::vector<Orbital> o) : name(n), Z(z), orbitals(o) {}
};

Atom Hydrogen();
Atom Lithium();
Atom Magnesium();
Atom Chlorine();