class ROM{

    struct User{
      byte *pass;
      char *name;
    };
    
    struct User2{
      byte pass[16];
      char name[16];
    };

    struct usuario{
      char *pass;
      char *name;
    };

    
    // Dirección de primer byte (address 0) de EEPROM
    int address = 0;
    byte value;

    public:
    void search(char user_name[16]){

        EEPROM.begin(512);
        int address_search = 0;   

        String user_name_search = String(user_name);
        
        Serial.print("Buscando: ");
        Serial.println(user_name_search);
        
        while(address_search <= 512){
            
            struct usuario u; 
            EEPROM.get( address_search, u );
            
            char* ptr1 = u.pass;
                        
            printf("Dirección: %d\n", address_search);
            
            printf("Clave: %s\n", ptr1);
            
            char* ptr = u.name;
            printf("Nombre: %s\n", ptr);

            String user_name_search_2 = String(u.name);
            //printf("Nombre string: %s\n", user_name_search_2);

            Serial.print("Nombre string: ");
            Serial.println(user_name_search_2);
            
            if( user_name_search.equals(user_name_search_2)){
                Serial.println( "User found: " );
                return;
            }

            
            ptr = NULL;
            ptr1 = NULL;
            
            // avanzar a siguiente dirección        
            address_search = address_search + sizeof(u); 
            
            if (address_search >= 512) {
                address_search = 0;
                Serial.println( "User not found: " );
                return;
            }
        }
    }

    public:
    void save(char user_name[16], char user_pass[16]){

        EEPROM.begin(512);
        Serial.println( "Guardando usuario: " );
                
        // almacenar objeto leído
        struct usuario u; 
        u.pass = user_pass;
        u.name = user_name;
                
        EEPROM.put( address, u );             
        
        printf("Dirección de guardado: %d\n", address);
                      
        // avanzar a siguiente dirección
        //address = address + 1;
        address = address + sizeof(u); 

        EEPROM.commit();
        EEPROM.end();
        
        if (address >= 512) {
            //address = 0;
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
