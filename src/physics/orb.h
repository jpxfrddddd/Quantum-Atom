#pragma once

#include "orbital.h"
#include <iostream>
#include <vector>

struct orbitalGroup{
    std::string name;
    int Z;
    std::vector<Orbital> orbitalVec;

    orbitalGroup(std::string n, int z, std::vector<Orbital> o) : name(n), Z(z), orbitalVec(o) {}
};

orbitalGroup Orbital_1s();

orbitalGroup Orbital_2s();
orbitalGroup Orbital_2p();

orbitalGroup Orbital_2p_x();
orbitalGroup Orbital_2p_z();
orbitalGroup Orbital_2p_y();

orbitalGroup Orbital_3s();
orbitalGroup Orbital_3p();

orbitalGroup Orbital_3p_x();
orbitalGroup Orbital_3p_z();
orbitalGroup Orbital_3p_y();