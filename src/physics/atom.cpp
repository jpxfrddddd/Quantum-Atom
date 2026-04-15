#include "atom.h"

Atom Hydrogen(){
    return Atom{
        "hydrogen",
        1,
        std::vector<Orbital>{
            {1,0,0,1,psi_1s}
        }
    };
}

Atom Lithium(){
    return Atom{
        "lithium",
        3,
        std::vector<Orbital>{
            {1,0,0,2,psi_1s},
            {2,0,0,1,psi_1s}
        }
    };
}

Atom Magnesium(){
    return Atom{
        "magnesium",
        12,
        std::vector<Orbital>{
            {1,0,0,2,psi_1s},
            {2,0,0,2,psi_2s},

            {2,1,-1,2,psi_2p_x},
            {2,1,-1,2,psi_2p_y},
            {2,1,1,2,psi_2p_z},

            {3,0,0,2,psi_3s}
        }
    };
}


