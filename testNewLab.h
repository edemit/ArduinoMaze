#ifndef UNTITLED1_LABY_H
#define UNTITLED1_LABY_H


#include <Arduino.h>
class laby {
    //---constantes---//

    public:
        #define F_DOWN 0b00010000  // bit 0 → 1
        #define F_RIGHT 0b00100000  // bit 1 → 2
        #define F_UP 0b01000000  // bit 2 → 4
        #define F_LEFT 0b10000000 // bit 3 → 8
        #define F_DOWN2 0b00000001  // bit 0 → 1
        #define F_RIGHT2 0b00000010  // bit 1 → 2
        #define F_UP2 0b00000100  // bit 2 → 4
        #define F_LEFT2 0b00001000  // bit 3 → 8
        int *schemaLab;
        int TAILLE ,TAILLE_G,END,ORIGIN=0,posPlayer=0;
        uint8_t *lab;

        laby(int taille,int tailleG) {
            TAILLE=taille;
            TAILLE_G=tailleG;
            END = TAILLE-1;
            lab = new uint8_t[(TAILLE*TAILLE)/2]();
        }


        void newDimLab() {// je check les voisins pour chaques cases pour crééer le labyrinthe definitif
            for (int y = 0; y < TAILLE*TAILLE; y++) {
                if (checkFlag(y-1,1)) {//je check que la case d'avant vienne vers moi
                    addDirection(y,3);
                }if (checkFlag(y-TAILLE,0)) {//je check que la case d'au dessus vienne vers moi
                    addDirection(y,2);
                }if (checkFlag(y+1,3)) {//je check que la case de devant vienne vers moi
                    addDirection(y,1);
                }if (checkFlag(y+TAILLE,2)) {//je check que la case de dessous vienne vers moi
                    addDirection(y,0);
                }
            }
        }
        void rotateMatrix() {
            uint8_t leftPart=lab[posPlayer/2]>>4;//extrait la partie de gauche de l'octet
            uint8_t rightPart= lab[posPlayer/2] & 0xF;//extrait la partie de droite de l'octet
            if (posPlayer%2==0) {
                leftPart = ((leftPart << 1) | (leftPart >> 3)) & 0xF; //effectue un shift de bits circulaire, shift les bits a gauche et y ajouter le bit qui manque au debut
            }else {
                rightPart = ((rightPart << 1) | (rightPart >> 3)) & 0xF;
            }
            lab[posPlayer/2]= leftPart << 4 | rightPart;//recolle les deux parties
        }


            bool checkFlag(int position,int direction) {
                if (position<0 || position>=(TAILLE*TAILLE))    {
                    return false;
                }
                if (position%2==0) {//partie gauche de l'octet
                    switch (direction) {
                        case 0:
                            return lab[(position/2)]&F_DOWN;
                        case 1:
                            return lab[(position/2)]&F_RIGHT;
                        case 2:
                            return lab[(position/2)]&F_UP;
                        case 3:
                            return lab[(position/2)]&F_LEFT;
                    }
                }else{//partie droite
                    switch (direction) {
                        case 0:
                            return lab[(position/2)]&F_DOWN2;
                        case 1:
                            return lab[(position/2)]&F_RIGHT2;
                        case 2:
                            return lab[(position/2)]&F_UP2;
                        case 3:
                            return lab[(position/2)]&F_LEFT2;
                    }
                }
                return false;
            }
        void addDirection(int position,int direction) {
            if (position%2==0) {//partie gauche de l'octet
                switch (direction) {
                    case 0:
                        lab[(position/2)]|=F_DOWN;
                        break;
                    case 1:
                        lab[(position/2)]|=F_RIGHT;
                        break;
                    case 2:
                        lab[(position/2)]|=F_UP;
                        break;
                    case 3:
                        lab[(position/2)]|=F_LEFT;
                        break;
                }
            }else{//partie droite
                switch (direction) {
                    case 0:
                        lab[(position/2)]|=F_DOWN2;
                        break;
                    case 1:
                        lab[(position/2)]|=F_RIGHT2;
                        break;
                    case 2:
                        lab[(position/2)]|=F_UP2;
                        break;
                    case 3:
                        lab[(position/2)]|=F_LEFT2;
                        break;
                }
            }
        }


