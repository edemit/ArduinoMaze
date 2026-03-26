//
// Created by pat on 12/03/2026.
//

#ifndef UNTITLED_LABV2_3D_H
#define UNTITLED_LABV2_3D_H

class labV2_3D {

    struct Line {
        uint8_t x1;
        uint8_t y1;
        uint8_t x2;
        uint8_t y2;
    };
    const Line leftDoor[2]={
        {0,7, 7,7},
        {0,1,6,2}
    };
    const Line rightDoorFrontDoor[7]={
        {56,8, 56,55},
        {54,8, 54,55},
        {53,8, 53,56},
        {50,7,50,56},
        {44,7,44,56},
        {38,7,38,56},
        {32,7,32,56}
    };
    const Line leftWall[2]={
        {0,0, 7,7},
        {5,0,6,2},
        };
    /**
     *
     */
    const Line leftWallLink[4]={
    {7,8, 7,55},
    {6,1,6,62},
    {4,0,4,63},
    {2,0,2,63}};
public:
    const Line frontWallNoDoor[12]= {// a mirroir a l'horizontal
        {8,7, 55,7},
        {6,2,54,7},
        {13,0,53,7},
        {36,0,49,6,},
        {34,0, 43,6},
        {28,0,37,6},
        {23,0,31,6},
        {18,0,24,6},
        {12,0,20,6},
        {6,0,14,7},
        {2,0,10,8},
        {2,0,9,7}
    };
    const Line middleDoor[12][12]= {
        {{8,8, 17,17},
       {18,18, 45,18},
       {45,18, 45,45},
       {46,17, 55,8},
       {46,46, 55,55},
       {40,45,45,45},
       {40,24, 40,45},
       {23,24, 40,24},
       {23,24, 23,45},
       {18,45, 23,45},
       {18,18, 18,45},
       { 8,55,17,46}},

        {{6,2,18,17},
        {6,61,18,47},
        {45,45,54,56},
        {45,18,54,7},
        {18,16,45,18},
        {45,18,45,45},
        {40,45,45,45},
        {40,24,40,45},
        {23,23,40,24},
        {23,23,23,47},
        {18,47,23,47},
        {18,16,18,47}},

        {{7,0,17,16},
        {7,63,17,46},
        {16,16,40,18},
        {40,18,40,44},
        {35,44,40,44},
        {40,44,53,56},
        {35,25,35,44},
        {22,24,35,25},
        {22,24,22,46},
        {17,46,22,46},
        {17,16,17,46},
        {40,18,53,7},},

        {{4,0,14,15},
        {14,15,37,18},
        {37,18,49,7},
        {37,43,49,56},
        {37,18,37,43},
        {33,43,37,43},
        {19,22,33,23},
        {14,45,19,45},
        {4,63,14,45},
        {14,15,14,45},
        {19,22,19,45},
        {33,23,33,43}},

        {{0,0,6,12},
        {6,12,29,19},
        {29,19,43,7},
        {29,44,43,56},
        {0,63,6,50},
        {6,50,12,49},
        {25,45,29,44},
        {12,21,25,23},
        {6,12,6,50},
        {12,21,12,49},
        {25,23,25,45},
        {29,19,29,44}},

        {{0,0,2,9},
        {2,9,24,18},
        {24,18,37,7},
        {24,46,37,56},
        {8,19,20,22},
        {0,63,2,52},
        {2,52,8,51},
        {20,47,24,46},
        {2,9,2,52},
        {24,18,24,46},
        {8,19,8,51},
        {20,22,20,47}},

        {{0,13,17,18},
        {17,18,31,7},
        {17,18,17,45},
        {17,45,31,56},
        {0,20,12,22},
        {12,22,12,46},
        {12,46,17,45},
        {0,0,0,0},
        {},
        {},
        {},
        {}},

        {{0,14,10,18},
        {10,18,10,46},
        {5,47,10,46},
        {0,21,5,22},
        {5,22,5,47},
        {10,18,25,7},
        {10,46,25,56},
        {0,0,0,0},
        {},
        {},
        {},
        {}},

        {{0,15,4,16},
        {4,16,19,7},
        {4,16,4,47},
        {0,48,4,47},
        {4,47,19,56},
        {0,0,0,0},
        {},
        {},
        {},
        {},
        {},
        {}},

        {{0,13,13,7},
        {0,50,13,56},
        {0,0,0,0},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {}},

        {{0,9,10,8},
        {0,55,10,56},
        {0,0,0,0},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {}},

        {{0,8,9,7},
        {0,55,9,56},
        {0,0,0,0},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {},
        {}},};

