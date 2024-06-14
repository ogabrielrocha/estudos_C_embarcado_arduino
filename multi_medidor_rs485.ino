// Protótipo para leitura dos dados do medidor Multi-K
//Gabriel Rocha e Luiz Sena
#include <ModbusMaster.h>

//Data Enable (DE) and Receiver Enable (/RE)
#define MAX485_DE      21
#define MAX485_RE_NEG  22

// Instancia Objeto ModbusMaster
ModbusMaster node;

uint8_t resultMain;

int endereco_Modulos[] {
  20 // 0 - TRAFO 1, 1250 KVa, 13800/380 - Aliger Labs
};

int vetor_endereco[] {
  0x02, // 0 - Tensão Trifásica [V] - UO
  0x04, // 1 - Corrente Trifásica [A] - IO
  0x06, // 2 - Fator de potencia Trifásico - FP
  0x08, // 3 - Potência Aparente Trifásica [VA] - SO
  0x0A, // 4 - Potência Reativa Trifásica [VAr] - QO
  0x0C, // 5 - Potência Ativa Trifásica [W] - PO
  0x0E, // 6 - Frequencia [Hz] - F
  0x3E, // 7 - Demanda Ativa [W] - DA
  0x42, // 8 - Demanada Aparente [VA] - DS
  0x62  // 9 - Contador Parcial - EAP
};

float vetor_resultado[10];

//Union para obter o valor de leitura:
union leitura {
  unsigned char vetor[4];
  float valorFloat;
} rs485;


void preTransmission() {
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission() {
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup() {
  // Atribui pinos como saída

  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);

  // inicializa modo de recebimento
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  // Atribui velocidade de comunicação (Baud Rate)
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N2);

  // Callbacks - Permite configurar o transeiver RS485 corretamente
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop() {
  float leitura_medidor(20)
  delay(1000);
}

/*************************************************************
  Realiza a leitura dos dados via RS485 do Multimedidor
*************************************************************/
float leitura_medidor(uint8_t deviceID) {

  node.begin(deviceID, Serial2);
  Serial.println();
  Serial.print("lendo dispositivo no endereço: ");
  Serial.println(deviceID);

  for (int i = 0; i < 10; i ++) {
    resultMain = node.readInputRegisters(vetor_endereco[i], 2); // Realiza a leitura do registrador 9 ate 14 (0x0E)
    delay(50);

    if (resultMain == node.ku8MBSuccess) {
      rs485.vetor[0] = (node.getResponseBuffer(0x00) >> 8) & 0xFF;  //Parte Alta Word 1
      rs485.vetor[1] = node.getResponseBuffer(0x00) & 0xFF;         //Parte Baixa Word 1
      rs485.vetor[2] = (node.getResponseBuffer(0x01) >> 8) & 0xFF;  //Parte Alta Word 2
      rs485.vetor[3] = node.getResponseBuffer(0x01) & 0xFF;        //Parte Baixa Word 2
    }
    else {
      Serial.println("Falha de comunicacao com o medidor");
      rs485.vetor[0] = 00;      //Parte Alta Word 1
      rs485.vetor[1] = 00;      //Parte Baixa Word 1
      rs485.vetor[2] = 00;      //Parte Alta Word 2
      rs485.vetor[3] = 00;      //Parte Baixa Word 2
    }
    vetor_resultado[i] = rs485.valorFloat;
    Serial.print("Indice: ");
    Serial.println(i);
    Serial.print("Valor: ");
    Serial.println(rs485.valorFloat);
    Serial.println("---------------------------------------------------");
  }
}
