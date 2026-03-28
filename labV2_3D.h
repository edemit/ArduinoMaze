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

  const Line fw1[5] = {  // the vertical link between the wall on the left and the wall on the middle or the way to the front door
    { 4, 4, 4, 59 },
    { 1, 1, 1, 62 },
    { 16, 16, 16, 47 },
    { 13, 13, 13, 50 },
    { 10, 10, 10, 53 }
  };
  const Line fw2[4] = {  // the vertical link between the wall on the left and the door on the left, appears in the door in the animation
    { 16, 16, 16, 47 },
    { 11, 11, 11, 52 },
    { 7, 7, 7, 56 },
    { 3, 3, 3, 60 }
  };
  const Line fw3[5] = {  // wall on the left after the door, this part is mandatory
    { 18, 18, 18, 18 },
    { 14, 14, 16, 16 },
    { 10, 10, 11, 11 },
    { 4, 4, 7, 7 },
    { 0, 0, 3, 3 }
  };
  const Line fw4[7][7] = {
    // the front door
    { { 4, 4, 15, 15 },
      { 4, 59, 15, 48 },
      { 15, 15, 15, 48 },
      { 15, 15, 31, 15 },
      { 21, 22, 31, 22 },
      { 15, 48, 21, 48 },
      { 21, 22, 21, 48 } },

    { { 1, 1, 12, 12 },
      { 1, 62, 12, 51 },
      { 12, 12, 12, 51 },
      { 12, 12, 31, 12 },
      { 20, 20, 31, 20 },
      { 20, 20, 20, 51 },
      { 12, 51, 20, 51 } },

    { { 0, 0, 9, 9 },
      { 9, 9, 31, 9 },
      { 9, 9, 9, 54 },
      { 0, 63, 9, 54 },
      { 9, 54, 18, 54 },
      { 18, 19, 18, 54 },
      { 18, 19, 31, 19 } },

    { { 0, 0, 6, 6 },
      { 6, 6, 31, 6 },
      { 6, 6, 6, 57 },
      { 0, 63, 6, 57 },
      { 6, 57, 17, 57 },
      { 17, 17, 17, 57 },
      { 17, 17, 31, 17 } },

    { { 0, 0, 3, 3 },
      { 3, 3, 31, 3 },
      { 3, 3, 3, 60 },
      { 0, 63, 3, 60 },
      { 3, 60, 13, 60 },
      { 13, 13, 13, 60 },
      { 13, 13, 31, 13 } },

    { { 9, 10, 31, 10 },
      { 9, 10, 9, 63 },
      { 0, 0, 0, 0 },
      {},
      {},
      {},
      {} },

    { { 3, 4, 31, 4 },
      { 3, 4, 3, 63 },
      { 0, 0, 0, 0 },
      {},
      {},
      {},
      {} },
  };

  const Line fw5[5] = {  // wall that can be seen from the front and that goes to  the left door
    { 0, 4, 4, 4 },
    { 0, 1, 1, 1 },
    { 12, 16, 16, 16 },
    { 8, 13, 13, 13 },
    { 4, 10, 10, 10 }
  };
  const Line fw7[3] = {  //the next front door
    { 16, 16, 18, 18 },
    { 13, 13, 18, 18 },
    { 10, 10, 18, 18 }
  };
  const Line fw8[6] = {  //the left wall if no door
    { 0, 0, 4, 4 },
    { 0, 0, 1, 1 },
    { 16, 16, 18, 18 },
    { 11, 11, 16, 16 },
    { 7, 7, 13, 13 },
    { 3, 3, 10, 10 }
  };
  const Line fw9[3] = {  // the next front door if no wall
    { 16, 16, 47, 16 },
    { 13, 13, 50, 13 },
    { 10, 10, 53, 10 }
  };


  //-----rotationAnimation-----//
  const Line leftDoor[2] = {
    { 0, 7, 7, 7 },
    { 0, 1, 6, 2 }
  };
  const Line rightDoorFrontDoor[7] = {
    { 56, 8, 56, 55 },
    { 54, 8, 54, 55 },
    { 53, 8, 53, 56 },
    { 50, 7, 50, 56 },
    { 44, 7, 44, 56 },
    { 38, 7, 38, 56 },
    { 32, 7, 32, 56 }
  };
  const Line leftWall[2] = {
    { 0, 0, 7, 7 },
    { 5, 0, 6, 2 },
  };
  /**
     *
     */
  const Line leftWallLink[4] = {
    { 7, 8, 7, 55 },
    { 6, 1, 6, 62 },
    { 4, 0, 4, 63 },
    { 2, 0, 2, 63 }
  };
