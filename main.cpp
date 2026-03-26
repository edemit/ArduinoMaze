#include <Arduino.h>
#include <DFRobot_RGBMatrix.h> // Hardware-specific library
#include "testNewLab.h"


#define OE   	9
#define LAT 	10
#define CLK 	11
#define A   	A0
#define B   	A1
#define C   	A2
#define D   	A3
#define E   	A4
#define WIDTH   64
#define _HIGH	64

DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, _HIGH);

int vitesse=100;
int pos=0;
int angleVu=1;
int tailleLaby=5;
laby newlab = laby(5,3);

// void rotate(){
//     angleVu=(((angleVu-1)%4  )+4 )%4 ;
//     for (int i = 0; i < 12; ++i) {
//         newLab.affichageAnim(i);
//         //std::this_thread::sleep_for(std::chrono::milliseconds(vitesse));
//         delay(vitesse);
//     }
//     bool old0=newLab.options[0];
//     newLab.options[0]=newLab.options[1];
//     newLab.options[1]=newLab.options[2];
//     newLab.options[2]=newLab.options[3];
//     newLab.options[3]=old0;
//     newLab.affichageAnim(0);
// }


void showLab() {
    for (int y = 0; y < newlab.TAILLE; y++) {
        for (int i = 0; i < 3; ++i) {
            for (int x = 0; x < newlab.TAILLE; x++) {
                int pos=(y*newlab.TAILLE)+x;
                for (int j = 0; j < 3; ++j) {
                    if (i==0) {
                        if (newlab.checkFlag(pos,2)) {
                            matrix.drawPixel((x*(newlab.TAILLE_G+1))+j,(y*(newlab.TAILLE_G+1))+i, matrix.Color333(0, 0, 4));
                        }

                        //std::cout << "\033[36m██\033[0m" << (checkFlag(pos,2) ? "  " : "\033[36m██\033[0m") << "\033[36m██\033[0m";;
                    }if(i==((newlab.TAILLE_G/2)+newlab.TAILLE_G%2)-1) {
                        //std::cout << (checkFlag(pos,3) ? "  " : "\033[36m██\033[0m") << "  " << (checkFlag(pos,1) ? "  " : "\033[36m██\033[0m");;
                        matrix.drawPixel((x*(newlab.TAILLE_G+1))+j,(y*(newlab.TAILLE_G+1))+i, matrix.Color333(0, 0, 4));
                    }if (i==newlab.TAILLE_G-1) {
                        //std::cout << "\033[36m██\033[0m" << (checkFlag(pos,0) ? "  " : "\033[36m██\033[0m") << "\033[36m██\033[0m";;
                        matrix.drawPixel((x*(newlab.TAILLE_G+1))+j,(y*(newlab.TAILLE_G+1))+i, matrix.Color333(0, 0, 4));
                    }
                }
            }
        }
    }
}

void setup() {

  matrix.begin();
  Serial.begin(9600);
  Serial1.begin(9600);

  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));
    showLab();

}

void loop() {
    if (Serial1.available() > 0){
        int cara=Serial1.read();
        // switch(cara){
        //     case 100:
        //         Serial.println("D");
        //         if (posRotate.x+1<newlab.TAILLE
        //             and !(posRotate.y==END.y and posRotate.x+1==END.x))  {
        //             posRotate={posRotate.y,posRotate.x+1};
        //             showLab();
        //         }
        //         break;
        //     case 115:
        //         Serial.println("S");
        //         if (posRotate.y+1<newlab.TAILLE
        //             and !(posRotate.y+1==END.y and posRotate.x==END.x)) {
        //             posRotate  ={posRotate.y+1,posRotate.x};
        //             showLab();
        //         }
        //         break;
        //     case 113:
        //         Serial.println("Q");
        //         if (posRotate.x-1>=0
        //             and !(posRotate.y==END.y and posRotate.x-1==END.x)) {
        //             posRotate={posRotate.y,posRotate.x-1};
        //             showLab();
        //         }
        //         break;
        //     case 122:
        //         Serial.println("Z");
        //         if (posRotate.y-1>=0
        //             and !(posRotate.y-1==END.y and posRotate.x==END.x)) {
        //             posRotate={posRotate.y-1,posRotate.x};
        //             showLab();
        //         }
        //         break;
        //     case 114:
        //         Serial.println("R");
        //         rotateMatrix(posRotate.x,posRotate.y);
        //         largueur();
        //         showLab();
        //         break;
        // }
    }
}

//
// Created by pat on 12/03/2026.
//




/*
int main(int argc, char *argv[]) {
    laby leLab =laby(tailleLaby, 3);
    newLab.options[0]=leLab.checkFlag(pos,(angleVu+1)%4);
    newLab.options[1]=leLab.checkFlag(pos,(angleVu)%4);
    newLab.options[2]=leLab.checkFlag(pos,(angleVu+3)%4);
    newLab.options[3]=leLab.checkFlag(pos,(angleVu+2)%4);
    newLab.affichageAnim(0);
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        std::cout << "Tu as tape : " << input << std::endl;
        std::cout << newLab.options[0]<< newLab.options[1] << newLab.options[2] << newLab.options[3] << std::endl;
        if (input=="r") {//faudrait mettre un ptit switch
            rotate();
            std::cout<<" nouvel etat "<< newLab.options[0]<< newLab.options[1] << newLab.options[2] << newLab.options[3] << std::endl;
        }
        else if (input=="z") {
            if (newLab.options[1]) {
                std::cout<<" ancienne position "<<std::endl;
                switch (angleVu) {
                    case 3:
                        pos-=1;
                        break;
                    case 2:
                        pos-=tailleLaby;
                        break;
                    case 1:
                        pos+=1;
                        break;
                    case 0:
                        pos+=tailleLaby;
                        break;
                }
                newLab.options[0]=leLab.checkFlag(pos,(angleVu+1)%4);
                newLab.options[1]=leLab.checkFlag(pos,(angleVu)%4);
                newLab.options[2]=leLab.checkFlag(pos,(angleVu+3)%4);
                newLab.options[3]=leLab.checkFlag(pos,(angleVu+2)%4);
                newLab.affichageAnim(0);
                std::cout<<" nouvelle position "<<pos<<std::endl;
                std::cout<<" nouvel etat "<< newLab.options[0]<< newLab.options[1] << newLab.options[2] << newLab.options[3] << std::endl;
            }
        }

        if (((tailleLaby*tailleLaby)-1)==pos) {
           break;
        }


    }
}
*/











/**
 * Calcul les pieces pouvant etre parcourue depuis le debut de mon labyrinthe
 * avec un parcour en largueur
 */
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
}

*/


