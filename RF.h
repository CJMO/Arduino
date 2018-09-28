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
      
      Serial.println(F("Clave"));
      printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
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

    /*public:
    void writeCard(int address, byte value[]){

      // Look for new cards, and select one if present
      if ( ! rfid.PICC_IsNewCardPresent() || ! rfid.PICC_ReadCardSerial() ) {
        //delay(50);
        return;
      }
      else{
        // Set new card ID
        byte newUid[] = value;
        if ( rfid.MIFARE_SetUid(newUid, (byte)4, true) ) {
          Serial.println(F("Asignado nuevo UID a la tarjeta."));
        }
      }
      
    }*/

    public:
    int writeBlock(int blockNumber, byte arrayAddress[]) {
      //this makes sure that we only write into data blocks. Every 4th block is a trailer block for the access/security info.
      int largestModulo4Number=blockNumber/4*4;
      int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector
      if (blockNumber > 2 && (blockNumber+1)%4 == 0){Serial.print(blockNumber);Serial.println(" is a trailer block:");return 2;}//block number is a trailer block (modulo 4); quit and send error code 2
      Serial.print(blockNumber);
      Serial.println(" is a data block:");
      
      /*****************************************authentication of the desired block for access***********************************************************/
      byte status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(rfid.uid));
      /*
      //byte PCD_Authenticate(byte command, byte blockAddr, MIFARE_Key *key, Uid *uid);
      //this method is used to authenticate a certain block for writing or reading
      //command: See enumerations above -> PICC_CMD_MF_AUTH_KEY_A  = 0x60 (=1100000),    // this command performs authentication with Key A
      //blockAddr is the number of the block from 0 to 15.
      //MIFARE_Key *key is a pointer to the MIFARE_Key struct defined above, this struct needs to be defined for each block. New cards have all A/B= FF FF FF FF FF FF
      //Uid *uid is a pointer to the UID struct that contains the user ID of the card.
      if (status != MFRC522::STATUS_OK) {
             Serial.print("PCD_Authenticate() failed: ");
             //Serial.println(rfid.GetStatusCodeName(status));
             return 3;//return "3" as error message
      }
      //it appears the authentication needs to be made before every block read/write within a specific sector.
      //If a different sector is being authenticated access to the previous one is lost.
      */
      /*****************************************writing the block***********************************************************/

      // Show the whole sector as it currently is
      Serial.println(F("Current data in sector:"));
      rfid.PICC_DumpMifareClassicSectorToSerial(&(rfid.uid), &key, blockNumber);
      Serial.println();
    
      status = rfid.MIFARE_Write(blockNumber, arrayAddress, 16);//valueBlockA is the block number, MIFARE_Write(block number (0-15), byte array containing 16 values, number of bytes in block (=16))
      //status = mfrc522.MIFARE_Write(9, value1Block, 16);
      if (status != MFRC522::STATUS_OK) {
               Serial.print("MIFARE_Write() failed: ");
               //Serial.println(rfid.GetStatusCodeName(status));
               return 4;//return "4" as error message
      }
      Serial.println("block was written");
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
