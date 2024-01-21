//---CAPTEUR DE PARTICULES FINES----oct 2019 - Tony Vanpoucke (Edulab, Rennes 2)--------------------//
//                                                                                                  //
//   Petit capteur de particules fines avec affichage 7 segements et seuil d'alerte                 //
//--------------------------------------------------------------------------------------------------//

// ----- PARAMETRES MODIFIABLES ----------------------------------//

int SeuilPM25 = 15;       //Seuil d'alerte aux PM 2,5 (15 μg/m3 en moyenne annuelle)
int SeuilPM10 = 30;       //Seuil d'alerte aux PM 10 (30 μg/m3 en moyenne annuelle)
int Note = 1000;          //fréquence de la note d'alerte (1000 par défaut)
int nbBips = 4;           //nombre de bips par alertes (4 par défaut)
int clignotte = 400;      //vitesse de clignottement des messages d'alerte (400 par défaut)
float luminosite = 2.5;   //luminosité des écrans

// ----- Variables du programme

int PM10 = 0;
int PM25 = 0;
bool buzz = 0;

// ----- Bibliothèques utilisées

#include "SdsDustSensor.h"
#include <Arduino.h>
#include <TM1637Display.h>

// ----- Branchements arduino

int rxPin = 7;
int txPin = 6;
int piezoPin = 13;

// Branchelent cran 1
#define CLK1 10
#define DIO1 11

// Ecran 2
#define CLK2 8
#define DIO2 9

TM1637Display display1(CLK1, DIO1);
TM1637Display display2(CLK2, DIO2);

SdsDustSensor sds(rxPin, txPin);

// ----- INITIALISATION ---------------------------------------//

void setup()
{
  Serial.begin(9600);
  sds.begin();
  display1.setBrightness(luminosite);
  display2.setBrightness(luminosite);
  SeuilPM25 = SeuilPM25 * 100;
  SeuilPM10 = SeuilPM10 * 100;
}

// ----- PROGRAMME ---------------------------------------//

void loop()
{
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };      //affichage vide
  PmResult pm = sds.readPm();                        //scan le capteur de particules
  if (pm.isOk()) {                                   //s'il fonctionne
    PM10 = ((pm.pm10) * 100);                        //stock les valeurs du capteur dans deux variables
    PM25 = ((pm.pm25) * 100);
    Serial.println(PM10);
    Serial.println(PM25);

    if ((SeuilPM25 < PM25) || (SeuilPM10 < PM10)) { //Si une des valeurs dépasse un seuil ...
      display1.showNumberDecEx(PM25, 64, false);    //Affiche les valeurs dans les écrans 7 segements
      display2.showNumberDecEx(PM10, 64, false);
      delay(clignotte);
      if (buzz == 0) {                              //Déclanche un signal sonore
          buzz = 1;
          Serial.println("buzz");
        for (int i = 0; i <= nbBips; i++) {
          tone(piezoPin, Note);
        delay(clignotte);
        noTone(piezoPin);
        delay(clignotte);
        }
        
      }
      if (SeuilPM25 < PM25) {                       //Fait clignotter l'affichage de PM25
        display1.setSegments(blank);
      }
      if (SeuilPM10 < PM10) {                       //Fait clignotter l'affichage de PM10
        display2.setSegments(blank);
      }
      delay(clignotte);

    }
    else {
      display1.showNumberDecEx(PM25, 64, false);    //Sinon valeurs dans les écrans 7 segements
      display2.showNumberDecEx(PM10, 64, false);
      buzz = 0;
    }
  }
}

// --------------------------------------------------------//