public:
  const Line frontWallNoDoor[12] = {  // a mirroir a l'horizontal
    { 8, 7, 55, 7 },
    { 6, 2, 54, 7 },
    { 13, 0, 53, 7 },
    {
      36,
      0,
      49,
      6,
    },
    { 34, 0, 43, 6 },
    { 28, 0, 37, 6 },
    { 23, 0, 31, 6 },
    { 18, 0, 24, 6 },
    { 12, 0, 20, 6 },
    { 6, 0, 14, 7 },
    { 2, 0, 10, 8 },
    { 2, 0, 9, 7 }
  };
  const Line middleDoor[12][12] = {
    { { 8, 8, 17, 17 },
      { 18, 18, 45, 18 },
      { 45, 18, 45, 45 },
      { 46, 17, 55, 8 },
      { 46, 46, 55, 55 },
      { 40, 45, 45, 45 },
      { 40, 24, 40, 45 },
      { 23, 24, 40, 24 },
      { 23, 24, 23, 45 },
      { 18, 45, 23, 45 },
      { 18, 18, 18, 45 },
      { 8, 55, 17, 46 } },

    { { 6, 2, 18, 17 },
      { 6, 61, 18, 47 },
      { 45, 45, 54, 56 },
      { 45, 18, 54, 7 },
      { 18, 16, 45, 18 },
      { 45, 18, 45, 45 },
      { 40, 45, 45, 45 },
      { 40, 24, 40, 45 },
      { 23, 23, 40, 24 },
      { 23, 23, 23, 47 },
      { 18, 47, 23, 47 },
      { 18, 16, 18, 47 } },

    {
      { 7, 0, 17, 16 },
      { 7, 63, 17, 46 },
      { 16, 16, 40, 18 },
      { 40, 18, 40, 44 },
      { 35, 44, 40, 44 },
      { 40, 44, 53, 56 },
      { 35, 25, 35, 44 },
      { 22, 24, 35, 25 },
      { 22, 24, 22, 46 },
      { 17, 46, 22, 46 },
      { 17, 16, 17, 46 },
      { 40, 18, 53, 7 },
    },

    { { 4, 0, 14, 15 },
      { 14, 15, 37, 18 },
      { 37, 18, 49, 7 },
      { 37, 43, 49, 56 },
      { 37, 18, 37, 43 },
      { 33, 43, 37, 43 },
      { 19, 22, 33, 23 },
      { 14, 45, 19, 45 },
      { 4, 63, 14, 45 },
      { 14, 15, 14, 45 },
      { 19, 22, 19, 45 },
      { 33, 23, 33, 43 } },

    { { 0, 0, 6, 12 },
      { 6, 12, 29, 19 },
      { 29, 19, 43, 7 },
      { 29, 44, 43, 56 },
      { 0, 63, 6, 50 },
      { 6, 50, 12, 49 },
      { 25, 45, 29, 44 },
      { 12, 21, 25, 23 },
      { 6, 12, 6, 50 },
      { 12, 21, 12, 49 },
      { 25, 23, 25, 45 },
      { 29, 19, 29, 44 } },

    { { 0, 0, 2, 9 },
      { 2, 9, 24, 18 },
      { 24, 18, 37, 7 },
      { 24, 46, 37, 56 },
      { 8, 19, 20, 22 },
      { 0, 63, 2, 52 },
      { 2, 52, 8, 51 },
      { 20, 47, 24, 46 },
      { 2, 9, 2, 52 },
      { 24, 18, 24, 46 },
      { 8, 19, 8, 51 },
      { 20, 22, 20, 47 } },

    { { 0, 13, 17, 18 },
      { 17, 18, 31, 7 },
      { 17, 18, 17, 45 },
      { 17, 45, 31, 56 },
      { 0, 20, 12, 22 },
      { 12, 22, 12, 46 },
      { 12, 46, 17, 45 },
      { 0, 0, 0, 0 },
      {},
      {},
      {},
      {} },

    { { 0, 14, 10, 18 },
      { 10, 18, 10, 46 },
      { 5, 47, 10, 46 },
      { 0, 21, 5, 22 },
      { 5, 22, 5, 47 },
      { 10, 18, 25, 7 },
      { 10, 46, 25, 56 },
      { 0, 0, 0, 0 },
      {},
      {},
      {},
      {} },

    { { 0, 15, 4, 16 },
      { 4, 16, 19, 7 },
      { 4, 16, 4, 47 },
      { 0, 48, 4, 47 },
      { 4, 47, 19, 56 },
      { 0, 0, 0, 0 },
      {},
      {},
      {},
      {},
      {},
      {} },

    { { 0, 13, 13, 7 },
      { 0, 50, 13, 56 },
      { 0, 0, 0, 0 },
      {},
      {},
      {},
      {},
      {},
      {},
      {},
      {},
      {} },

    { { 0, 9, 10, 8 },
      { 0, 55, 10, 56 },
      { 0, 0, 0, 0 },
      {},
      {},
      {},
      {},
      {},
      {},
      {},
      {},
      {} },

    { { 0, 8, 9, 7 },
      { 0, 55, 9, 56 },
      { 0, 0, 0, 0 },
      {},
      {},
      {},
      {},
      {},
      {},
      {},
      {},
      {} },
  };

