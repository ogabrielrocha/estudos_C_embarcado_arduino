// Autor Gabriel Rocha
// Prototipagem Display com Matrix de Led 64x16 - Nestlé Safety

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Font_Data.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define NUM_ZONES 2
#define ZONE_SIZE 8
#define MAX_DEVICES (NUM_ZONES * ZONE_SIZE)

#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5
MD_Parola MATRIZ_LED = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  
void MD_Parola::displayScroll  ( const char *    pText,
                                 textPosition_t  align,
                                 textEffect_t    effect,
                                 uint16_t        speed 
                               );   


}
