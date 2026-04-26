#include "orb.h"
#include "atom.h"

//Assuming they are full

orbitalGroup Orbital_1s(){
    return orbitalGroup{
        "orbital_1s",
        2,
        std::vector<Orbital>{
            {1,0,0,2, psi_1s}
        }
    };
}

orbitalGroup Orbital_2s(){
    return orbitalGroup{
        "orbital_2s",
        2,
        std::vector<Orbital>{
            {2,0,0,2,psi_2s}
        }
    };
}

orbitalGroup Orbital_2p(){
    return orbitalGroup{
        "orbital_2p",
        6,
        std::vector<Orbital>{
            {2,1,1,2,psi_2p_x},
            {2,1,0,2,psi_2p_z},
            {2,1,-1,2,psi_2p_y}
        }
    };
}

orbitalGroup Orbital_2p_x(){
    return orbitalGroup{
        "orbital_2p_x",
        2,
        std::vector<Orbital>{
            {2,1,1,2,psi_2p_x}
        }
    };
}

orbitalGroup Orbital_2p_z(){
    return orbitalGroup{
        "orbital_2p_z",
        2,
        std::vector<Orbital>{
            {2,1,0,2,psi_2p_z}
        }
    };
}

orbitalGroup Orbital_2p_y(){
    return orbitalGroup{
        "orbital_2p_y",
        2,
        std::vector<Orbital>{
            {2,1,-1,2,psi_2p_y}
        }
    };
}

orbitalGroup Orbital_3s(){
    return orbitalGroup{
        "orbital_3s",
        2,
        std::vector<Orbital>{
            {3,0,0,2,psi_3s}
        }
    };
}

orbitalGroup Orbital_3p(){
    return orbitalGroup{
        "orbital_3p",
        6,
        std::vector<Orbital>{
            {3,1,1,2,psi_3p_x},
            {3,1,0,2,psi_3p_z},
            {3,1,-1,1,psi_3p_y}
        }
    };
}

orbitalGroup Orbital_3p_x(){
    return orbitalGroup{
        "orbital_3p_x",
        2,
        std::vector<Orbital>{
            {3,1,1,2,psi_3p_x}
        }
    };
}

orbitalGroup Orbital_3p_z(){
    return orbitalGroup{
        "orbital_3p_z",
        2,
        std::vector<Orbital>{
            {3,1,0,2,psi_3p_z}
        }
    };
}

orbitalGroup Orbital_3p_y(){
    return orbitalGroup{
        "orbital_3p_y",
        2,
        std::vector<Orbital>{
            {3,1,-1,2,psi_3p_y}
        }
    };
}