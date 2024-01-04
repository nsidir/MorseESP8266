#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "index.h" //Webpage contents
#include "credentials.h"

const char* ssid = ssid; 
const char* password = password; 

ESP8266WebServer server(80);

int buttonPin = D2;
int ledPin = D7; 

int buttonState = 0;    // Variable to store the button state
int lastButtonState = 0; // Variable to store the previous button state
unsigned long lastDebounceTime = 0; // Last time the button state changed
unsigned long debounceDelay = 50; // Debounce delay in milliseconds

void dot() {
  digitalWrite(ledPin, HIGH);
  delay(200);  
  digitalWrite(ledPin, LOW);
  delay(200);  
}

void dash() {
  digitalWrite(ledPin, HIGH);
  delay(600); 
  digitalWrite(ledPin, LOW);
  delay(200);  
}

void sendMorseCode(char character) {
  character = toupper(character);

  switch (character) {
    case 'A':
      dot(); dash(); break;
    case 'B':
      dash(); dot(); dot(); dot(); break;
    case 'C':
      dash(); dot(); dash(); dot(); break;
    case 'D':
      dash(); dot(); dot(); break;
    case 'E':
      dot(); break;
    case 'F':
      dot(); dot(); dash(); dot(); break;
    case 'G':
      dash(); dash(); dot(); break;
    case 'H':
      dot(); dot(); dot(); dot(); break;
    case 'I':
      dot(); dot(); break;
    case 'J':
      dot(); dash(); dash(); dash(); break;
    case 'K':
      dash(); dot(); dash(); break;
    case 'L':
      dot(); dash(); dot(); dot(); break;
    case 'M':
      dash(); dash(); break;
    case 'N':
      dash(); dot(); break;
    case 'O':
      dash(); dash(); dash(); break;
    case 'P':
      dot(); dash(); dash(); dot(); break;
    case 'Q':
      dash(); dash(); dot(); dash(); break;
    case 'R':
      dot(); dash(); dot(); break;
    case 'S':
      dot(); dot(); dot(); break;
    case 'T':
      dash(); break;
    case 'U':
      dot(); dot(); dash(); break;
    case 'V':
      dot(); dot(); dot(); dash(); break;
    case 'W':
      dot(); dash(); dash(); break;
    case 'X':
      dash(); dot(); dot(); dash(); break;
    case 'Y':
      dash(); dot(); dash(); dash(); break;
    case 'Z':
      dash(); dash(); dot(); dot(); break;
    case ' ':
      delay(700);  // Extra delay for word spacing
      break;
    default:
      break;
  }
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleDecode() {
  String message = server.arg("message");
  Serial.println("Received message: " + message);

  for (int i = 0; i < message.length(); i++) {
    sendMorseCode(message[i]);
  }

  server.send(200, "text/plain", "Message received: " + message);
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  WiFi.begin(ssid, password);     
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  server.on("/", handleRoot);

  server.on("/decode", HTTP_GET, handleDecode);

  server.begin();
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();     

  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        // Button pressed, send Morse code and character
        Serial.println("Button Pressed");
      }
    }
  }

  lastButtonState = reading;
  
}