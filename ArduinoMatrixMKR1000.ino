
//#include <DFRobot_RGBMatrix.h> // Hardware-specific library
#include "testNewLab.h"
#include "labV2_3D.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
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


int analogPin = A0;
//const int butDown = 2;
const int butUp = 4;
//const int butLeft = 4;
//const int butRight = 1;

#define SCREEN_I2C_ADDR 0x3C  // or 0x3C
#define SCREEN_WIDTH 128      // OLED display width, in pixels
#define SCREEN_HEIGHT 64      // OLED display height, in pixels
#define OLED_RST_PIN -1       // Reset pin (-1 if not available)
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RST_PIN);
#define FRAME_DELAY (42)
#define FRAME_WIDTH (64)
#define FRAME_HEIGHT (64)
#define FRAME_COUNT (sizeof(frames) / sizeof(frames[0]))

int taille = 7;
int taille_G = 7;
int clig = 0;
int valPot = 0;

laby newlab(8, 3);
labV2_3D lab3D;


/*void showLab2() {
  for (int y = 0; y < newlab.TAILLE; y++) {
    for (int i = 0; i < 3; ++i) {
      for (int x = 0; x < newlab.TAILLE; x++) {
        int pos = (y * newlab.TAILLE) + x;
        Serial.println(pos);
        if (i == 0) {
          if (newlab.checkFlag(pos, 2)) {
            Serial.print("██  ██");
          } else {
            Serial.print("██████");
          }
        }
        if (i == 1) {
          if (newlab.checkFlag(pos, 3)) {
            if (newlab.checkFlag(pos, 1)) {
              Serial.print("      ");
            } else {
              Serial.print("    ██");
            }
          } else {
            if (newlab.checkFlag(pos, 1)) {
              Serial.print("██    ");
            } else {
              Serial.print("██  ██");
            }
          }
        }
        if (i == 2) {
          if (newlab.checkFlag(pos, 0)) {
            Serial.print("██  ██");
          } else {
            Serial.print("██████");
          }
        }
        //Serial.print("  ");
      }
      Serial.println();
    }
    //Serial.println();
  }
}
*/
/*
void afficheLabOled() {
  byte bitMapLab[512] = {};
  for (int i = 0; i < 512; ++i) {
    //bitMapLab[ j+(i*8)]=(tabAffichage[i] & (0x000000FF << (j*8)) ) >> (j*8)); //&& 0xFF  ;
    bitMapLab[i] = 0xFF;
  }

  for (int y = 0; y < newlab.TAILLE; ++y) {
    for (int x = 0; x < newlab.TAILLE; ++x) {
      int posY = y * (newlab.TAILLE_G + 1);
      int posX = x * (newlab.TAILLE_G + 1);
      int pos = (y * newlab.TAILLE) + x;
      bool bas = newlab.checkFlag(pos, 0);
      bool droite = newlab.checkFlag(pos, 1);
      bool haut = newlab.checkFlag(pos, 2);
      bool gauche = newlab.checkFlag(pos, 3);
      if (clig > 7 && pos == newlab.posPlayer) {
        bitMapLab[(y * 64) + x] = 0xFE;
        bitMapLab[(y * 64) + x + 8] = 0xFE;
        bitMapLab[(y * 64) + x + 16] = 0xFE;
        bitMapLab[(y * 64) + x + 24] = 0xFE;
        bitMapLab[(y * 64) + x + 32] = 0xFE;
        bitMapLab[(y * 64) + x + 40] = 0xFE;
        bitMapLab[(y * 64) + x + 48] = 0xFE;
      } else {
        if (haut) {
          bitMapLab[(y * 64) + x] = 0xC6;
          bitMapLab[(y * 64) + x + 8] = 0xC6;
        } else {
          bitMapLab[(y * 64) + x] = 0xFE;
          bitMapLab[(y * 64) + x + 8] = 0xFE;
        }
        if (gauche) {
          if (droite) {
            bitMapLab[(y * 64) + x + 16] = 0x00;
            bitMapLab[(y * 64) + x + 24] = 0x00;
            bitMapLab[(y * 64) + x + 32] = 0x00;
          } else {
            bitMapLab[(y * 64) + x + 16] = 0x06;
            bitMapLab[(y * 64) + x + 24] = 0x06;
            bitMapLab[(y * 64) + x + 32] = 0x06;
          }
        } else {
          if (droite) {
            bitMapLab[(y * 64) + x + 16] = 0xC0;
            bitMapLab[(y * 64) + x + 24] = 0xC0;
            bitMapLab[(y * 64) + x + 32] = 0xC0;
          } else {
            bitMapLab[(y * 64) + x + 16] = 0xC6;
            bitMapLab[(y * 64) + x + 24] = 0xC6;
            bitMapLab[(y * 64) + x + 32] = 0xC6;
          }
        }
        if (bas) {
          bitMapLab[(y * 64) + x + 40] = 0xC6;
          bitMapLab[(y * 64) + x + 48] = 0xC6;
        } else {
          bitMapLab[(y * 64) + x + 40] = 0xFE;
          bitMapLab[(y * 64) + x + 48] = 0xFE;
        }
      }
      bitMapLab[((y)*64) + x + 56] = 0x00;
    }
  }
  display.clearDisplay();
  display.drawBitmap(32, 0, bitMapLab, FRAME_WIDTH, FRAME_HEIGHT, 1);
  display.display();
}*/

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


