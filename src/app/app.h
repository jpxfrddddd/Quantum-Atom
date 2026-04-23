#pragma once
#include "../physics/atom.h"
#include "../renderer/renderer.h"

class App{
public:
    App* appPtr = nullptr;
    Atom currentAtom;
    Camera cam;
    Renderer renderer;

    App(Atom atom, Camera cam) : currentAtom(atom), cam(0.0f, 0.0f, 3.0f) {}

    void keyCallback();
    void mouseCallback();
    void init();
    void update();
    void render();
};