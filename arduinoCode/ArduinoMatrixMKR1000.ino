
#include "testNewLab.h"
#include "labV2_3D.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Arduino.h>
#include "homeAnimation.h"


#define OE 9
#define LAT 10
#define CLK 11
#define A A0
#define B A1
#define C A2
#define D A3
#define E A4
#define WIDTH 64
#define _HIGH 64


int analogPin = A2;
const uint8_t butUp = 5;

#define SCREEN_I2C_ADDR 0x3C
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

Adafruit_SH1106G display(128, 64, &Wire);
#define FRAME_DELAY (42)
#define FRAME_WIDTH (64)
#define FRAME_HEIGHT (64)
#define FRAME_COUNT (sizeof(frames) / sizeof(frames[0]))

HardwareSerial mySerial(1);

const uint8_t taille = 7;
const uint8_t taille_G = 7;
uint8_t valPot = 0;
//uint8_t valPot[3] = {};//value of the potentiometer the 3 last to get an average and not have sprite flicks
byte bitMapLab[512];
laby newlab(8, 3);
labV2_3D lab3D;
uint8_t pointerPot = 0;
char direction = 'E';
uint valTest = 0;



/*int averagePot(){
  int totalPot=0;
  for(uint8_t i=0; i<3 ; i++){
    totalPot+=valPot[i];
  }
  return totalPot/3;
}*/

void afficheLabOled() {
  //uint64_t tabAffichage[64]={};
  //newLab.affichageAnim(0,tabAffichage);
  byte bitMapLab2[512] = {};
  for (int i = 0; i < 512; ++i) {
    bitMapLab2[i] = 0xFE;
  }
  for (uint8_t y = 0; y < newlab.TAILLE; ++y) {
    for (uint8_t x = 0; x < newlab.TAILLE; ++x) {
      int posY = y * (newlab.TAILLE_G + 1);
      int posX = x * (newlab.TAILLE_G + 1);
      int pos = y * newlab.TAILLE + x;
      bool bas = newlab.checkFlag(pos, 0);
      bool droite = newlab.checkFlag(pos, 1);
      bool haut = newlab.checkFlag(pos, 2);
      bool gauche = newlab.checkFlag(pos, 3);
      if (haut) {
        bitMapLab2[(y * 64) + x] = 0xC6;
        bitMapLab2[(y * 64) + x + 8] = 0xC6;
      }
      if (gauche) {
        if (droite) {
          bitMapLab2[(y * 64) + x + 16] = 0x00;
          bitMapLab2[(y * 64) + x + 24] = 0x00;
          bitMapLab2[(y * 64) + x + 32] = 0x00;
        } else {
          bitMapLab2[(y * 64) + x + 16] = 0x06;
          bitMapLab2[(y * 64) + x + 24] = 0x06;
          bitMapLab2[(y * 64) + x + 32] = 0x06;
        }
      } else {
        if (droite) {
          bitMapLab2[(y * 64) + x + 16] = 0xC0;
          bitMapLab2[(y * 64) + x + 24] = 0xC0;
          bitMapLab2[(y * 64) + x + 32] = 0xC0;
        } else {
          bitMapLab2[(y * 64) + x + 16] = 0xC6;
          bitMapLab2[(y * 64) + x + 24] = 0xC6;
          bitMapLab2[(y * 64) + x + 32] = 0xC6;
        }
      }
      if (bas) {
        bitMapLab2[(y * 64) + x + 40] = 0xC6;
        bitMapLab2[(y * 64) + x + 48] = 0xC6;
      }
      bitMapLab2[((y)*64) + x + 56] = 0x00;
      if (pos == newlab.posPlayer) {
        bitMapLab2[(y * 64) + x + 16] += 0x10;
        bitMapLab2[(y * 64) + x + 32] += 0x10;
        bitMapLab2[(y * 64) + x + 24] += 0x38;
      }
    }
  }
  display.drawBitmap(64, 0, bitMapLab2, FRAME_WIDTH, FRAME_HEIGHT, 1);
}



