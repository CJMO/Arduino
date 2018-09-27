#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  D4    //Pin 9 para el reset del RC522
#define SS_PIN  SS   //Pin 10 para el SS (SDA) del RC522

MFRC522 rfid(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
MFRC522::MIFARE_Key key;

#include "RF.h"

RF rf_manager = RF();

String texto_lectura = "";

void setup() {

  rf_manager.initialize();

}


void loop() {
  
  rf_manager.loop_RF();

  while (Serial.available() > 0) {

    texto_lectura = Serial.readStringUntil('\r\n');

    // Convert from String Object to String.
    char buf[sizeof(texto_lectura)];
    texto_lectura.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;

    byte newUid[4];
    int i=0;
    
    while ((str = strtok_r(p, ";", &p)) != NULL){ // delimiter is the semicolon
       Serial.println(str);

       /*if(i<4){
        newUid[i] = (BYTE*)str;
       }*/

       //int numero = atoi(str);
      //Serial.println(numero, DEC);

    }
    RF.printDec(str);    
     
  }
  //rf_manager.writeCard();
}
  