    Line doorLeftForward[5] {
        {}
    };
    Line middleDoorForward[7][7]={};
    //[porte a gauche, porte en face, porte a droite, porte derrière ]
public :
    bool options[4]={
    false,true,true,true
};


    void showPoint(Line l, uint64_t t[64],bool mirrorHor,bool mirrorVert) {
        //std::cout<<(l.y1-l.y2)/(l.x1-l.x2)<<" ici "<<std::endl;
        float a = (l.x1-l.x2);
        //std::cout<<a<<" original "<<int(l.x1)<< " x1 "<<int(l.y1)<< " y1 "<<int(l.x2) << " x2 "<<int(l.y2)<< " y2 "<<std::endl;
        if (a!=0) {
            if (!mirrorVert) {;
                a=(l.y1-l.y2)/a;// a=(y1-y2)/(x1-x2)
                //std::cout<<a<<"result "<<std::endl;
                int b=l.y1-(l.x1*a);
                for (int i=l.x1; i<l.x2+1; ++i) {
                    //std::cout<<a<<" a "<<b<<" b "<<i << " x "<<(a*i)+b<< " y "<< mirrorHor << " horzontal "<< mirrorVert << " vertical" <<std::endl;
                    t[int(a*i)+b]|=(1ULL << i);
                    if (mirrorHor) {
                        //std::cout<<a<<" a "<<b<<" b "<<i << " x "<<((-a)*i)+(63-b)<< " y "<< mirrorHor << " horzontal "<< mirrorVert << " vertical" <<std::endl;
                        t[int((-a)*i)+(63-b)]|=(1ULL << i);
                    }
                }
            }else{
                l={ uint8_t(63-l.x1),l.y1,uint8_t(63-l.x2),l.y2};
                a = (l.x1-l.x2);
                a=(l.y1-l.y2)/a;// a=(y1-y2)/(x1-x2)
                //std::cout<<a<<" r A "<<std::endl;
                int b=l.y1-(l.x1*a);
                //std::cout<<a<<" r B "<<std::endl;
                for (int i=l.x2; i<l.x1+1; ++i) {
                    //std::cout<<a<<" a "<<b<<" b "<<i << " x "<<(a*i)+b<< " y "<< mirrorHor << " horzontal "<< mirrorVert << " vertical" <<std::endl;
                    t[int(a*i)+b]|=(1ULL << i);
                    if (mirrorHor) {
                        //std::cout<<a<<" a "<<b<<" b "<<i << " x "<<((-a)*i)+(63-b)<< " y "<< mirrorHor << " horzontal "<< mirrorVert << " vertical" <<std::endl;
                        t[int((-a)*i)+(63-b)]|=(1ULL << i);
                    }
                }
            }
        }else {
            //std::cout<<"  vertical barre ";
            if (mirrorVert) {
                for (int i=l.y1; i<l.y2+1; ++i) {
                    t[i]|=(1ULL << 63-l.x1);
                    if (mirrorHor) {
                        t[63-i]|=(1ULL << 63-l.x1);
                    }
                }
            }else{
                for (int i=l.y1; i<l.y2+1; ++i) {
                    t[i]|=(1ULL << l.x1);
                    if (mirrorHor) {
                        t[63-i]|=(1ULL << l.x1);
                    }
                }

            }
        }
    }

public:
    void affichageAnim(int frame) {//TODO : créer une liste de référencement
        uint64_t tabAffichage[64]={};

        if (options[0]) {//affichage de la partie gauche si il y a une porte
            if (frame<2) {//affiche la petite partie du mur vers la porte a gauche avant la rotation
                showPoint(leftDoor[frame],tabAffichage,true,false);
            }
        }else {//sans porte gauche
            if (frame<2) {//affiche la petite partie du mur a gauche avant la rotation
                showPoint(leftWall[frame],tabAffichage,true,false);
            }
            // affiche l'arrete entre le centre et la gauche avant qu'il disparaisse grace a la rotation
            if(frame<4  and  !options[1]) {
                showPoint(leftWallLink[frame],tabAffichage,false,false);
            }
        }



        if (options[1]) {//affichage d'en face su il y a une porte
            if (options[0] and frame<4) {//si j'ai porte a gauche et au centre
                // arrete entre gauche et centre
                showPoint(leftWallLink[frame],tabAffichage,false,false);
            }
            if (options[2] ) {//si j'ai porte centre et a droite
                if (frame<7) {//7 premières frames de l'arrete entre porte centre et porte droite
                    showPoint(rightDoorFrontDoor[frame],tabAffichage,false,
                    false);
                }else {//retour en arriere dans les frames avec une rotation verticale
                    showPoint(rightDoorFrontDoor[6-(frame%6)],tabAffichage,false,
                    true);
                }
            }
            for (int i = 0; i < 12; ++i) {// porte du centre rotation vers la gauche
                if (!(middleDoor[frame][i].x1==0 and middleDoor[frame][i].x2==0 and middleDoor[frame][i].y1==0 and middleDoor[frame][i].y2==0)) {
                    // les animations peuvent etre entre 2 et 12 droites, j'arrete les animations des que je croise un point allant de (0,0) a (0,0)
                    showPoint(middleDoor[frame][i],tabAffichage,false,false);
                }
            }
        }else {//sans porte milieu
            if (!options[2]) {//sans porte a droite
                if(frame<7) {//j'affiche les premieres annimations de l'arrete du mur entre centre et droite
                    showPoint(rightDoorFrontDoor[frame],tabAffichage,false,
                    false);
                }else {//je boucle et j'affiche les memes animations dans l'autre sens avec un effect mirroir vertical
                    //pour faire passer cette arrete jusqu'a gauche de l'ecran
                    showPoint(rightDoorFrontDoor[6-(frame%6)],tabAffichage,false,
                    true);
                }
            }
            //affichage du mur au centre
            showPoint(frontWallNoDoor[frame],tabAffichage,true,false);
        }


        if (options[2]) {//affichage de la partie droite si il y a une porte
            if (frame ==0) {
                // si j'ai une porte a droite j'affiche la partie du mur
                //seulement pour 0 car le middle door inversé prend le dessus apres pour cet affichage
                showPoint(leftDoor[0],tabAffichage,true,true);
            }
            if (options[3] and frame>8) {//affichage de l'arrete entre la droite et la nouvelle droite apres rotation
                // en fin de rotation si il y a une porte dans la nouvelle droite on affiche cette arrete
                showPoint(leftWallLink[12-frame],tabAffichage,false,true);
            }if (frame>0) {// affichage de la middle door inversé por montrer la nouvelle porte
                for (int i = 0; i < 12; ++i) {
                    if (!(middleDoor[12-frame][i].x1==0 and middleDoor[12-frame][i].x2==0 and middleDoor[12-frame][i].y1==0 and middleDoor[12-frame][i].y2==0)) {
                        showPoint(middleDoor[12-frame][i],tabAffichage,false,true);
                    }//TODO : mettre  un break ou un while
                }
            }
        }else {//sans porte droite
            if (frame==0) {//affiche le mur de la droite si je n'ai pas de porte
                showPoint(leftWall[0],tabAffichage,true,true);
            }// affiche l'arrete de la nouvelle droite / droite si les deux n'ont pas de porte
            if (!options[3] and frame>8 ) {
                showPoint(leftWallLink[12-frame],tabAffichage,false,true);
            }//affiche le debut du mur de la nouvelle droite
            if (!options[3] and frame==11) {//
                showPoint(leftWall[1],tabAffichage,true,true);
            }
            if (frame>0) {
                showPoint(frontWallNoDoor[12-frame],tabAffichage,true,true);
            }
        }
        if (options[3]) {//affichage de la partie droite apres avec avoir tourné si il y a une porte
            if (frame==11) {//
                showPoint(leftDoor[1],tabAffichage,true,true);
            }
        }

        //regler pour le prochain stable

    }
};





#endif UNTITLED_LABV2_3D_H