/*
reset the options when you enter in a new room 
options is the fact that there is a doorway (not a door)
if there is a doorway it has to connect to an other doorway to have an open door 
*/
void resetVue() {
  lab3D.options[0] = newlab.checkFlag(newlab.posPlayer, (lab3D.angleVu + 1) % 4);
  lab3D.options[1] = newlab.checkFlag(newlab.posPlayer, (lab3D.angleVu) % 4);
  lab3D.options[2] = newlab.checkFlag(newlab.posPlayer, (lab3D.angleVu + 3) % 4);
  lab3D.options[3] = newlab.checkFlag(newlab.posPlayer, (lab3D.angleVu + 2) % 4);
  getDoorClosed(lab3D.angleVu);
}

/*
rotate my options, used to loop the animation or rotation with the potentiometer
*/
void rotate(bool rotToRight) {
  if (rotToRight) {
    lab3D.angleVu = (((lab3D.angleVu - 1) % 4) + 4) % 4;
    bool old0 = lab3D.options[0];
    lab3D.options[0] = lab3D.options[1];
    lab3D.options[1] = lab3D.options[2];
    lab3D.options[2] = lab3D.options[3];
    lab3D.options[3] = old0;
  } else {
    lab3D.angleVu = ((lab3D.angleVu + 1) % 4);
    bool old3 = lab3D.options[3];
    lab3D.options[3] = lab3D.options[2];
    lab3D.options[2] = lab3D.options[1];
    lab3D.options[1] = lab3D.options[0];
    lab3D.options[0] = old3;
  }
  getDoorClosed(lab3D.angleVu);//get the new door states
}

/*launch the animation forward to go to a new room
*/
void forwardAnim(bool shiftedView) {
  bool oldOptions[4] = { lab3D.options[0], lab3D.options[1], lab3D.options[2], lab3D.options[3] };  //old layout of the room used a the stat of the animation
  resetVue();
  // reset the room layont in the class to the one we will ente
  if (shiftedView) {  //if i'm in the space where i see mostly the door that not in my angle i need to shift to the left all options
    rotate(true);
    bool old1 = oldOptions[0];
    oldOptions[0] = oldOptions[1];
    oldOptions[1] = oldOptions[2];
    oldOptions[2] = oldOptions[3];
    oldOptions[3] = oldOptions[0];
  }
  getDoorClosed(lab3D.angleVu);
  for (uint8_t i = 0; i < 9; i++) {
    lab3D.afficheForward(i, oldOptions, bitMapLab,newlab.posPlayer==63);
    displayShow();
    delay(100);
  }

  if (shiftedView) {  // i shift them back to the right to not break the graphism
    rotate(false);
  }
}

/*shows the labyrinth an the cardinal points
*/
void displayShow() {
  display.setCursor(2, 0);
  display.clearDisplay();
  display.drawBitmap(32, 0, bitMapLab, FRAME_WIDTH, FRAME_HEIGHT, 1);
  //afficheLabOled(); debugging function to show the lab
  display.setTextSize(2);
  display.setCursor(111, 10);
  display.setTextColor(SH110X_WHITE);
  display.print(direction);
  display.display();
}

/* normalizes the values of my potentiometer get the frame the angle and the cardinals points it must show
*/
void potValueUpdate() {
  uint8_t nval = analogRead(analogPin) / 88;
  if (nval != valPot) {  //1=bleu  2=rouge 3=vert 0=jaune
    if ((nval) > 41 or (nval) < 6) {
      valTest = 3;  //TODO : regler le valTest sur la meme metrique que angle
      direction = 'E';
    }
    if ((nval) > 4 and (nval) < 18) {
      valTest = 1;
      direction = 'S';
    }
    if ((nval) > 17 and (nval) < 30) {
      valTest = 0;
      direction = 'O';
    }
    if (nval > 29 and nval < 42) {
      valTest = 2;
      direction = 'N';
    }
    if ((nval / 12) != (valPot / 12)) {
      rotate(
        ((nval / 12) > (valPot / 12))
        or ((nval / 12) == 0 and (valPot / 12) == 3));
    }
    valPot = nval;
    lab3D.affichageAnim(nval % 12, bitMapLab, lab3D.getDC[0], lab3D.getDC[1]);
    displayShow();
  }
}
/*use in case the potentiometer is touched during an animation to get the new angleVu
*/
void angleUpdate() {
  int nval = (valPot / 12);
  switch (nval) {
    case 0:
      lab3D.angleVu = 1;
      break;
    case 1:
      lab3D.angleVu = 0;
      break;
    case 2:
      lab3D.angleVu = 3;
      break;
    case 3:
      lab3D.angleVu = 2;
      break;
  }
  resetVue();
}

