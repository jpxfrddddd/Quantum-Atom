#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "../physics/atom.h"
#include "../physics/orb.h"
#include "../camera/camera.h"

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
    unsigned int shaderProgram;
    unsigned int VAO, VBO;
    unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);

    void initOpenGL();
    void renderOrbital(const orbitalGroup& orbital, const Camera& cam);
    void render(const Atom& atom, const Camera& cam);
    float getMaxDensity(const Atom& atom, float box_size);
    float getOrbitalDensity(const orbitalGroup& orbital, float box_size);
    float getBoxSize(const Atom& atom);
    float time = (float)(glfwGetTime());

    void generateCloud(const Atom& atom);
    void generateOrbital(const orbitalGroup& orbitalGenerated);
};