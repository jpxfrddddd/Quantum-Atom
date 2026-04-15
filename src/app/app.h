#pragma once
#include "../physics/atom.h"
#include "../renderer/renderer.h"

class App{
public:
    Atom currentAtom;
    Renderer renderer;

    App(Atom atom) : currentAtom(atom) {}

    void init();
    void update();
    void render();
};