/*
get the state of the visible doorways (closed or open) 
*/
void getDoorClosed(uint8_t angle){
    lab3D.getDC[0]=true; 
    lab3D.getDC[1]=true; 
    switch(angle){
        case 0://
            if ((newlab.posPlayer+8)<=63){
                lab3D.getDC[0]=!(newlab.checkFlag(newlab.posPlayer+8,  2) and lab3D.options[1]);
            }
            if ((newlab.posPlayer-1)>=0 and (newlab.posPlayer%8)!=0){
                lab3D.getDC[1]=!(newlab.checkFlag(newlab.posPlayer-1,  1) and lab3D.options[2]);
            }
            break;
        case 1://
            if ((newlab.posPlayer+1)<=63 and ((newlab.posPlayer+1)%8)!=0){
                lab3D.getDC[0]=!(newlab.checkFlag(newlab.posPlayer+1,  3) and lab3D.options[1]);
            }
            if ((newlab.posPlayer+8)<=63){
                lab3D.getDC[1]=!(newlab.checkFlag(newlab.posPlayer+8,  2) and lab3D.options[2]);
            }
            break;
        case 2://
            if ((newlab.posPlayer-8)>=0){
                lab3D.getDC[0]=!(newlab.checkFlag(newlab.posPlayer-8,  0) and lab3D.options[1]);
            }
            if ((newlab.posPlayer+1)<=63 and ((newlab.posPlayer+1)%8)!=0){
                lab3D.getDC[1]=!(newlab.checkFlag(newlab.posPlayer+1,  3) and lab3D.options[2]);
            }
            break;
        case 3://
            if ((newlab.posPlayer-1)>=0 and (newlab.posPlayer%8)!=0){
                
                lab3D.getDC[0]=!(newlab.checkFlag(newlab.posPlayer-1,  1) and lab3D.options[1]);
            }
            if ((newlab.posPlayer-8)>=0){
                lab3D.getDC[1]=!(newlab.checkFlag(newlab.posPlayer-8,  0) and lab3D.options[2]);
            }
            break;
    }
}


