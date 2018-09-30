class ROM{

    struct User{
      byte *pass;
      char *name;
    };
    
    struct User2{
      byte pass[16];
      char name[16];
    };
    
    //EEPROM.begin(512);

    // Dirección de primer byte (address 0) de EEPROM
    int address = 0;
    byte value;

    public:
    void search(char user_name[16]){
        
        /*address = 0;
        
        Serial.println( "Leyendo usuario: " );
        
        // Buscar dirección en EEPROM
        while(address <= 512){
            value = EEPROM.read(address);
            Serial.print(address);
            Serial.print("\t");
            Serial.print(value, DEC);
            Serial.println();
            
            address = address + 1;
        }*/
        
        
        address = 0;        
                
        // almacenar objeto leído
        //User u;              
        
        //printf("Tamaño: %d\n", sizeof(u));
        
        while(address <= 512){
            
            struct User2 u; 
            EEPROM.get( address, u );
            
            byte* ptr1 = u.pass;
            //printHex( u.pass, 18 );
            
            printf("Dirección: %d\n", address);
            
            printf("Clave: %s\n", ptr1);
            
            char* ptr = u.name;
            printf("Nombre: %s\n", ptr);
            
            if( u.name == user_name){
                Serial.println( "User found: " );
            }

            // avanzar a siguiente dirección        
            address = address + 32; //sizeof(u); 
            
            if (address >= 512) {
                address = 0;
                Serial.println( "User not found: " );
                return;
            }
        }
    }

    public:
    void save(byte user_pass[16], char user_name[10]){
        
        address = 0;
        
        Serial.println( "Guardando usuario: " );
                
        // almacenar objeto leído
        /*User u; 
        //User2 u;
        
        u.pass = user_pass;
        u.name = user_name;
        
        byte content[] = {
            0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
            0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
            0x08, 0x09, 0xff, 0x0b, //  9, 10, 255, 12,
            0x0c, 0x0d, 0x0e, 0x0f  // 13, 14,  15, 16
        };*/

        struct User2 u2 = {
            {
            0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
            0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
            0x08, 0x09, 0xff, 0x0b, //  9, 10, 255, 12,
            0x0c, 0x0d, 0x0e, 0x0f  // 13, 14,  15, 16
        },
            "CJMO"
        };
        EEPROM.put( address, u2 );     
        EEPROM.commit();
        EEPROM.end();
        
        printf("Dirección: %d\n", address);
                      
        // avanzar a siguiente dirección
        //address = address + 1;
        address = address + sizeof(u2); 
        
        if (address >= 512) {
            address = 0;
            return;
        }
    }
    
    private:
    void printHex(byte *buffer, int bufferSize) {
      for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
      }
    }
};