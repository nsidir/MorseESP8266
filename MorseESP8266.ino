#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include "index.h" //Webpage contents
#include "credentials.h"

const char* ssid = ssidHiddden.c_str();
const char* password = passwordHidden.c_str();

ESP8266WebServer server(80);

WiFiClient client;

String morseWord;

int buttonPin = D1;
int ledPin = D7; 

int buttonState = 0;     // current state of the button
int lastButtonState = 0; // previous state of the button
int startPressed = 0;    // the moment the button was pressed
int endPressed = 0;      // the moment the button was released
int holdTime = 0;        // how long the button was hold
int idleTime = 0;        // how long the button was idle

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

void handleReceive() {
  server.send(200, "text/plain", "Morse Word is: " + morseWord);
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

  server.on("/receive", HTTP_POST, handleReceive);
  
  server.begin();
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();     

  buttonState = digitalRead(buttonPin); // read the button input

  if (buttonState != lastButtonState) { // button state changed
    updateState(morseWord);
  }

  lastButtonState = buttonState;        // save state for next loop

  if (holdTime >= 3000) {
    //Serial.print("Sent Word");
    client.print(String("POST ") + "/receive HTTP/1.1\r\nHost: 192.168.1.8\r\nUser-Agent: BuildFailureDetectorESP8266\r\nConnection: close\r\n\r\n");
    //Serial.print("Sent the Morse Code");
    morseWord = "";
    //server.send(200, "text/plain", "Morse Word is: " + morseWord);
  }
}

void updateState(String& morseWord) {
  // the button has been just pressed
  if (buttonState == HIGH) {
      startPressed = millis();
      idleTime = startPressed - endPressed;

      if (idleTime >= 1200 && idleTime < 3000) {
          Serial.print(" "); 
          morseWord += ' ';
      }

      

  // the button has been just released
  } else {
      endPressed = millis();
      holdTime = endPressed - startPressed;

      if (holdTime > 20 && holdTime < 300) {
          Serial.print("."); 
          morseWord += '.';
      }

      if (holdTime >= 300 && holdTime < 3000) {
          Serial.print("-"); 
          morseWord += '-';
      }

  }
}
