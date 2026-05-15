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
            {2,0,0,1,psi_2s}
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

Atom Chlorine(){
    return Atom{
        "chlorine",
        17,
        std::vector<Orbital>{
            {1,0,0,2,psi_1s},

            {2,0,0,2,psi_2s},
            {2,1,1,2,psi_2p_x},
            {2,1,0,2,psi_2p_z},
            {2,1,-1,2,psi_2p_y},

            {3,0,0,2,psi_3s},
            {3,1,1,2,psi_3p_x},
            {3,1,0,2,psi_3p_z},
            {3,1,-1,1,psi_3p_y}
        }
    };
}

Atom Zinc(){
    return Atom{
        "zinc",
        30,
        std::vector<Orbital>{
            {1,0,0,2,psi_1s},

            {2,0,0,2,psi_2s},
            {2,1,1,2,psi_2p_x},
            {2,1,0,2,psi_2p_z},
            {2,1,-1,2,psi_2p_y},

            {3,0,0,2,psi_3s},
            {3,1,1,2,psi_3p_x},
            {3,1,0,2,psi_3p_z},
            {3,1,-1,2,psi_3p_y},

            {3,2,2,2,psi_3d_x2y2},
            {3,2,1,2,psi_3d_xz},
            {3,2,0,2,psi_3d_z2},
            {3,2,-1,2,psi_3d_yz},
            {3,2,-2,2,psi_3d_xy},

            {4,0,0,2,psi_4s}
        }
    };
}
