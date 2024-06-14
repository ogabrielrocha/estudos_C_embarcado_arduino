#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define SS 5
#define RST 14
#define DIO0 2



void setup() {
   Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");

  LoRa.setPins(SS, RST, DIO0);
  
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
//  LoRa.setSyncWord(0xF3);
//  Serial.println("Inicialização do LoRa OK!");
}

void loop() {
  Serial.print("Sending packet: ");
  LoRa.beginPacket();
  LoRa.print(1);
  LoRa.endPacket();
  delay(10000);
}