        void clearDirection(int position) {//effectue un ET logique, par consequent supprime la partie non désiré
            if (position%2==0) {//partie gauche de l'octet
                lab[(position/2)]&=0b00001111;
            }else{//partie droite
                lab[(position/2)]&=0b11110000;
            }
        }
        void creationRandLab() {
            int posOrigin=(TAILLE*TAILLE)-1;
            //std::random_device rd;
            //std::mt19937 gen(rd());

            for (int y = 0; y < (TAILLE*TAILLE*10); y++) {//pour avoir un labyrinthe bien shuffle il faut shift l'origine tailleLaby*tailleLaby*10
                //calcul des choix de shift
                bool voisin[4]= {true,true,true,true};
                int cptVoisinValide=4;
                if (posOrigin%TAILLE == 0) {
                    cptVoisinValide--;
                    voisin[3] = false;
                }if (posOrigin/TAILLE == 0) {
                    cptVoisinValide--;
                    voisin[2] = false;
                }if ((posOrigin+1)%TAILLE == 0){
                    cptVoisinValide--;
                    voisin[1] = false;
                }if (posOrigin/TAILLE == TAILLE-1) {
                    cptVoisinValide--;
                    voisin[0] = false;
                }



                //std::uniform_int_distribution<> dis(1, cptVoisinValide);
                //int randomNum = dis(gen);;
                int randomNum = random(1,cptVoisinValide+1);
                int cptVTrue = 0;
                int cptIndex= 0;
                while (cptVTrue!=randomNum && cptIndex<4){
                    if (voisin[cptIndex]) {
                        cptVTrue++;
                    }if (cptVTrue!=randomNum) {
                        cptIndex++;
                    }
                }
                //std::cout << cptVTrue << " : " << posOrigin << " : " << cptIndex << std::endl;
                //clear de l'origine d'avant
                clearDirection(posOrigin);
                addDirection(posOrigin,cptIndex);
                switch (cptIndex) {
                    case 0:
                        clearDirection(posOrigin+TAILLE);
                        posOrigin=posOrigin+TAILLE;
                        break;
                    case 1:
                        clearDirection(posOrigin+1);
                        posOrigin=posOrigin+1;
                        break;
                    case 2:
                        clearDirection(posOrigin-TAILLE);
                        posOrigin=posOrigin-TAILLE;
                        break;
                    case 3:
                        clearDirection(posOrigin-1);
                        posOrigin=posOrigin-1;
                        break;
                }
            }
        }
        void tabCreation() {
            //ma flat list est composée de 1 octets dont je partage chaque octet entre 2 cases
            // pour un table de taille 4 ma premiere colonne est sur le premier octet 1 premiers bits a droite sont pour la case 0
            // mes 4 d'apres sont pour la case 1
            for (int pos = 0; pos < ((TAILLE*TAILLE)-1) ; pos++) {// mon tableau est une flat list
                if ((pos+1)%TAILLE==0) {// calcule si je me trouve a la fin de la ligne
                    // calcule si je me trouve dans le debut de l'octet ou la fin
                    addDirection(pos,0);
                }else {
                    addDirection(pos,1);
                }
            }
        }








/*void largueur() {
    Index file[(TAILLE*TAILLE)/2]={};
    file[0]={0,0};
    for (auto & i : visite) {
        i={0,0};
    }
    int debut=0;
    int fin=1;
    int finV=1;
    while (fin!=debut) {
        Index tete = file[debut];
        debut++;
        //std::cout << tete.x  <<""<< tete.y << "voici la tete"  << std::endl  ;

        if ((tete.y)>0
            and schemaLab[tete.y][tete.x][0][(newlab.TAILLE_G/2)]==1
            and schemaLab[tete.y-1][tete.x][(newlab.TAILLE_G-1)][(newlab.TAILLE_G/2)]==1
            and !inTab(finV,visite,{tete.y-1,tete.x})) {
            file[fin]={tete.y-1,tete.x};
            visite[finV]={tete.y-1,tete.x};
            finV++;
            fin=(fin+1)%30;
            //std::cout << tete.x <<""<< tete.y-1 << "enfant de la tete1" << std::endl  ;
        }if (tete.x>0
            and schemaLab[tete.y][tete.x][(newlab.TAILLE_G/2)][0]==1
            and schemaLab[tete.y][tete.x-1][(newlab.TAILLE_G/2)][newlab.TAILLE_G-1]==1
            and !inTab(finV,visite,{tete.y,tete.x-1})) {
            file[fin]={tete.y,tete.x-1};
            visite[finV]={tete.y,tete.x-1};
            finV++;
            fin=(fin+1)%30;
            //std::cout << tete.x-1 <<""<< tete.y << "enfant de la tete2" << std::endl  ;
        }if (tete.y<(TAILLE-1)
            and schemaLab[tete.y][tete.x][newlab.TAILLE_G-1][(newlab.TAILLE_G/2)]==1
            and schemaLab[tete.y+1][tete.x][0][(newlab.TAILLE_G/2)]==1
            and !inTab(finV,visite,{tete.y+1,tete.x})) {

            file[fin]={tete.y+1,tete.x};
            visite[finV]={tete.y+1,tete.x};
            finV++;
            fin=(fin+1)%30;
            //std::cout << tete.x <<""<< tete.y+1 << "enfant de la tete3" << std::endl  ;
        }if (tete.x<(TAILLE-1)
            and schemaLab[tete.y][tete.x][(newlab.TAILLE_G/2)][newlab.TAILLE_G-1]==1
            and schemaLab[tete.y][tete.x+1][(newlab.TAILLE_G/2)][0]==1
            and !inTab(finV,visite,{tete.y,tete.x+1})) {
            file[fin]={tete.y,tete.x+1};
            visite[finV]={tete.y,tete.x+1};
            finV++;
            fin=(fin+1)%30;
            //std::cout << tete.x+1 <<""<< tete.y << "enfant de la tete4" << std::endl  ;
        }
    }
}*/





};

#endif UNTITLED1_LABY_H