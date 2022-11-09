/*
  Arduino MQTT DEMO
*/

#include <ArduinoMqttClient.h>
#include <WiFiNINA.h> // for MKR1000 change to: #include <WiFi101.h>
//#include <WiFi101.h>


#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate
//    flashed in the WiFi module.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
const int controlpin1 = 2;
const int controlpin2 = 3; 
const int controlpin3 = 4;
//const char broker[] = "192.168.1.142"; // RPI2 HW Address of the MQTT server

//const char broker[] = "192.168.1.180"; // RPI2 HW Address of the MQTT server
//int        port     = 1883;
const char broker[] = "192.168.1.229"; // QEMU pi4-27 Address of the MQTT server
int        port     = 9883;
//const char broker[] = "192.168.1.173"; // QEMU pi4-37 Address of the MQTT server
//const char broker[] = "192.168.1.211"; // QEMU pi4-38 Address of the MQTT server
//const char topic[]  = "arduino/simple";
const char topic[]  = "update/memo";


//const char subtopic[]  = "arduino/cmd";

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;
const int buttonPin = 1;
int buttonPrevState = 0;
int buttonState = 0;         // variables for reading the pushbutton status
String subMessage = "";

void setup() {
  //Initialize serial
  Serial.begin(9600);
  pinMode(controlpin1, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(controlpin2, OUTPUT);
  pinMode(controlpin3, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
 
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  digitalWrite(controlpin2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);
  digitalWrite(controlpin2, LOW);   // turn the LED on (HIGH is the voltage level)
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
        delay(5000);
        digitalWrite(controlpin2, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  digitalWrite(controlpin1, HIGH);   // turn the LED on (HIGH is the voltage level)

  Serial.println("You're connected to the network");
  digitalWrite(LEDR,HIGH);
        delay(5000);
  digitalWrite(LEDR,LOW);      
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");
  mqttClient.setId("CID1883");


  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("user", "password");
  mqttClient.setUsernamePassword("testuser", "password123");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  //digitalWrite(LEDG,HIGH);
  while (!mqttClient.connect(broker, port)) {
    digitalWrite(LEDG,HIGH);
    delay(400);
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    digitalWrite(LEDG,LOW);
    delay(400);
    digitalWrite(LEDR,HIGH);
    delay(400);
    //delay(200);
    digitalWrite(LEDR,LOW);
    //digitalWrite(LEDG,HIGH);
    delay(400);
    
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  // subscribe to a topic
//  mqttClient.subscribe(subtopic);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(controlpin2, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void loop() {

  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  //mqttClient.poll();

  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    subMessage = "";
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

    // use the Stream interface to print the contents
    while (mqttClient.available()) {
      subMessage = subMessage + (char)mqttClient.read();
    }
    Serial.println(subMessage);

    if(subMessage == "ON") {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH)
    buttonPrevState = HIGH;

  //if ( (buttonState == LOW) && (buttonPrevState == HIGH) ) {
    count++;
    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print("BUTTON ");
    mqttClient.print(count);
    mqttClient.endMessage();
    buttonPrevState = LOW;
           digitalWrite(controlpin3, HIGH);   // turn the LED on (HIGH is the voltage level)
           delay(200); 
           digitalWrite(controlpin3, LOW);   // turn the LED on (LOW is the voltage level)
           delay(200);
           digitalWrite(controlpin1, HIGH);   // turn the LED on (HIGH is the voltage level)
           delay(200); 
           digitalWrite(controlpin1, LOW);   // turn the LED on (LOW is the voltage level)
           delay(200);
           digitalWrite(controlpin2, HIGH);   // turn the LED on (HIGH is the voltage level)
           delay(200); 
           digitalWrite(controlpin2, LOW);   // turn the LED on (LOW is the voltage level)
           delay(200);


    Serial.println("Sent MQTT message.");

  //}
}
