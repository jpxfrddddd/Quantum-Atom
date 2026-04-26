#pragma once
#define PI 3.14159265358979323846f

struct Orbital{
    int n,l,m;
    int electrons;
    float (*psi)(float x, float y, float z, float Zeff);
};