
#include "heltec.h"
#define BAND 915E6
#define contato 17
//Autor Bernardo
char st;
int contador = 0;
int estado = 0;
String menssagem = "";
unsigned long  utimoEnvio = 0;
unsigned long tempo = 0;
void setup () {
  //Iniciando a biblioteca Heltec
  Heltec.begin(true, true, true, true, BAND);
  pinMode(contato,INPUT);
  estado = 0;
  tempo = millis();
    //Apenas escrevendo na tela de Oled
    Heltec.display->setContrast(255);
    Heltec.display->clear();

    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(8, 0, "Bem Vindo ");
    Heltec.display->display();
    //Animacao do display para o carregamento 
     for(int i = 0; i <= 100;i++){
     Heltec.display->drawProgressBar(8,30,110,5,i);
     Heltec.display->display();
     delay(15);
     }
    Heltec.display->clear();

    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(8, 0, "Aguardando");
    Heltec.display->display();

    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(15, 20, "Sinal...");
    Heltec.display->display();
     
     
    
    
  
}

void loop () {
 if(tempo <= 20000){  
   //Serial.println("volteiii");
    tempo = millis()-utimoEnvio;
   // Serial.println(tempo);
      
    
 if (estado == 0) { 
  if (digitalRead(contato) == 0) {
    LoRa.beginPacket();
    LoRa.print("0");
    LoRa.endPacket();
    utimoEnvio = millis();
    menssagem = "Ligado";
   
   
    estado = 1;
    
   
  } else {
    LoRa.beginPacket();
    LoRa.print("1");
    LoRa.endPacket();
    utimoEnvio = millis();
     menssagem = "Desligado";
    
    estado = 1;
    
    
  }
} else if (estado == 1 ){
    int packetSize = LoRa.parsePacket();
    
  if (packetSize) {
    Serial.println("Pacote enviado com sucesso'");
    while (LoRa.available ()) {
      st = (char)LoRa.read ();
      Serial.println(st); 
      int sinal = LoRa.packetRssi();
      Serial.println(sinal);
   
      tempo = millis()-utimoEnvio;
/////////////////////////////////////Parte do visor //////////////////////////
    Heltec.display->clear();

    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 0, "   Sinal : " + String(sinal)+"dB");
    
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 24, "   Enviado com sucesso");
     Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 35, " Menssagem: " + menssagem);

    Heltec.display->display();
    ////////////////////////////////Variaveis Booleanas
      estado = 0; 
      contador = 1;   
    }
  } 
   
} 

  

} else {
     Heltec.display->clear();

    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(0, 0, "  Sem Sinal");
    
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(0, 25, "        !!!");
    delay(1000);
    Heltec.display->display();
  int packetSize = LoRa.parsePacket();
    
  if (packetSize) {
    Serial.println("Pacote enviado com sucesso'");
    while (LoRa.available ()) {
      st = (char)LoRa.read ();
      Serial.println(st); 
      int sinal = LoRa.packetRssi();
      Serial.println(sinal);
      utimoEnvio = millis();
 
      tempo = 0;
      estado = 0;
       
    }
  } 
}
}

   
