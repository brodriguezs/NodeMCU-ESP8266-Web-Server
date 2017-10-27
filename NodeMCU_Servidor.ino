/*
 * API Control NodeMCU con módulo ESP8266 Wifi
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h> 

///Configuración red
const char *ssid = "***********";
const char *password = "*******";

const int gradosParaSubir = -400;
const int gradosParaBajar = 360;

/*
 * Definición de Pines
 */
 
const int pin_servo = 4;
const int ledUno = 16;
const int ledDos = 5;
/*
 * Step by Step Motor
 */
const int pinInUno = 0; 
const int pinInDos = 2;  
const int pinInTres = 14; 
const int pinInCuatro = 12; 

const int retardo = 5;

/*
 * Instancia del Servidor 
 */
ESP8266WebServer server ( 80 );

/*
 * Variables Globales
 */ 
boolean ledUnoOn = false;
boolean ledDosOn = false;
int gradosPrevios = 0;
int posRejilla = 0;
Servo myservo;

/////Funciones
void subirRejilla(){
  int pos;
  
  myservo.attach(pin_servo);
  
  for(pos = posRejilla; pos <= 90; pos++) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  posRejilla = pos;
  
  myservo.detach();
  server.send ( 200, "text/html", "Rejilla arriba" );
}

void bajarRejilla(){
  int pos;
  
  myservo.attach(pin_servo);

  for(pos = posRejilla; pos>=0; pos--)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  
  posRejilla = pos;
  
  myservo.detach();
  server.send ( 200, "text/html", "Rejilla abajo" );
  
}
/*
 * Function to togle Led 1
 */

void controlLuzUno(){

  if( ledUnoOn == false ){
    ledUnoOn = true;
    Serial.println ("encendió led");
    digitalWrite ( ledUno, 1 );
    
  } else {
    ledUnoOn = false;
    Serial.println ("apagó led");
    digitalWrite ( ledUno, 0 );
  }
  server.send ( 200, "text/html", "Luz ok" );
}
/*
 * Function to togle Led 2
 */
void controlLuzDos(){
  
  if( ledDosOn == false ){
    ledDosOn = true;
    Serial.println ("encendió led");
    digitalWrite ( ledDos, 1 );
    
  } else {
    ledDosOn = false;
    Serial.println ("apagó led");
    digitalWrite ( ledDos, 0 );
  }
  server.send ( 200, "text/html", "Luz ok" );
}
/*
 * Function to make the motor step by step move to right
 */
void pasoDer(int i){         
 for(int j; j<i; j++){
  digitalWrite(pinInCuatro, LOW); 
  digitalWrite(pinInTres, LOW);  
  digitalWrite(pinInDos, HIGH);  
  digitalWrite(pinInUno, HIGH);  
  delay(retardo); 
  digitalWrite(pinInCuatro, LOW); 
  digitalWrite(pinInTres, HIGH);  
  digitalWrite(pinInDos, HIGH);  
  digitalWrite(pinInUno, LOW);  
  delay(retardo); 
  digitalWrite(pinInCuatro, HIGH); 
  digitalWrite(pinInTres, HIGH);  
  digitalWrite(pinInDos, LOW);  
  digitalWrite(pinInUno, LOW);  
  delay(retardo); 
  digitalWrite(pinInCuatro, HIGH); 
  digitalWrite(pinInTres, LOW);  
  digitalWrite(pinInDos, LOW);  
  digitalWrite(pinInUno, HIGH);  
  delay(retardo);  
 }
}
/*
 * Function to make the motor step by step move to left
 */
