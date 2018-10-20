#include <Arduino.h>
#include <EEPROM.h>

class ROM {

    struct user {
      char pass[16];
      char name[16];
    };

    int EEPROM_size = 256;
    // Dirección de primer byte (address 0) de EEPROM
    int address = 0;

  public:
    String get_list() {

      String data = "Listado de usuarios creados en el sistema:\n\n";
      data += "[Usuario - Clave de ingreso]\n";

      EEPROM.begin(EEPROM_size);
      int address_search = 0;

      Serial.print("Listando: ");
      
      while (address_search < address) {

        struct user u;
        EEPROM.get( address_search, u );

        printf("Dirección: %d\n", address_search);

        String user_name_search = String(u.name);
        Serial.print("Nombre string: ");
        Serial.println(user_name_search);

        if ( u.name != "") {
          data += u.name;
          data += " - ";
          data += u.pass;
          data += "\n";
        }

        // avanzar a siguiente dirección
        address_search = address_search + sizeof(u); //user_name.length() + user_name.length()

        if (address_search >= EEPROM_size) {
          //address_search = 0;
          return data;
        }
      }
      return data;
    }
 
  public:
    String save(char user_name[16], char user_pass[16]) {

      String data = "";

      EEPROM.begin(EEPROM_size);
      Serial.println( "Guardando usuario: " );

      // almacenar objeto tipo usuario
      struct user u;

      strcpy(u.pass, user_pass);
      strcpy(u.name, user_name);

      EEPROM.put( address, u );

      printf("Dirección de guardado: %d\n", address);
      data = "Usuario '" + String(u.name) + "' guardado correctamente en la posici&oacute;n de memoria " + String(address);

      // avanzar a siguiente dirección
      address = address + sizeof(u);

      EEPROM.commit();
      EEPROM.end();

      return data;

    }

   public:
    String delete_user(char user_name_search[16]) {

      String data = "Eliminar usuarios del sistema:\n\n";
     
      EEPROM.begin(EEPROM_size);
      int address_search = 0;

      Serial.print("Buscando: ");
      
      while (address_search < EEPROM_size) {

        struct user u;
        EEPROM.get( address_search, u );

        printf("Dirección: %d\n", address_search);

        String user_name = String(u.name);
        Serial.print("Nombre: ");
        Serial.println(user_name);

        if ( user_name.equals(String(user_name_search))) {
            Serial.println( "User found: " );

            /*int final_address = address_search + sizeof(u);
            for (int i = address_search ; i <= final_address ; i++) {
              EEPROM.write(i, 0);              
            }*/

            strcpy(u.pass, "");
            strcpy(u.name, "");      
            EEPROM.put( address_search, u );
            
            EEPROM.commit();
            EEPROM.end();
            
            data += "Usuario eliminado";
            return data;
          }

        // avanzar a siguiente dirección
        address_search = address_search + sizeof(u); 

        if (address_search >= EEPROM_size) {
          data += "Usuario no encontrado";
          return data;
    
        }
      }
      return data;
    }

  public:
    void search(char user_name[16]) {

      EEPROM.begin(EEPROM_size);
      int address_search = 0;

      String user_name_search = String(user_name);

      Serial.print("Buscando: ");
      Serial.println(user_name_search);

      if(address == 0){
        Serial.println( "No hay datos guardados" );
        return;
      }

      while (address_search < address) {

        struct user u;
        EEPROM.get( address_search, u );

        char* ptr1 = u.pass;

        if( ptr1 != "" ){
          Serial.println(ptr1);

          printf("Dirección: %d\n", address_search);

          printf("Clave: %s\n", ptr1);
  
          char* ptr = u.name;
          printf("Nombre: %s\n", ptr);
  
          String user_name_search_2 = String(u.name);
          //printf("Nombre string: %s\n", user_name_search_2);
  
          Serial.print("Nombre string: ");
          Serial.println(user_name_search_2);
  
          if ( user_name_search.equals(user_name_search_2)) {
            Serial.println( "User found: " );
            return;
          }

          ptr = NULL;
          ptr1 = NULL;

          
        }


        // avanzar a siguiente dirección
        address_search = address_search + sizeof(u);

        if (address_search >= EEPROM_size) {
          //address_search = 0;
          Serial.println( "User not found: " );
          return;
        }
      }
    }

  public:
    void clear_data() {
      EEPROM.begin(EEPROM_size);
      for (int i = 0 ; i <= EEPROM_size ; i++) {
        EEPROM.write(i, 0);
        //EEPROM.put( i, 0 );
      }
      EEPROM.commit();
      EEPROM.end();

      address = 0;

    } 
  
};
