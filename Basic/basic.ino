#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include<ESP8266mDNS.h>
const char* ssid = "YourSuperIdea";  // Enter SSID here
const char* password = "8827196303&Tk";  //Enter Password here

ESP8266WebServer server(801);       //starting server with port 801

uint8_t pin1=D0 , pin2=D2, pin3=D3, pin4=D4;
bool spin1 = LOW, spin2=LOW, spin3=LOW, spin4=LOW;
void handle_pin1();
void handle_pin2();
void handle_pin3();
void handle_pin4();
void handle_OnConnect();
String strings();
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(D4,OUTPUT);
  digitalWrite(D4,HIGH);
  digitalWrite(pin1,spin1);
  digitalWrite(pin2,spin2);
  digitalWrite(pin3,spin3);
  digitalWrite(pin4,spin4);
    
  WiFi.mode(WIFI_STA);                  //WiFi mode
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);           //connect to your local wi-fi network

  while (WiFi.status() != WL_CONNECTED) {         //check wi-fi is connected to wi-fi network
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());        //print IP address
 
  if (!MDNS.begin("esp8266")) {                                     //starting MDNS 
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
                                                        
  server.on("/", HTTP_GET, handle_OnConnect);                       //initialising the http response from url    
  server.on("/pin1", HTTP_POST, handle_pin1);
  server.on("/pin2", HTTP_POST, handle_pin2);
  server.on("/pin3", HTTP_POST, handle_pin3);
  server.on("/pin4", HTTP_POST, handle_pin4);
  server.onNotFound([](){
    server.send(404,"text/html", "Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
  MDNS.addService("http","tcp",801);
}
void loop() {
  MDNS.update();
  server.handleClient();
}
String strings(){                           //html generation//backslash to use " as normal character
  String a1="<head><meta name=\"viewport\" content=\"width=device-width\"></head><title>Control Ligths</title><body text = \"aqua\" bgcolor = \"black\"><center><h1>Press buttons to toggle the state of switch</h1><hr><br><form action=\"/pin1\" method=\"POST\">Switch 1  - <input type=\"submit\"value=\"";
  a1+=digitalRead(pin1)?"OFF":"ON";
  a1+="\"></form><form action=\"/pin2\" method=\"POST\">Switch 2  - <input type=\"submit\"value=\"";
  a1+=digitalRead(pin2)?"OFF":"ON";
  a1+="\"></form><form action=\"/pin3\" method=\"POST\">Switch 3  - <input type=\"submit\"value=\"";
  a1+=digitalRead(pin3)?"OFF":"ON";
  a1+="\"></form><form action=\"/pin4\" method=\"POST\">Switch 4  - <input type=\"submit\"value=\"";
  a1+=digitalRead(pin4)?"OFF":"ON";
  a1+="\"></form><br></center></body>";
  Serial.println(a1);
  return a1;
}
void handle_OnConnect() {                         //function to handle request
  String a2=strings();
  server.send(200, "text/html",a2); 
}
void handle_pin1(){                               //function to handle request
  spin1=!spin1;
  digitalWrite(pin1,spin1);
  server.sendHeader("Location","/");
  server.send(303);
}
void handle_pin2(){                               //function to handle request
  spin2=!spin2;
  digitalWrite(pin2,spin2);
  server.sendHeader("Location","/");
  server.send(303);
}
void handle_pin3(){                               //function to handle request
  spin3=!spin3;
  digitalWrite(pin3,spin3);
  server.sendHeader("Location","/");
  server.send(303);
}
void handle_pin4(){                                //function to handle request
  spin4=!spin4;
  digitalWrite(pin4,spin4);
  server.sendHeader("Location","/");
  server.send(303);
}
