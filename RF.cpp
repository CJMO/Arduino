#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

class RF{
      
    //MFRC522 rfid(SS_PIN, RST_PIN);
    //MFRC522 rfid(SS, D4); ///Creamos el objeto para el RC522
    //MFRC522::MIFARE_Key key;

    //Función para comparar dos vectores
    boolean compareArray(byte array1[],byte array2[]){
      if(array1[0] != array2[0])return(false);
      if(array1[1] != array2[1])return(false);
      if(array1[2] != array2[2])return(false);
      if(array1[3] != array2[3])return(false);
      return(true);
    }
    
    public:
    void initialize() {

     //MFRC522 rfid(SS_PIN, RST_PIN);
     MFRC522 rfid(SS, D4); ///Creamos el objeto para el RC522
     MFRC522::MIFARE_Key key;
      
      Serial.begin(9600);
      SPI.begin(); // Init SPI bus
      rfid.PCD_Init(); // Init MFRC522 

      for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
      }
      
      Serial.println(F("Inicialización finalizada"));
      
      Serial.println(F("Clave"));
      printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
    }

    byte ActualUID[4]; //almacenará el código del Tag leído
    byte Usuario2[4]= {0xB7, 0xAE, 0xC7, 0x73} ; //código del usuario 1
    byte Usuario1[4]= {183, 174, 199, 115} ; //código del usuario 2
    
    byte clave1 = 183;
    byte clave2 = 174;
    byte clave3 = 199;
    byte clave4 = 115;
    
    byte clave[4] = {clave1, clave2, clave3, clave4};
    
    public:
    void loop_RF() {

       MFRC522 rfid(SS, D4); ///Creamos el objeto para el RC522
       MFRC522::MIFARE_Key key;
     
       digitalWrite(D3, LOW);
       digitalWrite(D5, LOW);
        
      // Revisamos si hay nuevas tarjetas  presentes
      if ( rfid.PICC_IsNewCardPresent()) {  
        //Seleccionamos una tarjeta
        if ( rfid.PICC_ReadCardSerial()) {            
            
            Serial.print(F("Tipo de tarjeta: "));
            MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
            Serial.println(rfid.PICC_GetTypeName(piccType));

            // Check is the PICC of Classic MIFARE type
            if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
            piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
            piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
            Serial.println(F("Tarjeta no es de tipo MIFARE clásico."));
            return;
            }

            //comparamos los UID para determinar si es uno de nuestros usuarios  
            if(compareArray(rfid.uid.uidByte,Usuario1))
            {
                Serial.println("Acceso concedido...");
                digitalWrite(D3, HIGH);  
                delay(3000);
                //delay_led(3);
                digitalWrite(D3, LOW);
            }            
            else
            {
                Serial.println("Acceso denegado...");
                //digitalWrite(D5, HIGH);
                //digitalWrite(D3, LOW);
                //delay(3000);
                //digitalWrite(D5, HIGH);
            }
                        
            // Store NUID into nuidPICC array
            for (byte i = 0; i < 4; i++) {
              ActualUID[i] = rfid.uid.uidByte[i];
            }
           
            Serial.println(F("Número de tarjeta:"));            
            printDec(rfid.uid.uidByte, rfid.uid.size);           
            Serial.println();
            printHex(rfid.uid.uidByte, rfid.uid.size);
            
            // Terminamos la lectura de la tarjeta tarjeta  actual
            rfid.PICC_HaltA();
            
            // Stop encryption on PCD
            rfid.PCD_StopCrypto1();           
                         
      
        }
          
      }
      
    }
   
    /**
     * Helper routine to dump a byte array as hex values to Serial. 
     */
    private:
    void printHex(byte *buffer, byte bufferSize) {
      for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
      }
    }

    /**
     * Helper routine to dump a byte array as dec values to Serial.
     */
     public:
    void printDec(byte *buffer, byte bufferSize) {
      for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
      }
    }

};
