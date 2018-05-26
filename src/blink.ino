//Autores: 
//Erik Sardinha
//Henrique Velardo
//John Lennon
//Marcos Felipe

//Data inicio: 01/05/2018
//Data final: 26/05/2018

//Projeto adaptado de projetos encontrados na internet como base
//Uso do css e scripts do site filiflop

//Importando biblioteca
#include <SPI.h>   
#include <Ethernet.h>

//Definindo variaveis
String readString;
boolean ligado = true;

//Definindo pinos
#define Lamp 8

//Definindo rede
byte mac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[]={172,16,18,20};
byte gateway[]={172,16,18,1};
byte subnet[]={255,255,254,0};

//Configuração porta para o Ethernet Shield
EthernetServer server(80);

void setup() {
  //Define a rede do Ethernet Shiel
  Ethernet.begin(mac, ip, gateway, subnet);
  
  //Inicia o servidor pelo Ethernet Shield
  server.begin();
  
  //Define o tipo de pino para saida
  pinMode(Lamp, OUTPUT);

  //Inicia o rele desligado
  digitalWrite(Lamp, HIGH);
  
}

void loop()
{
  //Inicia cliente
  EthernetClient client = server.available();
  //Verifica se há conexão
  if (client) {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == 'n')
        {
          //Controle do rele
          Serial.println(readString);
          //Liga o Rele
          if (readString.indexOf("?ligar") > 0)
          {
            digitalWrite(Lamp, LOW);
            Serial.println("Lampada Ligada");
            ligado = false;
          }
          else
          {
            //Desliga o Rele
            if (readString.indexOf("?desligar") > 0)
            {
              digitalWrite(Lamp, HIGH);
              Serial.println("Lampada  Desligado");
              ligado = true;
            }
          }
          readString = "";
          //Layout/ Interface do servidor Ethernet
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          client.println("<head>");
          client.println("<title>FILIPEFLOP - Automacao Residencial</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
          client.println("<meta name='viewport' content='width=720, initial-scale=0.5' />");
          client.println("<link rel='stylesheet' type='text/css' href='http://img.filipeflop.com/files/download/automacao/automacao_residencial.css' />");
          client.println("<script type='text/javascript' src='http://img.filipeflop.com/files/download/automacao/automacao_residencial.js'></script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<div id='wrapper'><img alt='FILIPEFLOP' src='http://img.filipeflop.com/files/download/automacao/logoFF.jpg'/><br/>");
          client.println("<div id='div1'>Rele 1</div>");
          client.print("<div id='rele'></div><div id='estado' style='visibility: hidden;'>");
          client.print(ligado);
          client.println("</div>");
          client.println(
            "<div id='botao'></div>");
          client.println("</div>");
          client.println("</div>");
          client.println("<script>AlteraRele1()</script>");
          client.println("</div>");
          client.println("</body>");
          client.println("</head>");
          delay(1);
          client.stop();
        }
      }
    }
  }
}