void pasoIzq(int i) {        
  for(int j; j<i; j++){
    digitalWrite(pinInCuatro, HIGH); 
    digitalWrite(pinInTres, HIGH);  
    digitalWrite(pinInDos, LOW);  
    digitalWrite(pinInUno, LOW);  
    delay(retardo); 
    digitalWrite(pinInCuatro, LOW); 
    digitalWrite(pinInTres, HIGH);  
    digitalWrite(pinInDos, HIGH);  
    digitalWrite(pinInUno, LOW);  
    delay(retardo); 
    digitalWrite(pinInCuatro, LOW); 
    digitalWrite(pinInTres, LOW);  
    digitalWrite(pinInDos, HIGH);  
    digitalWrite(pinInUno, HIGH);  
    delay(retardo); 
    digitalWrite(pinInCuatro, HIGH); 
    digitalWrite(pinInTres, LOW);  
    digitalWrite(pinInDos, LOW);  
    digitalWrite(pinInUno, HIGH);  
    delay(retardo); 
  }
}

/*
 * Function to put off the motor
 */       
void apagarMotorPaso() {         
 digitalWrite(pinInCuatro, LOW); 
 digitalWrite(pinInTres, LOW);  
 digitalWrite(pinInDos, LOW);  
 digitalWrite(pinInUno, LOW);  
}

/*
 * Function to raise the step by step motor (curtain)
 */
void subirCortina(){
  controlCortina(gradosParaSubir);
}

/*
 * Function to lower the step by step motor (curtain)
 */
void bajarCortina(){
  controlCortina(gradosParaBajar);
}

/*
 * Function to control the step by step motor
 */
void controlCortina(int grados){
  Serial.println("control Cortina");
  Serial.println(grados);
  if(grados > 0){
    Serial.println("grados positivos");
    pasoDer(grados - gradosPrevios);
  } else {
    Serial.println("grados negativos");
    pasoIzq(grados + gradosPrevios);
  }
  // remember the previous value
  gradosPrevios = grados;
  
  apagarMotorPaso();
  server.send ( 200, "text/html", "Cortina ok" );
}
/*
 * Function that handles the index page
 */
void handleRoot() {
	char temp[400];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	snprintf ( temp, 400,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>Control Wifi usando NODEMCU con ESP8266</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>IoT Rocks!!!!!!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

		hr, min % 60, sec % 60
	);
	server.send ( 200, "text/html", temp );
}
/*
 * Function that handles when an URL that not exists is requested
 */
void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );
}
/*
 * Setup Arduino NodeMCU function
 */
void setup ( void ) {

	Serial.begin ( 115200 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

  pinMode(ledUno, OUTPUT);
  pinMode(ledDos, OUTPUT);
  pinMode(pinInUno, OUTPUT);
  pinMode(pinInDos, OUTPUT);
  pinMode(pinInTres, OUTPUT);
  pinMode(pinInCuatro, OUTPUT);

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

	if ( MDNS.begin ( "esp8266" ) ) {
		Serial.println ( "MDNS responder started" );
	}

  /*
   * API Endpoints 
   */

	server.on ( "/", handleRoot );
  server.on ( "/rejilla/subir", subirRejilla );
  server.on ( "/rejilla/bajar", bajarRejilla );
  server.on ( "/cortina/subir", subirCortina );
  server.on ( "/cortina/bajar", bajarCortina );
  server.on ( "/luz/1", controlLuzUno );
  server.on ( "/luz/2", controlLuzDos );

	server.on ( "/test.svg", drawGraph );
	server.on ( "/inline", []() {
		server.send ( 200, "text/plain", "this works as well" );
	} );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
}
/*
 * Function Loop like Arduino used in the ESP8266 to run the main server client 
 */
void loop ( void ) {
	server.handleClient();
}
/*
 * Function that graph nothing special :p 
 */
void drawGraph() {
	String out = "";
	char temp[100];
	out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
 	out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
 	out += "<g stroke=\"black\">\n";
 	int y = rand() % 130;
 	for (int x = 10; x < 390; x+= 10) {
 		int y2 = rand() % 130;
 		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
 		out += temp;
 		y = y2;
 	}
	out += "</g>\n</svg>\n";

	server.send ( 200, "image/svg+xml", out);
}
