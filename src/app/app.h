#pragma once

#include "../physics/atom.h"
#include "../renderer/renderer.h"
#include "../physics/orb.h"

class App{
public:
    App* appPtr = nullptr;
    Atom currentAtom;
    orbitalGroup currentOrbital;
    Camera cam;
    Renderer renderer;

    App(Atom atom, Camera cam, orbitalGroup generatedOrbital) : currentAtom(atom), cam(0.0f, 0.0f, 0.0f), currentOrbital(generatedOrbital) {}

    void keyCallback();
    void mouseCallback();
    void init();
    void update();
    void render();
};