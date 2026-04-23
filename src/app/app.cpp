#include "app.h"
#include "../physics/atom.h"

//extern Camera cam;

void keyCallback(){}

void App::init(){
    currentAtom = Magnesium();
}

void App::update(){}

void App::render(){
    renderer.render(currentAtom, cam);
}

App* appPtr;