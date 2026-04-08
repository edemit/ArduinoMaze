
//#include <DFRobot_RGBMatrix.h> // Hardware-specific library
#include "testNewLab.h"
#include "labV2_3D.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Arduino.h>


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
#define SCREEN_WIDTH 128      // OLED display width, in pixels
#define SCREEN_HEIGHT 64      // OLED display height, in pixels

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
uint8_t pointerPot=0; 
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
      if (pos==newlab.posPlayer){
        bitMapLab2[(y * 64) + x + 16]+=0x10;
        bitMapLab2[(y * 64) + x + 32]+=0x10;
        bitMapLab2[(y * 64) + x + 24]+=0x38;
      } 
    }

  }
  display.drawBitmap(64, 0, bitMapLab2, FRAME_WIDTH, FRAME_HEIGHT, 1);
}







void resetVue() {
  lab3D.options[0] = newlab.checkFlag(newlab.posPlayer, (lab3D.angleVu + 1) % 4);
  lab3D.options[1] = newlab.checkFlag(newlab.posPlayer, (lab3D.angleVu) % 4);
  lab3D.options[2] = newlab.checkFlag(newlab.posPlayer, (lab3D.angleVu + 3) % 4);
  lab3D.options[3] = newlab.checkFlag(newlab.posPlayer, (lab3D.angleVu + 2) % 4);
}


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
}


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

  for (uint8_t i = 0; i < 9; i++) {
    lab3D.afficheForward(i, oldOptions, bitMapLab);
    displayShow();
    delay(100);
  }
  if (shiftedView) {  // i shift them back to the right to not break the graphism
    rotate(false);
  }
}


void displayShow() {
  display.setCursor(2, 0);
  display.clearDisplay();
  display.drawBitmap(0, 0, bitMapLab, FRAME_WIDTH, FRAME_HEIGHT, 1);
  afficheLabOled();
  //display.setTextSize(2);
  //display.setCursor(111, 10);
  //display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  //display.print(direction);
  display.display();
}


void potValueUpdate(){
  uint8_t nval= analogRead(analogPin) /88;
  if (nval != valPot) {  //1=bleu  2=rouge 3=vert 0=jaune
    if ((nval ) > 41 or (nval ) < 6) {
      valTest = 3;//TODO : regler le valTest sur la meme metrique que angle
      direction = 'E';
    }
    if ((nval ) > 4 and (nval ) < 18) {
      valTest = 1;
      direction = 'S';
    }
    if ((nval ) > 17 and (nval ) < 30) {
      valTest = 0;
      direction = 'O';
    }
    if (nval > 29 and nval < 42) {
      valTest = 2;
      direction = 'N';
    }
    if ((nval / 12) != (valPot  / 12)) {
      rotate(
        ((nval / 12 ) > (valPot / 12))
        or ((nval  / 12) == 0 and (valPot  / 12) == 3));
    }
    valPot=nval;
    lab3D.affichageAnim(nval % 12, bitMapLab);
    displayShow();
  }
}
void angleUpdate(){
  int nval=(valPot / 12);
  Serial.println("nval");
  Serial.println(nval);
  switch(nval){
    case 0:
      lab3D.angleVu=1;
      break;
    case 1:
      lab3D.angleVu=0;
      break;
    case 2:
      lab3D.angleVu=3;
      break;
    case 3:
      lab3D.angleVu=2;
      break;
  }
  resetVue();
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
  //Serial1.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 20, 21); // RX, TX
  delay(1500);
  // fill the screen with 'black'
  //matrix.fillScreen(matrix.Color333(0, 0, 0));


  pinMode(butUp, INPUT_PULLUP);
  resetVue();
  displayShow();
  valPot = analogRead(analogPin) / 88;
  potValueUpdate();
  angleUpdate();
  lab3D.affichageAnim(valPot % 12, bitMapLab);
  displayShow();
  
}





void loop() {
  delay(50);

  if (mySerial.available()) {
    String msg = mySerial.readString();
    msg.trim(); // remove \r or spaces
    Serial.println(msg);

  }

  /*Serial.println("valtest");
  Serial.println(valTest);
  Serial.println("valpot");
  Serial.println(valPot);
  Serial.println("angle");
  Serial.println(lab3D.angleVu);*/

  potValueUpdate();
  //angleUpdate();

  if (digitalRead(butUp) == LOW) {

    switch (valTest) {
      case 3:  //vert -> en face angle1
        if ((lab3D.options[1] and lab3D.angleVu == 1) or (lab3D.options[2] and lab3D.angleVu == 2)) {
          newlab.posPlayer += 1;
          forwardAnim((lab3D.options[2] and lab3D.angleVu == 2));
          delay(100);
        }
        break;
      case 2:  //rouge -> a gauche angle 2
        if ((lab3D.options[1] and lab3D.angleVu == 2) or (lab3D.options[2] and lab3D.angleVu == 3)) {
          newlab.posPlayer -= newlab.TAILLE;
          forwardAnim((lab3D.options[2] and lab3D.angleVu == 3));
          delay(100);
        }
        break;
      case 1:  //bleu -> derriere angle 3
        if ((lab3D.options[1] and lab3D.angleVu == 0) or (lab3D.options[2] and lab3D.angleVu == 1)) {
          newlab.posPlayer += newlab.TAILLE;
          forwardAnim((lab3D.options[2] and lab3D.angleVu == 1));
          delay(100);
        }
        break;
      case 0:
        if (((lab3D.options[1] and lab3D.angleVu == 3) or (lab3D.options[2] and lab3D.angleVu == 0))) {
          newlab.posPlayer -= 1;
          forwardAnim((lab3D.options[2] and lab3D.angleVu == 0));
          delay(100);
          // jaune ->  a droite angle à
        }
        break;
    }
    lab3D.affichageAnim(valPot % 12, bitMapLab);
    displayShow();
  }
}
