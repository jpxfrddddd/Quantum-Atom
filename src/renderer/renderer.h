#pragma once
#include "../physics/atom.h"
#include <GLFW/glfw3.h>
#include "camera.h"
#include <vector>
#include "../physics/orb.h"

void initOpenGL();

struct ElectronPoint{
    float x, y, z;
    float r, g, b, a;
};

struct OrbitalPoint{
    float x, y, z;
    float r, g, b, a;
};

class Renderer{
private:
    std::vector<ElectronPoint> pointCloud;
    std::vector<OrbitalPoint> orbitalCloud;
public:
    void renderOrbital(const orbitalGroup& orbital, const Camera& cam);
    void render(const Atom& atom, const Camera& cam);
    float getMaxDensity(const Atom& atom, float box_size);
    float getBoxSize(const Atom& atom);
    float time = (float)(glfwGetTime());

    void generateCloud(const Atom& atom);
    void generateOrbital(const orbitalGroup& orbitalGenerated);
};