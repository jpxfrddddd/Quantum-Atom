#include "app.h"
#include "../physics/atom.h"
#include "../physics/orb.h"

void keyCallback(){}

void App::init(){
    currentAtom = Chlorine();
    currentOrbital = Orbital_1s();
}

void App::update(){}

void App::render(){
    renderer.render(currentAtom, cam);
    renderer.renderOrbital(currentOrbital, cam);
}

App* appPtr;