public:
  int angleVu = 1;
  bool options[4] = {
    false, true, true, true
  };


  void showPoint(Line l, uint64_t t[64], bool mirrorHor, bool mirrorVert) {
    //std::cout<<(l.y1-l.y2)/(l.x1-l.x2)<<" ici "<<std::endl;
    float a = (l.x1 - l.x2);
    //std::cout<<a<<" original "<<int(l.x1)<< " x1 "<<int(l.y1)<< " y1 "<<int(l.x2) << " x2 "<<int(l.y2)<< " y2 "<<std::endl;
    if (a != 0) {
      if (!mirrorVert) {
        ;
        a = (l.y1 - l.y2) / a;  // a=(y1-y2)/(x1-x2)
        //std::cout<<a<<"result "<<std::endl;
        int b = l.y1 - (l.x1 * a);
        for (int i = l.x1; i < l.x2 + 1; ++i) {
          //std::cout<<a<<" a "<<b<<" b "<<i << " x "<<(a*i)+b<< " y "<< mirrorHor << " horzontal "<< mirrorVert << " vertical" <<std::endl;
          t[int(a * i) + b] |= (1ULL << i);
          if (mirrorHor) {
            //std::cout<<a<<" a "<<b<<" b "<<i << " x "<<((-a)*i)+(63-b)<< " y "<< mirrorHor << " horzontal "<< mirrorVert << " vertical" <<std::endl;
            t[int((-a) * i) + (63 - b)] |= (1ULL << i);
          }
        }
      } else {
        l = { uint8_t(63 - l.x1), l.y1, uint8_t(63 - l.x2), l.y2 };
        a = (l.x1 - l.x2);
        a = (l.y1 - l.y2) / a;  // a=(y1-y2)/(x1-x2)
        //std::cout<<a<<" r A "<<std::endl;
        int b = l.y1 - (l.x1 * a);
        //std::cout<<a<<" r B "<<std::endl;
        for (int i = l.x2; i < l.x1 + 1; ++i) {
          //std::cout<<a<<" a "<<b<<" b "<<i << " x "<<(a*i)+b<< " y "<< mirrorHor << " horzontal "<< mirrorVert << " vertical" <<std::endl;
          t[int(a * i) + b] |= (1ULL << i);
          if (mirrorHor) {
            //std::cout<<a<<" a "<<b<<" b "<<i << " x "<<((-a)*i)+(63-b)<< " y "<< mirrorHor << " horzontal "<< mirrorVert << " vertical" <<std::endl;
            t[int((-a) * i) + (63 - b)] |= (1ULL << i);
          }
        }
      }
    } else {
      //std::cout<<"  vertical barre ";
      if (mirrorVert) {
        for (int i = l.y1; i < l.y2 + 1; ++i) {
          t[i] |= (1ULL << 63 - l.x1);
          if (mirrorHor) {
            t[63 - i] |= (1ULL << 63 - l.x1);
          }
        }
      } else {
        for (int i = l.y1; i < l.y2 + 1; ++i) {
          t[i] |= (1ULL << l.x1);
          if (mirrorHor) {
            t[63 - i] |= (1ULL << l.x1);
          }
        }
      }
    }
  }

  uint8_t reverseByte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
  }

  void affichageAnim(int frame, byte packets[512]) {  //TODO : créer une liste de référencement
    uint64_t tabAffichage[64] = {};

    if (options[0]) {   //affichage de la partie gauche si il y a une porte
      if (frame < 2) {  //affiche la petite partie du mur vers la porte a gauche avant la rotation
        showPoint(leftDoor[frame], tabAffichage, true, false);
      }
    } else {            //sans porte gauche
      if (frame < 2) {  //affiche la petite partie du mur a gauche avant la rotation
        showPoint(leftWall[frame], tabAffichage, true, false);
      }
      // affiche l'arrete entre le centre et la gauche avant qu'il disparaisse grace a la rotation
      if (frame < 4 and !options[1]) {
        showPoint(leftWallLink[frame], tabAffichage, false, false);
      }
    }



    if (options[1]) {                  //affichage d'en face su il y a une porte
      if (options[0] and frame < 4) {  //si j'ai porte a gauche et au centre
        // arrete entre gauche et centre
        showPoint(leftWallLink[frame], tabAffichage, false, false);
      }
      if (options[2]) {   //si j'ai porte centre et a droite
        if (frame < 7) {  //7 premières frames de l'arrete entre porte centre et porte droite
          showPoint(rightDoorFrontDoor[frame], tabAffichage, false,
                    false);
        } else {  //retour en arriere dans les frames avec une rotation verticale
          showPoint(rightDoorFrontDoor[6 - (frame % 6)], tabAffichage, false,
                    true);
        }
      }
      for (int i = 0; i < 12; ++i) {  // porte du centre rotation vers la gauche
        if (!(middleDoor[frame][i].x1 == 0 and middleDoor[frame][i].x2 == 0 and middleDoor[frame][i].y1 == 0 and middleDoor[frame][i].y2 == 0)) {
          // les animations peuvent etre entre 2 et 12 droites, j'arrete les animations des que je croise un point allant de (0,0) a (0,0)
          showPoint(middleDoor[frame][i], tabAffichage, false, false);
        }
      }
    } else {              //sans porte milieu
      if (!options[2]) {  //sans porte a droite
        if (frame < 7) {  //j'affiche les premieres annimations de l'arrete du mur entre centre et droite
          showPoint(rightDoorFrontDoor[frame], tabAffichage, false,
                    false);
        } else {  //je boucle et j'affiche les memes animations dans l'autre sens avec un effect mirroir vertical
          //pour faire passer cette arrete jusqu'a gauche de l'ecran
          showPoint(rightDoorFrontDoor[6 - (frame % 6)], tabAffichage, false,
                    true);
        }
      }
      //affichage du mur au centre
      showPoint(frontWallNoDoor[frame], tabAffichage, true, false);
    }


    if (options[2]) {  //affichage de la partie droite si il y a une porte
      if (frame == 0) {
        // si j'ai une porte a droite j'affiche la partie du mur
        //seulement pour 0 car le middle door inversé prend le dessus apres pour cet affichage
        showPoint(leftDoor[0], tabAffichage, true, true);
      }
      if (options[3] and frame > 8) {  //affichage de l'arrete entre la droite et la nouvelle droite apres rotation
        // en fin de rotation si il y a une porte dans la nouvelle droite on affiche cette arrete
        showPoint(leftWallLink[12 - frame], tabAffichage, false, true);
      }
      if (frame > 0) {  // affichage de la middle door inversé por montrer la nouvelle porte
        for (int i = 0; i < 12; ++i) {
          if (!(middleDoor[12 - frame][i].x1 == 0 and middleDoor[12 - frame][i].x2 == 0 and middleDoor[12 - frame][i].y1 == 0 and middleDoor[12 - frame][i].y2 == 0)) {
            showPoint(middleDoor[12 - frame][i], tabAffichage, false, true);
          }  //TODO : mettre  un break ou un while
        }
      }
    } else {             //sans porte droite
      if (frame == 0) {  //affiche le mur de la droite si je n'ai pas de porte
        showPoint(leftWall[0], tabAffichage, true, true);
      }  // affiche l'arrete de la nouvelle droite / droite si les deux n'ont pas de porte
      if (!options[3] and frame > 8) {
        showPoint(leftWallLink[12 - frame], tabAffichage, false, true);
      }                                   //affiche le debut du mur de la nouvelle droite
      if (!options[3] and frame == 11) {  //
        showPoint(leftWall[1], tabAffichage, true, true);
      }
      if (frame > 0) {
        showPoint(frontWallNoDoor[12 - frame], tabAffichage, true, true);
      }
    }
    if (options[3]) {     //affichage de la partie droite apres avec avoir tourné si il y a une porte
      if (frame == 11) {  //
        showPoint(leftDoor[1], tabAffichage, true, true);
      }
    }

    //transforme la matrice graphiquement
    for (int i = 0; i < 512; i++) {
      packets[i] = reverseByte((tabAffichage[i / 8] >> ((i % 8) * 8)) & 0xFF);
    }
    //regler pour le prochain stable
  }

  void afficheForward(int frame, bool oldOptions[4], byte packets[512]) {
    uint64_t tabAffichage[64] = {};
    //-----before the going throught the door-----//
    if (oldOptions[0]) {  //if there is a door before going through the new one
      if (frame < 2) {
        showPoint(fw1[frame], tabAffichage, true, false); //the link 
        showPoint(fw5[frame], tabAffichage, true, false);  //wall that lead to the door on the left | before door
      }
    } else {
      if (frame < 2) {  //the left wall before it goes throught the door
        showPoint(fw8[frame], tabAffichage, true, false);
      }
    }

    //if oldOptions[1]{} of there is door if front (useless, there is one if not you can't go forward)
    //else{}if there is no door in front, this can be funny if we do and animation where the person smash is head in the wall
    if (oldOptions[2]) {  //if there is a door a the right before going through the new one
      if (frame < 2) {
        showPoint(fw1[frame], tabAffichage, true, true);
        showPoint(fw5[frame], tabAffichage, true, true);  //wall that lead to the door on the left | before door
      }
    } else {
      if (frame < 2) {  //the right wall before it goes throught the door
        showPoint(fw8[frame], tabAffichage, true, true);
      }
    }





    //-----after going throught the door-----//
    if (options[0]) {  //if there is a door after going through the door
      if (frame > 3) {
        showPoint(fw2[frame-4], tabAffichage, false, false);
        if (frame > 4) {
          if(options[1]) {
            showPoint(fw1[frame-3], tabAffichage, true, false);
          }
          showPoint(fw5[frame-3], tabAffichage, true, false);  //wall that lead to the door on the left | after door
        }
      }
    } else {
      if (frame > 3) {  //the left wall after it goes throught the door
        showPoint(fw8[frame-2], tabAffichage, true, false);
      }
    }

    if (options[1]) {
      if (frame >4){
        showPoint(fw7[frame-5], tabAffichage, true, false);
        showPoint(fw7[frame-5], tabAffichage, true, true);
      }
    }else{
      if (frame >4){
        if (!options[2]) {
          showPoint(fw1[frame-3], tabAffichage, true, true);// if the new front wall and the right side have no door we show the vertical link
        }
        if (!options[0]) {
          showPoint(fw1[frame-3], tabAffichage, true, false);// if the new front wall and the left side have no door we show the vertical link
        }
        showPoint(fw9[frame-5], tabAffichage, true, false);
      }
    } 
    
    if (options[2]) {  //if there is a door a the right after going through the door
      if (frame > 3) {
        showPoint(fw2[frame-4], tabAffichage, false, true);
        if (frame > 4) {
          if(options[1]) {
            showPoint(fw1[frame-3], tabAffichage, true, true);
          }
          showPoint(fw5[frame-3], tabAffichage, true, true);  //wall that lead to the door on the left | after door
        }
      }
    }
    else {
      if (frame > 3)
        showPoint(fw8[frame-2], tabAffichage, true, true);
    }
  


    //---the animation that will always append-----//
    if (frame < 7) {
      for (int i = 0; i < 7; i++) {
        if (!(fw4[frame][i].x1 == 0 and fw4[frame][i].x2 == 0 and fw4[frame][i].y1 == 0 and fw4[frame][i].y2 == 0)) {
          showPoint(fw4[frame][i], tabAffichage, false, false);  //door in front that you go through
          showPoint(fw4[frame][i], tabAffichage, false, true);   //door in front that you go through second part mirrored
        }
      }
    }
    if (frame > 2) {  //the little wall part juste at the angles of the door, mandatory part
      showPoint(fw3[frame-3], tabAffichage, true, false);
      showPoint(fw3[frame-3], tabAffichage, true, true);
    }

    for (int i = 0; i < 512; i++) {
      packets[i] = reverseByte((tabAffichage[i / 8] >> ((i % 8) * 8)) & 0xFF);
    }
  }
};





#endif UNTITLED_LABV2_3D_H