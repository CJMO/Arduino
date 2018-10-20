//#include <SPI.h>
//#include <MFRC522.h>
//#include <EEPROM.h>

#include <stdio.h>
#include <stdlib.h>

#define RST_PIN  D4    //Pin 9 para el reset del RC522
#define SS_PIN  SS   //Pin 10 para el SS (SDA) del RC522

#include "RF.cpp"
#include "ROM.cpp"
#include "Web.h"

FILE *archivo = fopen("D:/Datos/Documents/Arduino/RFID/log.txt", "w");

RF rf_manager = RF();

ROM rom_manager = ROM();

Web web_manager = Web();

String input_string = "";

String admin_key = "9999";


//
char nombre[] = "CJMO";
char clave[] = "CJMO";
char nombre2[] = "C";
char nombre3[] = "C3";

void setup() {

  pinMode(D3, OUTPUT); 
  pinMode(D5, OUTPUT);
 
  rf_manager.initialize(); 
  
  web_manager.setup_web();

  if (archivo == NULL)
  {
      printf("Error opening file!\n");
      //exit(1);
  }

  /* print some text */
  const char *text = "Inicialización completada";
  fprintf(archivo, "%s\n", text);

  fclose(archivo);

}


void loop() {
  
  rf_manager.loop_RF();
  web_manager.loop_web();
  
  boolean admin_mode = 0;
  while (Serial.available() > 0) {
    
    input_string = Serial.readStringUntil('\r\n');
    
    if(input_string.charAt(0) == '*' and input_string.charAt(1) == '#' ){
        admin_mode = 1;
        // eliminar primer(#) y último caracter(fin de línea)
        input_string = input_string.substring(2, input_string.length()-1);        
    }

    // Convert from String Object to String.
    char buf[sizeof(input_string)];
    input_string.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;

    byte newUid[4];
    int i=0;
    
    if( admin_mode == 1 ){
        Serial.println("Modo admin");
        
        //Serial.println(input_string);
        //Serial.println(admin_key);
        
        if(input_string.equals(admin_key)){
          Serial.println("Clave admin correcta"); 
           
       }
       else {
          Serial.println("Clave admin incorrecta");         
       }
    }

          
    while ((str = strtok_r(p, ";", &p)) != NULL){ // delimiter is the semicolon
       
       String t = String(str);
       t = t.substring(0, t.length()-1);
       

       if(t.equals("guardar")){
          Serial.println("guardando");      
          rom_manager.save(nombre, clave); 
          rom_manager.save(nombre2, clave);    
            
       }
       else if(t.equals("buscar")){
          Serial.println("buscando");
          rom_manager.search(nombre2); 
                 
       }

    }
    //rf_manager.printDec(str);   

     
  }
  //rf_manager.writeCard();
}
  
