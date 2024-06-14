#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <LoRa.h>
#include <SPI.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

//============================================================//
// DEFINIÇÃO DOS PINOS DE DADOS DOS DISPLAYS E DO MÓDULO LORA //
//============================================================//

#define PIN  27 // PINO DO DISPLAY 1
#define PIN2 33 // PINO DO DISPLAY 2

#define SS   5  // CHIP SELECT DO MÓDULO
#define RST  14 // RESET DO MÓDULO
#define DIO0 2  // PINO DE DADOS 


//==============================================//
// DECLARAÇÃO DE FUNCIONAMENTO DAS MATRIZES RGB //
//==============================================//

Adafruit_NeoMatrix DISPLAY_1 = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

Adafruit_NeoMatrix DISPLAY_2 = Adafruit_NeoMatrix(32, 8, PIN2,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

  
//==================================================//
// DECLARAÇÃO DAS MATRIZES DE CORES DE CADA DISPLAY //
//==================================================//

const uint16_t COLORS_1[] = {
  DISPLAY_1.Color(255, 0, 0),           // Vermelho - 0
  DISPLAY_1.Color(0, 255, 0),           // Verde    - 1
  DISPLAY_1.Color(0, 0, 255),           // Azul     - 2
  DISPLAY_1.Color(255, 255, 0),         // Amarelo  - 3
  DISPLAY_1.Color(255, 0, 255),         // Magenta  - 4
  DISPLAY_1.Color(0, 255, 255),         // Ciano    - 5
  DISPLAY_1.Color(255, 255, 255),       // Branco   - 6
  DISPLAY_1.Color(0, 0, 0)              // Preto    - 7
  }; // (R, G, B)

const uint16_t COLORS_2[] = {
  DISPLAY_1.Color(255, 0, 0),           // Vermelho - 0
  DISPLAY_1.Color(0, 255, 0),           // Verde    - 1
  DISPLAY_1.Color(0, 0, 255),           // Azul     - 2
  DISPLAY_1.Color(255, 255, 0),         // Amarelo  - 3
  DISPLAY_1.Color(255, 0, 255),         // Magenta  - 4
  DISPLAY_1.Color(0, 255, 255),         // Ciano    - 5
  DISPLAY_1.Color(255, 255, 255),       // Branco   - 6
  DISPLAY_1.Color(0, 0, 0)              // Preto    - 7
  }; // (R, G, B)
  

//============================================//
// DECLARAÇÃO DA COR E BRILHO DE CADA DISPLAY //
//============================================//

#define DISPLAY_1_COLOR 5
#define DISPLAY_2_COLOR 2
#define DISPLAY_3_COLOR 4

#define DISPLAY_1_BRILHO 50
#define DISPLAY_2_BRILHO 50
#define DISPLAY_BLINK    14 // QUANTO MAIOR, MAIS LENTO O DISPLAY PISCA - VARIÁVEL DE CONTROLE DE PISCADAS DE DISPLAY_1

//====================================//
// SEÇÃO DA CONFIGURAÇÃO DE MENSAGENS //
//====================================//

String message_1 = "PARE";
String message_2 = "MHE EM MOVIMENTO";
String message_3 = "CAMINHO LIVRE";
String message_4 = "SIGA";
String message_5 = "DALE";


//=====================================================================//
// DECLARAÇÃO DE VARIÁVEIS PARA DETERMINAR O COMPRIMENTOS DAS MATRIZES //
//=====================================================================//

int x1 = DISPLAY_1.width();
int x2 = DISPLAY_2.width();
int x3 = DISPLAY_BLINK; 


//=================//
// INÍCIO DO SETUP //
//=================//

void setup() {
  
//======================================//
// INICIALIZAÇÃO DOS DISPLAYS E DO LORA //
//======================================//


  //  MÓDULO LORA
  //  INÍCIO DA SERIAL PARA VERMOS AS MENSAGENS CHEGANDO
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("LoRa Receiver");
  LoRa.setPins(5, 14, 2);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }  
  
  
 /* while (!LoRa.begin(915E6));{
    Serial.println("Falhou");
    delay(500);
  }*/
  
  //LoRa.setSyncWord(0xF3);
  //Serial.println("Inicialização do LoRa OK!");

  
  // DISPLAY SUPERIOR = DISPLAY_1 
  DISPLAY_1.begin();  
  DISPLAY_1.setTextWrap(false);
  DISPLAY_1.setBrightness(DISPLAY_1_BRILHO);
  DISPLAY_1.setTextColor(DISPLAY_1_COLOR);
 
  // DISPLAY INFERIOR = DISPLAY_2  
  DISPLAY_2.begin();
  DISPLAY_2.setTextWrap(false);
  DISPLAY_2.setBrightness(DISPLAY_2_BRILHO);
  DISPLAY_2.setTextColor(DISPLAY_2_COLOR);
  
}


//================//
// INÍCIO DO LOOP //
//================//

void loop() {

//=======================//
// CONTROLE DO DISPLAY_1 //
//=======================//

  DISPLAY_1.fillScreen(0);
  DISPLAY_1.setCursor(4, 0);
  DISPLAY_1.print(message_4);
  DISPLAY_1.setTextColor(COLORS_1[DISPLAY_1_COLOR]);
  DISPLAY_1.show();
  
 /* if(--x1 < -100){
     x1 = DISPLAY_1.width();
     DISPLAY_1.setTextColor(COLORS_1[DISPLAY_1_COLOR]);
    }  */                      //CASO QUEIRA QUE A MENSAGEM DESLIZE, USE ESSE IF


  /* if(--x3  < 0){
      DISPLAY_1.setTextColor(COLORS_1[DISPLAY_1_COLOR]);
  }      
     if (x3 == -DISPLAY_BLINK) {
      x3 = DISPLAY_BLINK;
  } */                        // CASO QUEIRA QUE A MENSAGEM PISQUE USO ESSE IF


//=======================//
// CONTROLE DO DISPLAY_2 //
//=======================//

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    
    while (LoRa.available()) { 
//      message_5 = LoRa.readString();

      int message_lora = LoRa.read();

      if (message_lora == 1){
      
        DISPLAY_2.fillScreen(0);
        DISPLAY_2.setCursor(x2, 0);
        DISPLAY_2.print(message_5);  
      
        if(--x2 < -70) { 
          x2 = DISPLAY_2.width();
          DISPLAY_2.setTextColor(COLORS_2[DISPLAY_2_COLOR]);    
        }   
        DISPLAY_2.show(); 
     
        delay(30);
      }
   }
 }  
  delay(1); 
}
