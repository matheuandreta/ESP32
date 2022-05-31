#define THINGER_SERIAL_DEBUG

//Inclui a biblioteca do servidor Thinker.io
#include <ThingerESP32.h>

//Definição do parâmetros para conexão com a plataforma Thinger.io
#define USERNAME "matheusandreta"
#define DEVICE_ID "Esp32Rega"
#define DEVICE_CREDENTIAL "2Ckr9KWC9OmxWfXs"
#define SSID "Andreta"
#define SSID_PASSWORD "mhandreta"

//Biblioteca Sensor de Temperatura
#include <Adafruit_Sensor.h>
#include "DHT.h"

//Determina os dados para fazer a conexão com a plataforma
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);


//Pino de acesso do sensor de temperatura DHT11 
const int DHTPIN = 13;

//Defini o tipo de sensor
#define DHTTYPE    DHT11       

//Define Variáveis do DHT
DHT dht(DHTPIN, DHTTYPE);
float humidity;     
float temperature;  

//Criando variáveis
int pot;
bool Trav;

//Declaração da variavel para usar a função millis
unsigned long int tempoAnterior = 0;

void setup() {

  //Definição dos pinos como saida
  pinMode(2,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(5,OUTPUT);

  //Parâmetros para fazer a conexão WIFI
  thing.add_wifi(SSID, SSID_PASSWORD);


  //Criação das variaveis para envio dos pacotes
  thing["Sensores"] >> [](pson& out){
      out["Temperatura"] = temperature;
      out["Humidade"] = humidity;
      out["Umidadesolo"] = pot;
  };

  thing["Led"] << digitalPin(2);
  thing["Valvula"] << digitalPin(33);
  thing["Trava"] << digitalPin(5);
  
  //Inicia o sensor DHT11 
  dht.begin();
  

}

void loop() {
  //Parâmetro que envia os dados para a plataforma Thinger.io
  thing.handle();

  //Leitura dos dados de temperatura e umidade relativa do DHT11 
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  //Leitura do sensor de umidade do solo
  pot = analogRead(33);
  //Converter a entrada 12 bits que é de 0 a 4095, para uma escala de 0 a 100
  pot = map(pot, 4095, 0, 0, 100);

  //Definição de uma trava para bloquear a valvula, quando o usuario quiser manter a valvula desligada
  Trav = digitalRead(5);

  //Estrutura condicional que só irá executar os comandos quando a trava estiver desligada (nivel lógico baixo - "LOW")
  if (Trav == LOW){
       //Executa a função quando a umidade for inferior à 30%
       if (pot < 30) {
          //Liga a valvula e liga o LED indicador
          digitalWrite(33, LOW);
          digitalWrite(2, HIGH);
     
      }
  }

  // Espera o tempo determinado de 10 segundos
  if (millis() - tempoAnterior >= 10000){
      tempoAnterior = millis();
      //Desliga a válvula e o LED indicador
      digitalWrite(33, HIGH);
      digitalWrite(2,LOW);
  }
}
