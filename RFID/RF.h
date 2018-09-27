class RF{

    #define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}
      
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
      Serial.begin(9600);
      SPI.begin(); // Init SPI bus
      rfid.PCD_Init(); // Init MFRC522 

      for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
      }

      Serial.println(F("Inicialización finalizada"));
      
      /*Serial.println(F("Clave"));
      printHex(key.keyByte, MFRC522::MF_KEY_SIZE);*/
    }

    byte ActualUID[4]; //almacenará el código del Tag leído
    byte Usuario2[4]= {0xB7, 0xAE, 0xC7, 0x73} ; //código del usuario 1
    byte Usuario1[4]= {183, 174, 199, 115} ; //código del usuario 2

    public:
    void loop_RF() {
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
            Serial.println("Acceso concedido...");
            else
            Serial.println("Acceso denegado...");
            
            
            // Store NUID into nuidPICC array
            for (byte i = 0; i < 4; i++) {
              ActualUID[i] = rfid.uid.uidByte[i];
            }
           
            Serial.println(F("Número de tarjeta:"));            
            printDec(rfid.uid.uidByte, rfid.uid.size);
            Serial.println();
            
            // Terminamos la lectura de la tarjeta tarjeta  actual
            rfid.PICC_HaltA();
            
            // Stop encryption on PCD
            rfid.PCD_StopCrypto1();
      
        }
          
      }
      
    }

    public:
    void writeCard(/*byte NEW_UID*/){

      // Look for new cards, and select one if present
      if ( ! rfid.PICC_IsNewCardPresent() || ! rfid.PICC_ReadCardSerial() ) {
        //delay(50);
        return;
      }
      else{
        // Set new card ID
        byte newUid[] = NEW_UID;
        if ( rfid.MIFARE_SetUid(newUid, (byte)4, true) ) {
          Serial.println(F("Asignado nuevo UID a la tarjeta."));
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
    void printDec(byte *buffer, byte bufferSize) {
      for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
      }
    }

};
