#include "app.h"
#include "../physics/atom.h"

//App::App(Atom atom) : currentAtom(atom) {}

void App::init(){
    currentAtom = Magnesium();
}

void App::update(){}

void App::render(){
    renderer.render(currentAtom);
}