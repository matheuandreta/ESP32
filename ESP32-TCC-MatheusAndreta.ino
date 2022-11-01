//******************************************************************************************************
//Código desenvolvido para o Trabalho de Conclusão de Curso
//Autor: Matheus Henrique Andreta
//Curso: Engenharia de Computação
//Faculdade Gran Tietê
//******************************************************************************************************

//Inclui a biblioteca do servidor Thinker.io
#include <ThingerESP32.h>

//Definição do parâmetros para conexão com a plataforma Thinger.io
#define THINGER_SERIAL_DEBUG
#define USERNAME "matheusandreta"
#define DEVICE_ID "Esp32Rega"
#define DEVICE_CREDENTIAL "2Ckr9KWC9OmxWfXs"
//Dados para conectar no WIFI
#define SSID "Andreta"
#define SSID_PASSWORD "mhandreta"

//Biblioteca Sensor de Temperatura
#include <Adafruit_Sensor.h>
#include "DHT.h"

//Determina os dados para fazer a conexão com a plataforma Thinger.io
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//Pino de acesso do sensor de temperatura DHT11 
const int DHTPIN = 13;

//Define o tipo de sensor
#define DHTTYPE    DHT11 
//Define os pinos dos lEDs indicadores
#define ledamarelo 5
#define ledvermelho 2
#define ledverde 23    

//Define Variáveis do DHT
DHT dht(DHTPIN, DHTTYPE);


//Criando variáveis
float humidity;     
float temperature;  
int pot;

void setup() {
  //Definição dos pinos como saida
  pinMode(ledvermelho,OUTPUT);
  pinMode(ledamarelo,OUTPUT);
  pinMode(ledverde,OUTPUT);

  //Parâmetros para fazer a conexão WIFI
  thing.add_wifi(SSID, SSID_PASSWORD);

  //Criação das variaveis para envio dos pacotes
  thing["Sensores"] >> [](pson& out){
      out["Temperatura"] = temperature;
      out["Humidade"] = humidity;
      out["Umidadesolo"] = pot;
      };

  //Inicia o sensor DHT11 
  dht.begin();

  //Definição para deixar as portas logicas em nivel baixo(LEDs desligados)
  digitalWrite(ledamarelo, LOW);
  digitalWrite(ledvermelho, LOW);
  digitalWrite(ledverde, LOW);
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

  //Executa a função da umidade do solo 
  if (pot >= 40 && pot <= 70 ) {
      //Liga o LED indicador
      digitalWrite(ledamarelo, HIGH);
      digitalWrite(ledvermelho, LOW);
      digitalWrite(ledverde, LOW);
     } else if (pot < 40) {
      //Liga o LED indicador
      digitalWrite(ledamarelo, LOW);
      digitalWrite(ledvermelho, HIGH);
      digitalWrite(ledverde, LOW);   
     } else if (pot > 70) {
      //Liga o LED indicador
      digitalWrite(ledamarelo, LOW);
      digitalWrite(ledvermelho, LOW);
      digitalWrite(ledverde, HIGH);
     }
  delay(1000);
}
