#include "Web.h"
#include "ROM.cpp"

// Datos de conexión a Wifi
const char* ssid = "ID_RED";
const char* password = "CLAVE_RED";

// Instancia de servidor web
ESP8266WebServer server(80);

String _admin_user = "admin99";
String _admin_key = "9999";

String admin = "";
String pass = "";

ROM _rom_manager = ROM();

void handleRoot() {
  server.send(200, "text/plain", "Bienvenido al portal administrativo de control de acceso!");
}

void handleNotFound() {
  String message = "";
  /*message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }*/

  if(server.argName(0).equals("admin")){
    admin = server.arg(0);
    
    if(server.argName(1).equals("pass")){
      pass = server.arg(1);
    }
  }

  Serial.print("Admin: ");
  Serial.println(admin);
  Serial.print("Pass: ");
  Serial.println(pass);

  // Verificar si se ingresa a modo administrador
  if( admin.equals(_admin_user) and pass.equals(_admin_key) ){
    Serial.println("Modo admin activado: ");

    // Gestionar operaciones sobre usuarios
    if(server.argName(2).equals("opcion")){

      String data = "";

      char nombre[16];
      char clave[16];

      //strcpy(nombre, "CJMO");
      //strcpy(clave, "CJMO");
            
      // Listar usuarios
      if(server.arg(2).equals("listar")){        
        data = _rom_manager.get_list();
        //_rom_manager.search(nombre);
        message += data;
      }
      // Guardar usuario
      else if (server.arg(2).equals("guardar")){
        // Obtener datos de usuario
        String user_name = "";
        String user_pass = "";
        if(server.argName(3).equals("nombre")){        
          user_name = server.arg(3);          
        }
        if(server.argName(4).equals("clave")){        
          user_pass = server.arg(4);          
        }

        char buf1[16];
        user_name.toCharArray(buf1, sizeof(buf1));
        char buf2[16];
        user_pass.toCharArray(buf2, sizeof(buf2));

        // Copiar datos de buffers a parámetros
        strcpy(nombre, buf1);
        strcpy(clave, buf2);

        data = _rom_manager.save(nombre, clave); 
        message += data;
      }
      // Limpiar memoria
      else if (server.arg(2).equals("limpiar")){
        
        _rom_manager.clear_data();
        
        data = "Datos borrados"; 
        message += data;
      }

      // Eliminar usuario
      else if (server.arg(2).equals("eliminar")){

        // Obtener datos de usuario
        String user_name = "";
        if(server.argName(3).equals("nombre")){        
          user_name = server.arg(3);          
        }
        
        char buf1[16];
        user_name.toCharArray(buf1, sizeof(buf1));
 
        // Copiar datos de buffers a parámetros
        strcpy(nombre, buf1);
        
        data = _rom_manager.delete_user(nombre);       
        message += data;
      }
     
    }
  }
  else{
    Serial.println("Modo admin no activado: ");
  }
  
  server.send(404, "text/plain", message);
}

void Web::setup_web(){
    Serial.begin(9600);
    Serial.println("Inicializando web: ");

    Serial.print("Conectando a ");
    Serial.println(ssid);
    
    WiFi.mode(WIFI_STA);
    WiFi.hostname("wemos");
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi conectado");

    server.on("/", handleRoot);

    server.on("/inline", []() {
      server.send(200, "text/plain", "this works as well");
    });

  server.onNotFound(handleNotFound);
  
    // Start the server
    server.begin();
    Serial.println("Servidor iniciado");
  
    // Print the IP address
    Serial.println(WiFi.localIP());
}

void Web::loop_web(){

   server.handleClient();  

}
