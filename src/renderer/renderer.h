#pragma once
#include "../physics/atom.h"
#include <GLFW/glfw3.h>
#include "camera.h"
#include <vector>

void initOpenGL();

struct ElectronPoint{
    float x, y, z;
    float r, g, b, a;
};

class Renderer{
private:
    std::vector<ElectronPoint> pointCloud;
public:
    void render(const Atom& atom, const Camera& cam);
    float getMaxDensity(const Atom& atom, float box_size);
    float time = (float)(glfwGetTime());

    void generateCloud(const Atom& atom);
};