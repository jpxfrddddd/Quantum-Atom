#pragma once
#include "../physics/atom.h"

void initOpenGL();

class Renderer{
public:
    void render(const Atom& atom);
    float getMaxDensity(const Atom& atom, float box_size);
};