#pragma once

struct Orbital{
    int n,l,m;
    int electrons;
    float (*psi)(float x, float y, float z, float Zeff);
};