#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  D4    //Pin 9 para el reset del RC522
#define SS_PIN  SS   //Pin 10 para el SS (SDA) del RC522

MFRC522 rfid(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
MFRC522::MIFARE_Key key;

#include "RF.h"

RF rf_manager = RF();

String texto_lectura = "";


///////////////////////
int block=6;//this is the block number we will write into and then read. Do not write into 'sector trailer' block, since this can make the block unusable.

//byte blockcontent[16] = {"000000000000001"};//an array with 16 bytes to be written into one of the 64 card blocks is defined
//byte blockcontent[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};//all zeros. This can be used to delete a block.
byte readbackblock[18];//This array is used for reading out a block. The MIFARE_Read method requires a buffer that is at least 18 bytes to hold the 16 bytes of a block.

byte blockcontent[]    = {
        0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
        0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
        0x08, 0x09, 0xff, 0x0b, //  9, 10, 255, 12,
        0x0c, 0x0d, 0x0e, 0x0f  // 13, 14,  15, 16
    };

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
    //rf_manager.printDec(str);   

    int respuesta = rf_manager.writeBlock(block, blockcontent);//the blockcontent array is written into the card block
    Serial.println(respuesta, DEC);
     
  }
  //rf_manager.writeCard();
}
  
