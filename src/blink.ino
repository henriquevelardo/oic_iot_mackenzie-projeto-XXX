//Autores: 
//Erik Sardinha
//Henrique Velardo
//John Lennon
//Marcos Felipe

//Data inicio: 01/05/2018
//Data final: 26/05/2018

//Projeto adaptado de projetos encontrados na internet como base

//Importando biblioteca
#include <SPI.h>   
#include <Ethernet.h>
#include <PubSubClient.h>

//Definindo variaveis
String ip = "";
String readString;
boolean statusLamp = LOW;

//Definindo pinos
#define pinoLamp 8

//Definindo rede
byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

//Definindo o broker MQTT (CloudMQTT foi o utilizado)
const char* mqtt_server = "broker.hivemq.com";   //server

//Configuração Ethernet Shield
EthernetClient ethClient;
PubSubClient mqttClient;

void setup() {
  //Define o tipo de pino para saida
  pinMode(pinoLamp, OUTPUT);
  
  //Define a comunicação serial
  Serial.begin(9600);
  while(!Serial){};
  Serial.println(F("SmartLamp - MQTT Communication"));
  Serial.println();
  
  //Define a rede do Ethernet Shiel usando DHCP
  if (Ethernet.begin(mac)==0){
    Serial.println(F("Habilite a configuração Ethernet via DHCP"));
    for(;;);
  }
  Serial.println(F("Configurando Ethernet via DHCP"));
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
  Serial.println();
  
  //Configuração do IP
  ip = String (Ethernet.localIP()[0]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[1]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[2]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[3]);
  
  //Configurando o Client MQTT
  mqttClient.setClient(ethClient);
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
  
  
  
  Serial.println();
  Serial.println(F("Pronto para enviar dados"));
  sendData();
  mqttClient.publish("oic2018smlamp/ip", ip.c_str());
  mqttClient.subscribe("oic2018smlamp/lamp");
}

void loop()
{
  statusLamp = digitalRead(pinoLamp);
  if (statusLamp == LOW){
    sendData(); 
    }
  if (statusLamp == HIGH){
    sendData();
    }  
  
  mqttClient.loop();
}

void sendData(){
  char msgBuffer[20];
  //Serial.print("Lampada está: ");
  //Serial.println((statusLamp == LOW) ? "OPEN" : "CLOSED");
  //Iniciando conexão com o MQTT
  while (!mqttClient.connected()){
    #ifdef DEBUG
    Serial.println("Conectando ao Broker MQTT...");
    #endif
    if (mqttClient.connect("SmartLamp")){
      #ifdef DEBUG
      Serial.println("Conectado");
      if (statusLamp == "LOW"){
      mqttClient.publish("oic2018smlamp/lamp", (statusLamp == LOW) ? "0" ;
      }
      if (statusLamp == "HIGH"){
        mqttClient.publish("oic2018smlamp/lamp", (statusLamp == HIGH) ? "1";
      }
      if (startsend) {
        mqttClient.publish("oic2018smlamp/lamp", (statusLamp == LOW) ? "0");
        mqttClient.publish("oic2018smlamp/ip", ip.c_str());
        startsend = LOW;
      }
      #endif
    } else {
      #ifdef DEBUG 
      Serial.print("falha estado  ");
      Serial.print(client.state());
      #endif
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char msgBuffer[20];
  Serial.print("Mensagem recebida: [");
  Serial.print(topic);
  Serial.print("] ");//MQTT_BROKER
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println(payload[0]);
  // Verifica o primeiro caracter da mensagem.
  if (payload[0] == 49)             // Valor "1" na tabela ASCII
  {
    digitalWrite(pinoLamp, HIGH);// Torna saida ON
    
  } else if (payload[0] == 48)      // Valor "0" in ASCII
  {
    digitalWrite(pinoLamp, LOW);// Torna saida OFF
  } else {
    Serial.println("Valor Invalido");
    mqttClient.publish("oic2018smlamp/error", "Syntax Error");
  }

}
