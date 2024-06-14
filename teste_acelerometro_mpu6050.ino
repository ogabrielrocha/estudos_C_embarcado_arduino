#include "Wire.h"

// ----- Definições -----
// Endereço do MPU6050 para o pino AD0 em baixa
#define MPU_ADDR  0x68

// Limite de aceleração para objeto parado
#define STOP_OFFSET  300

// ----- Protótipo das funções -----
uint8_t mpu6050_init();
void mpu6050_offset(int16_t cal_ac_x, int16_t cal_ac_y, int16_t cal_ac_z);
void mpu6050_write(uint8_t address, int16_t val, uint8_t size);
int16_t mpu6050_read(uint8_t address, uint8_t size);


// ----- Variáveis globais -----
// Variavel para armazenar o tempo entre cada amostra
unsigned long t_amostra = 0;
// Variaveis para verificar se o objeto está parado
unsigned long t_parado = 0;
bool parado = true;
// Variavel para guardar a distancia deslocada
float dist = 0;

void setup()
{
  Serial.begin(9600);
 
  // Inicializa a comunicação com o MPU6050
  // definindo os offsets de calibração para a
  // aceleração em x, y e z respectivamente
  uint8_t resp = mpu6050_init();

  if (!resp)
  {
    Serial.println("Falha na conexão do MPU6050");
  }
  else
  {
    Serial.println("MPU6050 conectado");
  }

  // Calibração
  // Comente esta linha se você não sabe os valores do seu sensor
  mpu6050_offset(-2267, -1111, 504);

  t_amostra = micros();
}

void loop()
{
  // Variaveis para armazenar o valor medido
  //int16_t ac_x = mpu6050_read(0x3B, 2);
  int16_t ac_y = mpu6050_read(0x3D, 2);
  //int16_t ac_z = mpu6050_read(0x3F, 2);

  // Cria uma variável auxiliar para manipular o valor de aceleração
  // Mude para o eixo desejado
  float aux_ac = float(ac_y);

  // ----- Detecção de objeto parado -----
  // Aceleração baixa e mais de 50ms constante
  if(abs(aux_ac) < STOP_OFFSET && abs(millis() - t_parado) > 50)
  {
    parado = true;
   
    t_amostra = micros();
  }
  else if(fabs(aux_ac) >= STOP_OFFSET)
  {
    t_parado = millis();
    parado = false;
  }

  // ----- Conversão do valor de aceleração -----
  // Converte o valor medido para m/s^2
  aux_ac = ((aux_ac + 32768.0) * 4.0/65536.0 - 2.0) * 9.81;

 
  // ----- Calculo da distancia deslocada -----
  //Se está parado
  if (parado)
  {
    if(dist != 0.0)
    {
      Serial.println("Distância deslocada (em y): ");
      // A função retorna o valor em metros,
      // então multiplica por 100 para converter para centímetros
      Serial.print(dist*100, 2);
      Serial.println(" cms em y.");
    }
    dist = 0.0;
  }
  // Está se movendo
  else
  {
    // Calcula o tempo percorrido
    t_amostra = micros() - t_amostra;
 
    dist = calculo_trapezio(dist, aux_ac, t_amostra);
   
    t_amostra = micros();
  }
}

/*
 * Calcula em tempo real a distância percorrida pela regra do trapézio
 * Parâmetros:
 *  - dist - distância que está sendo calculada
 *  - acel - aceleração medida atualmente
 *  - tempo - tempo em microssegundos gasto desde a ultima medição
 */
float calculo_trapezio(float dist, float acel, unsigned long tempo)
{
  // Armazena o ultimo valor de aceleração e velocidade dentro da própria
  // função declarando a variável como static (no 1º momento ela será 0)
  static float last_acel = 0.0;
  static float last_vel = 0.0;
 
  float vel;
  // Converte o tempo para um valor em segundos
  float t = (float)tempo/1000000.0;

  // Se o movimento acabou de começar, reinicia a velocidade e a aceleração
  if(dist == 0.0)
  {
    last_vel = 0.0;
    last_acel = 0.0;
  }

  // Calculo utilizando a regra do trapézio
  // Soma a velocidade anterior
  vel = last_vel + (last_acel + acel) * t / 2.0;
 
  dist = dist + (last_vel + vel) * t / 2.0;
 
  // Atualiza os valores antigos de aceleração e velocidade
  last_acel = acel;
  last_vel = vel;

  return dist;
}


// ----- Funções para comunicar com o MPU6050 -----

/*
 * Inicia a comunicação com o MPU6050 e retorna 1 se for bem sucedida
 */
uint8_t mpu6050_init()
{
  Wire.begin(); //Inicia a comunicação I2C
 
  // Lê o endereço do mpu para verificar se ele está conectado
  // se não estiver, sai da função  
  uint8_t id = mpu6050_read(0x75, 1);

  if(id != MPU_ADDR)
  {
    return 0;
  }

  // Define a fonte de clock do sensor para maior estabilidade
  // 0x6B = registrador PWR_MGMT_1
  // 4 = Clock externo (32.768kHz)
  // 5 = Clock externo (19.2MHz)
  mpu6050_write(0x6B, 4, 1);

  return 1;
}

/*
 * Define os valores de offset do MPU6050
 * Parâmetros:
 *  - cal_ac_x: ofsset de calibração da aceleração em x
 *  - cal_ac_y: ofsset de calibração da aceleração em y
 *  - cal_ac_z: ofsset de calibração da aceleração em z
 */
void mpu6050_offset(int16_t cal_ac_x, int16_t cal_ac_y, int16_t cal_ac_z)
{
  // Acel x
  mpu6050_write(0x06, cal_ac_x, 2);

  // Acel y
  mpu6050_write(0x08, cal_ac_y, 2);
 
  // Acel z
  mpu6050_write(0x0A, cal_ac_z, 2);
}

/*
 * Escreve em um registrador do MPU6050
 * Parâmetros:
 *  - address: endereço do registrador
 *  - val: valor a ser escrito no registrador
 *  - size: quantidade de bytes para escrever (o máximo são 2)
 */
void mpu6050_write(uint8_t address, int16_t val, uint8_t size)
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(address);
  if(size == 2)
  {
    Wire.write(val >> 8); // MSByte
  }
  Wire.write(val & 0xFF); // LSByte
  Wire.endTransmission();
}

/*
 * Retorna o valor de um registrador (ou dois consecutivos) do MPU6050
 * Parâmetros:
 *  - address: endereço do registrador
 *  - size: quantidade de bytes para ler (o máximo são 2)
 */
int16_t mpu6050_read(uint8_t address, uint8_t size)
{
  int16_t data = 0;

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(address);
  Wire.endTransmission(false);
 
  Wire.requestFrom(MPU_ADDR, size);
  if(size == 2)
  {
    data = Wire.read() << 8;
  }
  data |= Wire.read();

  return data;
}