void forwardAnim(){
  bool oldLayout[4]={lab3D.options[0],lab3D.options[1],lab3D.options[2],lab3D.options[3]};//old layout of the room used a the stat of the animation
  resetVue();// reset the room layont in the class to the one we will enter
  byte bitMapLab[512] = {};
  for(int i =0 ; i< 8; i++){
    lab3D.afficheForward(i, oldLayout,bitMapLab);
    display.clearDisplay();
    display.drawBitmap(32, 0, bitMapLab, FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    delay(100);
  }
  lab3D.affichageAnim(0, bitMapLab);
  display.clearDisplay();
  display.drawBitmap(32, 0, bitMapLab, FRAME_WIDTH, FRAME_HEIGHT, 1);
  display.display();
  
}



void setup() {
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  randomSeed(analogRead(A0) + micros());
  newlab.tabCreation();
  newlab.creationRandLab();
  newlab.newDimLab();
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR);
  //matrix.begin();
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1500);
  // fill the screen with 'black'
  //matrix.fillScreen(matrix.Color333(0, 0, 0));
  //showLab();

  //pinMode(butDown, INPUT_PULLUP);
  pinMode(butUp, INPUT_PULLUP);
  //pinMode(butRight, INPUT_PULLUP);
  //pinMode(butLeft, INPUT_PULLUP);
  //Serial.print("debut du test");
  resetVue();
  byte bitMapLab[512] = {};
  lab3D.affichageAnim(0, bitMapLab);
  display.clearDisplay();
  display.drawBitmap(32, 0, bitMapLab, FRAME_WIDTH, FRAME_HEIGHT, 1);
  display.display();
  valPot = analogRead(analogPin);
}



/*
void loop() {
  clig = (clig + 1) % 10;
  afficheLabOled();

  if (digitalRead(butR) == HIGH) {
    newlab.rotateMatrix();
    Serial.println("Button ROTATE");
    delay(200);
  }

  if (digitalRead(butDown) == HIGH) {
    if ((newlab.posPlayer / newlab.TAILLE) + 1 != newlab.TAILLE) {
      newlab.posPlayer += newlab.TAILLE;
    }
    Serial.println("Button down ");
    delay(200);
  }
  if (digitalRead(butUp) == HIGH) {
    if ((newlab.posPlayer / newlab.TAILLE) != 0) {
      newlab.posPlayer -= newlab.TAILLE;
    }
    Serial.println("Button up");
    delay(200);
  }
  if (digitalRead(butLeft) == HIGH) {
    if ((newlab.posPlayer % newlab.TAILLE) != 0) {
      newlab.posPlayer --;
    }
    Serial.println("Button left ");
    delay(200);
  }
  if (digitalRead(butRight) == HIGH) {
    if ((newlab.posPlayer % newlab.TAILLE)+1 !=  newlab.TAILLE ) {
      newlab.posPlayer ++;
    }
    Serial.println("Button right ");
    delay(200);
  }

  delay(FRAME_DELAY);
}*/


int valTest=0;

void loop() {
  valTest++;
  //digitalWrite(0, LOW);
  //digitalWrite(1, LOW);
  //digitalWrite(2, LOW);
  //digitalWrite(3, LOW);
  //digitalWrite(valTest%4, HIGH);
  if (Serial1.available() > 0) {
    char c = Serial1.read();
    Serial.println(c);
    Serial1.println(("reçu : "+c));
  }


  int nval = analogRead(analogPin);
  //Serial.println(nval / 21);
  if (nval != valPot) {
    if ((nval / 21) / 12 != (valPot / 21) / 12){
      //Serial.println((nval / 21) / 12 ,(nval / 21) /12);
      rotate(
        ((nval/21)/12 > (valPot/21)/12)
         or
        ((nval/21)/12 == 0 and (valPot/21)/12)== 3);
    }
    byte bitMapLab[512] = {};
    lab3D.affichageAnim((nval / 21) % 12, bitMapLab);
    display.clearDisplay();
    display.drawBitmap(32, 0, bitMapLab, FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
  }
  valPot = nval;

  if (digitalRead(butUp) == HIGH) {
    if (lab3D.options[1]) {
      switch (lab3D.angleVu) {
        case 3:
          newlab.posPlayer -= 1;
          break;
        case 2:
          newlab.posPlayer -= newlab.TAILLE;
          break;
        case 1:
          newlab.posPlayer += 1;
          break;
        case 0:
          newlab.posPlayer += newlab.TAILLE;
          break;
      }
      forwardAnim();
    }
    delay(200);
  }
}
