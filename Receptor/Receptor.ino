#include "heltec.h"
#define contato 0
#define BAND 915E6
char st;

char txt;
int estado = 1;
// textos
String menssagem;
//controle de sinal
unsigned long  utimoEnvio = 0;
unsigned long tempo = 0;
int sinal = 0 ;
//controle de perca de sinal
unsigned long anterior = 0;
unsigned long intervalo = 500 ;
unsigned int liga = 1;
/// Sinal de poblema
int erro = 0;
unsigned long atual;
void setup () {
  pinMode(25, OUTPUT);
  digitalWrite(25,HIGH);
  Heltec.begin(true, true, true, true, BAND);
  
  estado = 1;


  tempo = millis();

  Heltec.display->setContrast(255);
  Heltec.display->clear();

  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(8, 0, "Bem Vindo ");
  Heltec.display->display();

  for (int i = 0; i <= 100; i++) {
    Heltec.display->drawProgressBar(8, 30, 110, 5, i);
    Heltec.display->display();
    delay(10);
  }
  Heltec.display->clear();

  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(8, 0, "Aguardando");
  Heltec.display->display();

  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(15, 20, "Sinal...");
  Heltec.display->display();
  estado = 1;

}
void loop () {
  //Recebendo

  
  tempo = millis() - utimoEnvio;
  Serial.println("Tempo No Loop:");
  Serial.println(  tempo);
 // Serial.print("erros : " + erro);

   if ((tempo > 5000) && (tempo < 10000)) {
    
    Serial.println( "eestado : " + estado);
    goto enviar;
   
  } 
    if (tempo > 10000) {
    
    Heltec.display->clear();

    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 0, "   Sem sinal");

     Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 25, "  !!!!");


    Heltec.display->display();

    utimoEnvio = millis();
    erro++;
    sinal = 0;
    txt = '2';
    tempo = 0;
    estado = 1;
    
  } else 


  if (estado == 0) {
    enviar:
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      //tempo = millis() - utimoEnvio;
      Serial.println("Pacote recebido '");

      //Aguardando o pacote
      while (LoRa.available ()) {
        st = (char)LoRa.read ();
        Serial.println(st);
        sinal = LoRa.packetRssi();
        Serial.println(sinal);
        /////reset de tempo
        utimoEnvio = millis();
        //tempo = millis() - utimoEnvio;
        //////////////////////condicao///////////////////////
      }
       
      if (tempo < 5000) {
        Serial.println(tempo);
        txt = st;
      } else if (tempo > 5000) {
        txt = '3';
        sinal = 0;
      }

      if (txt == '0') {
        menssagem = "Ligado";
      } else if (txt == '1') {
        menssagem = "Desligado";
      } else {
        menssagem = "Desconhecido";
      }

    

    /////////////////////////////////////Parte do visor //////////////////////////

    Heltec.display->clear();

    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 0, "   Sinal : " + String(sinal) + "dB");

    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 24, "   Erros:" + String(erro) );


    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 35, "   Menssagem: " + menssagem);

    Heltec.display->display();
    
    estado = 1;
    }
  } else  if (estado == 1) {
    
    LoRa.beginPacket();
    LoRa.print("0");
    LoRa.endPacket();
    
    estado = 0;
  }




    if ((txt == '0') && (tempo <= 20000)) {
      digitalWrite(25, HIGH);
    } else {
      digitalWrite(25, LOW);
    }
  }