void setup() {
  randomSeed(analogRead(A0) + micros());
  newlab.tabCreation();
  newlab.creationRandLab();
  newlab.newDimLab();
  Wire.begin();
  display.begin(SCREEN_I2C_ADDR, true);
  //matrix.begin();
  Serial.begin(9600);
  Serial.println("START");
  //Serial1.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, 20, 21);  // RX, TX
  delay(4000);
  // fill the screen with 'black'
  //matrix.fillScreen(matrix.Color333(0, 0, 0));


  pinMode(butUp, INPUT_PULLUP);
  resetVue();
  displayShow();
  valPot = analogRead(analogPin) / 88;
  potValueUpdate();
  angleUpdate();
  lab3D.affichageAnim(valPot % 12, bitMapLab,true,true);
  displayShow();
  bool finBoucle=false;
  int indexAnimationHome=0;
  home_animation hAnim;
  newlab.shuffleMatrix();
  //wating screen during the web connection
  while(!finBoucle){
    display.clearDisplay();
    display.drawBitmap(32, 0, hAnim.epd_bitmap_[indexAnimationHome], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    indexAnimationHome++;
    indexAnimationHome%=8;
    if (digitalRead(butUp) == LOW) {
      finBoucle=true;
    }
    if (mySerial.available()) {
      String msg = mySerial.readString();
      msg.trim();  // remove \r or spaces
      Serial.println(msg);
      if (msg=="200"){
        finBoucle=true;
      }
    }
    Serial.println("Waiting for serveur connection");
    delay(120);
  }
  //send the data of the labyrinth to the website
  for(int i =0; i<(newlab.TAILLE*newlab.TAILLE)/2; i++ ){
    Serial.println(newlab.lab[i]);
    mySerial.println(newlab.lab[i]);
    delay(50);
  }
  getDoorClosed(lab3D.angleVu);
  lab3D.affichageAnim(valPot % 12, bitMapLab, lab3D.getDC[0], lab3D.getDC[1]);
  displayShow();
}





void loop() {
  if (!newlab.fin){
    delay(50);

    if (mySerial.available()) {//communication with web
      String msg = mySerial.readStringUntil('\n');
      msg.trim();

      if (msg.indexOf("/") != -1) {// message under the format command/value
        String command = msg.substring(0, msg.indexOf("/"));
        String posRotate = msg.substring(msg.indexOf("/") + 1);

        if (command == "rotate") {
          newlab.posPlayer2 = posRotate.toInt();

          if (newlab.posPlayer2 == newlab.posPlayer) {//you can't rotate a player 
            mySerial.println("rotate/400");//sendback refusal
          } else {
            newlab.rotateMatrix(newlab.posPlayer2);//rotate the piece
            mySerial.println("rotate/200");//send back confirmation
            getDoorClosed(lab3D.angleVu);//update the doors of the player room can open door in front of him
            lab3D.affichageAnim(valPot % 12, bitMapLab, lab3D.getDC[0], lab3D.getDC[1]);
            displayShow();
          }
        }
      }
    }
    potValueUpdate();//update the angles values of the potentionmeter

    if (digitalRead(butUp) == LOW) {
      switch (valTest) {
        /*whats being check in each directions is 
        if are in the animation where you look at the next door and it's open 
        or if the door that you see in front of you is open
         angleVu is the index of an animation from a door to the next one its desync with the directions like  nord sud .... 
         its effective usage is a 45 deg difference 
         after that it launches the animation 
        */
        case 3:  //EST
          if ((lab3D.options[1] and lab3D.angleVu == 1 and !lab3D.getDC[0]) 
          or (lab3D.options[2] and lab3D.angleVu == 2 and !lab3D.getDC[1])) {
            newlab.posPlayer += 1;
            forwardAnim((lab3D.options[2] and lab3D.angleVu == 2));
            mySerial.println("move/right");
            delay(100);
            
          }
          break;
        case 2:  // NORD
          if ((lab3D.options[1] and lab3D.angleVu == 2 and !lab3D.getDC[0]) 
          or (lab3D.options[2] and lab3D.angleVu == 3 and !lab3D.getDC[1])) {
            newlab.posPlayer -= newlab.TAILLE;
            forwardAnim((lab3D.options[2] and lab3D.angleVu == 3));
            mySerial.println("move/up");
            delay(100);
          }
          break;
        case 1:  // OUEST
          if ((lab3D.options[1] and lab3D.angleVu == 0 and !lab3D.getDC[0]) 
          or (lab3D.options[2] and lab3D.angleVu == 1 and !lab3D.getDC[1])) {
            newlab.posPlayer += newlab.TAILLE;
            forwardAnim((lab3D.options[2] and lab3D.angleVu == 1));
            mySerial.println("move/down");
            delay(100);
          }
          break;
        case 0: // SUD 
          if ((lab3D.options[1] and lab3D.angleVu == 3 and !lab3D.getDC[0]) 
          or (lab3D.options[2] and lab3D.angleVu == 0 and !lab3D.getDC[1])) {
            newlab.posPlayer -= 1;
            forwardAnim((lab3D.options[2] and lab3D.angleVu == 0));
            mySerial.println("move/left");
            delay(100);
            // jaune ->  a droite angle à
          }
          break;
      }
      getDoorClosed(lab3D.angleVu);
      lab3D.affichageAnim(valPot % 12, bitMapLab,lab3D.getDC[0],lab3D.getDC[1]);
      displayShow();
      newlab.objectivesUpdate();

    }
  }else{//if the objectif update is the end
    Serial.println("fin");
    mySerial.println("fin");
    display.setCursor(10, 5);
    display.clearDisplay();
    display.setTextSize(4);
    display.setTextColor(SH110X_WHITE);
    display.print("FIN");
    display.display();
  }
}
