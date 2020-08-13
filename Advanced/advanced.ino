#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include<ESP8266mDNS.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "YourSSID";  // Enter SSID here
const char* password = "YourPASSWORD";  //Enter Password here

ESP8266WebServer server(80);       
LiquidCrystal_I2C lcd(0x27,16,2);

uint8_t pin1=D5, pin2=D6, pin3=D7, pin4=D8, apin=A0;
bool spin1 = LOW, spin2=LOW, spin3=LOW,spin4=LOW,f1=0,f12=0;
int val3,val4=0,aspin=0;
unsigned long int curmil=0,premil=0,prm=0;
String pa,s;

void handle_pin1on();
void handle_pin1off();
void handle_pin2on();
void handle_pin2off();
void handle_pin3(int);
void handle_val();
ICACHE_RAM_ATTR void setf1(){
  f1=1;
}

uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t clock1[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t smily[8] = {0x0,0x0,0xa,0x0,0x11,0xe,0x0 };

char webpage[] = R"=(
<!DOCTYPE html>
<html lang="en" dir="ltr">
  <head>
    <meta charset="utf-8" content="width=device-width" name="viewport">
    <style>
    .content1{
      background-color: #000000F0;
      width: 100%;
      height: 100%;
      color: blue;
      max-width: 1528px;
    }

    .container {
      position: relative;
      display: inline-block;
      width: 100%;
      max-width: 60px;
      max-height: 86px;
      margin-right: 20px;
    }

    .container .btn {
      position: absolute;
      top: 0%;
      left: 0%;
      opacity: 0;
      border: none;
      width: inherit;
      height: 100%;
      cursor: pointer;
    }

    .butn{
      height: 24px;
      width: 24px;
      margin-right: 5px;
      opacity: 1;
      padding: 2px 6px;
      border-radius: 100%;
    }

    .slider {
      -webkit-appearance: none;
      width: 20%;
      height: 6px;
      min-width: 250px;
      border-radius: 3px;
      background: white;
      outline: none;
      border: 5px;
      cursor: not-allowed;
      opacity: 1;
      -webkit-transition: .1s;
      transition: opacity .1s;
    }
    .slider:hover {
      opacity: 0.7;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 15px;
      height: 15px;
      border: 0;
      border-radius: 100%;
      background: lawngreen;
      cursor: grab;
    }
    .bul41{
      animation-name: ani41;
      animation-duration: 0s;
      animation-iteration-count: infinite;
      animation-timing-function: steps(1);
    }

    @keyframes ani41 {
      0% {background: lawngreen;}
      50% {background:radial-gradient(white,black);}
      100% {background: lawngreen;}
    }
    .bul42{
      animation-name: ani41;
      animation-duration: 0s;
      animation-iteration-count: infinite;
      animation-timing-function: steps(1);
    }
    @keyframes ani41 {
      0% {background: lawngreen;}
      50% {background:radial-gradient(circle at center top, #797979 -95%, black 458%);}
      100% {background: lawngreen;}
    }
    input[type=text] {
      width: 140px;
      padding: 3px 10px;
      margin: 8px 0;
      box-sizing: border-box;
      border: none;
      border-radius: 5px;
      background-color: #3CBC8D;
      color: white;
    }
    </style>
    <title>Lights</title>
  </head>
  <body id="content1" class="content1" align="center">
    <h1 style="color:blue">NODE MCU controlling lights</h1><hr><br>
      <div class="container">
        <div style="height:86px;width:60px;">
          <div id="pin11" style="background:radial-gradient(white,black);height:60px;width:60px;border-radius:100%;"></div>
          <div id="pin12"style="position: relative;background: radial-gradient(circle at center top, #797979 -95%, black 458%);height: 8px;width: 30px;left: 15px;top: -5%;"></div>
          <div style="position:relative;background:brown;height:6px;width:30px;left:15px;top:-2%;border-radius:3px"></div>
          <div style="position:relative;background:brown;height:5px;width:30px;left:15px;top:1%;border-radius:0px 0px 5px 5px;"></div>
          <div style="position:relative;background:brown;height:6px;width:12px;left:24px;top:-1%;border-radius:0px 0px 5px 5px;"></div>
        </div>
        <button id="pin1" class="btn"></button>
      </div>
      <div class="container">
        <div style="height:86px;width:60px;">
          <div id="pin21" style="background:radial-gradient(white,black);height:60px;width:60px;border-radius:100%;"></div>
          <div id="pin22" style="position:relative;background:radial-gradient(circle at center top, #797979 -95%, black 458%);height: 8px;width: 30px;left: 15px;top: -5%;"></div>
          <div style="position:relative;background:brown;height:6px;width:30px;left:15px;top:-2%;border-radius:3px"></div>
          <div style="position:relative;background:brown;height:5px;width:30px;left:15px;top:1%;border-radius:0px 0px 5px 5px;"></div>
          <div style="position:relative;background:brown;height:6px;width:12px;left:24px;top:-1%;border-radius:0px 0px 5px 5px;"></div>
        </div>
        <button id="pin2" class="btn"></button>
      </div>
      <br><br><br>
      <div class="container">
        <div style="height:86px;width:60px;">
          <div id="pin31" style="background:radial-gradient(white,black);height:60px;width:60px;border-radius:100%;"></div>
          <div id="pin32" style="position:relative;background:radial-gradient(circle at center top, #797979 -95%, black 458%);height: 8px;width: 30px;left: 15px;top: -5%;"></div>
          <div style="position:relative;background:brown;height:6px;width:30px;left:15px;top:-2%;border-radius:3px"></div>
          <div style="position:relative;background:brown;height:5px;width:30px;left:15px;top:1%;border-radius:0px 0px 5px 5px;"></div>
          <div style="position:relative;background:brown;height:6px;width:12px;left:24px;top:-1%;border-radius:0px 0px 5px 5px;"></div>
        </div>
        <button id="pin3" class="btn"></button>
      </div>
      <br><br>
      <div>
        <input type="range" min="1" max="100" value="1" class="slider" id="slid" disabled=true>
        <p>Value: <span id="val"></span></p>
      </div>
      <br>
      <div class="container">
        <div style="height:86px;width:60px;">
          <div id="pin41" class="bul41" style="background:radial-gradient(white,black);height:60px;width:60px;border-radius:100%;"></div>
          <div id="pin42" class="bul42" style="position:relative;background:radial-gradient(circle at center top, #797979 -95%, black 458%);height: 8px;width: 30px;left: 15px;top: -5%;"></div>
          <div style="position:relative;background:brown;height:6px;width:30px;left:15px;top:-2%;border-radius:3px"></div>
          <div style="position:relative;background:brown;height:5px;width:30px;left:15px;top:1%;border-radius:0px 0px 5px 5px;"></div>
          <div style="position:relative;background:brown;height:6px;width:12px;left:24px;top:-1%;border-radius:0px 0px 5px 5px;"></div>
        </div>
        <button id="pin4" class="btn"></button>
      </div>
      <br><br>
      <div>
        <input type="range" min="30" max="1000" step="10" value="500" class="slider" id="slid2" disabled=true>
        <p>Value: <span id="val2"></span></p>
      </div>
      <br><br>
      <label for="textd">Enter text : </label>
      <input type="text" id="textda" maxlength="16">
      <button id="textbu" type="button" onclick="textdas()" style="border-radius:3px;border:none;background-color:gold;">Send</button>

  </body>

<script type="text/javascript">

  var pin1=document.getElementById('pin1')
  var pin2=document.getElementById('pin2')
  var pin3=document.getElementById('pin3')
  var pin4=document.getElementById('pin4')

  var pin11=document.getElementById('pin11')
  var pin12=document.getElementById('pin12')
  var pin21=document.getElementById('pin21')
  var pin22=document.getElementById('pin22')
  var pin31=document.getElementById('pin31')
  var pin32=document.getElementById('pin32')
  var pin41=document.getElementById('pin41')
  var pin42=document.getElementById('pin42')


  var slider = document.getElementById("slid");
  var output = document.getElementById("val");

  var slider2 = document.getElementById("slid2");
  var output2 = document.getElementById("val2");

  var content1 = document.getElementById("content1");
  var textda = document.getElementById("textda");
  var textbu = document.getElementById("textbu");
  var xhr = new XMLHttpRequest;

  setInterval(updateval,600);

  function pin1n(){
    pin11.style.background="lawngreen";
    pin12.style.background="lawngreen";
  }
  function pin1f(){
    pin11.style.background="radial-gradient(white,black)";
    pin12.style.background="radial-gradient(circle at center top, #797979 -95%, black 458%)";
  }
  function pin2n(){
    pin21.style.background="lawngreen";
    pin22.style.background="lawngreen";
  }
  function pin2f(){
    pin21.style.background="radial-gradient(white,black)";
    pin22.style.background="radial-gradient(circle at center top, #797979 -95%, black 458%)";
  }
  function sli(slival){
    output.innerHTML = slival;
    pin31.style.opacity=slival/100;
    pin32.style.opacity=slival/100;
  }
  function pin3v(slival){
    slider.disabled=false;
    slider.style.background="linear-gradient(90deg, black, yellow)";
    slider.style.cursor="pointer";
    pin31.style.background="lawngreen";
    pin32.style.background="lawngreen";
    pin31.style.opacity=slival/100;
    pin32.style.opacity=slival/100;
  }
  function pin3f(){
    slider.disabled=true;
    slider.style.background="white";
    slider.style.cursor="not-allowed";
    pin31.style.background="radial-gradient(white,black)";
    pin32.style.background="radial-gradient(circle at center top, #797979 -95%, black 458%)";
    pin31.style.opacity=1;
    pin32.style.opacity=1;
  }

  function sli2(slival){
    output2.innerHTML = slival;
    pin41.style.animationDuration=2*slival/1000+"s";
    pin42.style.animationDuration=2*slival/1000+"s";
  }
  function pin4v(slival){
    slider2.disabled=false;
    slider2.style.backgroundColor="blue";
    slider2.style.cursor="pointer";
    pin41.style.background="lawngreen";
    pin42.style.background="lawngreen";
    pin41.style.animationDuration=2*slival/1000+"s";
    pin42.style.animationDuration=2*slival/1000+"s";
  }
  function pin4f(){
    slider2.disabled=true;
    slider2.style.backgroundColor="white";
    slider2.style.cursor="not-allowed";
    pin41.style.background="radial-gradient(white,black)";
    pin42.style.background="radial-gradient(circle at center top, #797979 -95%, black 458%)";
    pin41.style.animationDuration=0+"s";
    pin42.style.animationDuration=0+"s";

  }
  function content11(grey1){
    content1.style.background="#000000"+grey1;
  }
  function textdas(){
    if(textda.value!=""){
      xhr.open("GET","text"+textda.value,true);
      xhr.send();
      textda.value="";
    }
  }
  function updateval(){
    xhr.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var xmlDoc = this.responseXML;
        var x = xmlDoc.getElementsByTagName("VALUE");

        if(x[0].childNodes[0].childNodes[0].nodeValue.match("0")){
          pin1f();
        }
        else {
          pin1n();
        }
        if(x[0].childNodes[1].childNodes[0].nodeValue.match("0")){
          pin2f();
        }
        else {
          pin2n();
        }
        if(x[0].childNodes[2].childNodes[0].nodeValue=="0"){
          pin3f();
          output.innerHTML = 0;
        }
        else {
          slider.value=x[0].childNodes[2].childNodes[0].nodeValue;
          pin3v(x[0].childNodes[2].childNodes[0].nodeValue);
          sli(x[0].childNodes[2].childNodes[0].nodeValue);
        }
        if(x[0].childNodes[3].childNodes[0].nodeValue=="0"){
          pin4f();
          output2.innerHTML = 0;
        }
        else {
          slider2.value=x[0].childNodes[3].childNodes[0].nodeValue;
          pin4v(x[0].childNodes[3].childNodes[0].nodeValue);
          sli2(x[0].childNodes[3].childNodes[0].nodeValue);
        }
        content11(x[0].childNodes[4].childNodes[0].nodeValue);
      }
    };
    xhr.open("GET", "upvalue", true);
    xhr.send();
  }

  window.addEventListener('load',function(){

    pin1.onclick=function(){
      if (pin11.style.background.match("lawngreen")){
        pin1f();
        xhr.open("GET","pin1off",true);
        xhr.send();
      }
      else{
        pin1n();
        xhr.open("GET","pin1on",true);
        xhr.send();
      }
    }

    pin2.onmousedown=function(){
      pin2n();
      xhr.open("GET","pin2on",true);
      xhr.send();
    }

    pin2.onmouseup=function(){
      pin2f();
      xhr.open("GET","pin2off",true);
      xhr.send();
    }

    pin2.addEventListener('touchstart', function(e){
      pin2n();
      xhr.open("GET","pin2on",true);
      xhr.send();
    }, false)

    pin2.addEventListener('touchend', function(e){
      pin2f();
      xhr.open("GET","pin2off",true);
      xhr.send();
    }, false)

    slider.oninput = function() {
      sli(slider.value);
      xhr.open("GET","pin3"+this.value,true);
      xhr.send();
    }
    pin3.onclick=function(){
      if (slider.disabled){
        pin3v(slider.value);
        xhr.open("GET","pin3"+slider.value,true);
        xhr.send();
      }
      else {
        pin3f();
        xhr.open("GET","pin30",true);
        xhr.send();
      }
    }
    slider2.oninput = function() {
      sli2(slider2.value);
      xhr.open("GET","pin4"+this.value,true);
      xhr.send();
    }

    pin4.onclick=function(){
      if (slider2.disabled){
        pin4v(slider2.value);
        xhr.open("GET","pin4"+slider2.value,true);
        xhr.send();
      }
      else {
        pin4f();
        xhr.open("GET","pin40",true);
        xhr.send();
      }
    }
  },false)
</script>
</html>

)=";

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(apin, INPUT);
 
  digitalWrite(pin1,spin1);
  digitalWrite(pin2,spin2);
  digitalWrite(pin3,spin3);
  digitalWrite(pin4,spin4);

  lcd.init();                      
  lcd.backlight();
  
  lcd.createChar(0, bell);
  lcd.createChar(4, heart);
  lcd.createChar(6, clock1);
  lcd.createChar(7, smily);
  lcd.home();
  
  lcd.print("Hello world...");
  lcd.setCursor(14, 0);
  lcd.write(4);
  lcd.write(4);
     
  WiFi.mode(WIFI_STA);                 
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);          

  while (WiFi.status() != WL_CONNECTED) {         
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());    
 
  if (!MDNS.begin("mylights")) {                                     
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
  lcd.clear();
  lcd.write(7);
  lcd.print(" ");
  lcd.print(WiFi.localIP());
  server.on("/",handle_root);
  server.on("/pin1on",handle_pin1on);
  server.on("/pin1off",handle_pin1off);
  server.on("/pin2on",handle_pin2on);
  server.on("/pin2off",handle_pin2off);
  server.on("/upvalue",handle_val);                                                      
  server.onNotFound([]() { 
    if (!handleFileRead(server.uri())) 
    server.send(404, "text/plain", "404: Not Found");
   });

  analogWriteRange(100);
  server.begin();
  Serial.println("HTTP server started");

  WiFi.printDiag(Serial);
  attachInterrupt(digitalPinToInterrupt(D3), setf1, RISING);
  delay(500);
  lcd.noBacklight();
}
void loop() {
  server.handleClient();
  aspin=(analogRead(apin)-1)/4;
  delay(10);
  curmil=millis();
  if(val4!=0){
    if(spin4 && curmil>=premil+val4){
      spin4=!spin4;
      digitalWrite(pin4,spin4);
      premil=curmil; 
    }
    else if(!spin4 && curmil>=premil+val4){
      spin4=!spin4;
      digitalWrite(pin4,spin4);
      premil=curmil;
    }
    delay(10);
  }
  if(f12){
    if(prm==0){
      lcd.backlight();
      delay(10);
      lcd.setCursor(14,0);
      lcd.write(0);
      lcd.write(0);
      lcd.setCursor(0,1);
      lcd.print(s);
      prm=curmil+5000;
    }
    else if(curmil>=prm){
      lcd.noBacklight();
      prm=0;
      f12=0;
    }
  }
  else if(f1){
     if(prm==0){
      lcd.backlight();
      delay(10);
      lcd.setCursor(14,0);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(0,1);
      lcd.print(s);
      prm=curmil+5000;
    }
    else if(curmil>=prm){
      lcd.noBacklight();
      prm=0;
      f1=0;
    } 
  }
  delay(10);
}

void handle_root(){
  server.send(200,"text/html",webpage);
}
void handle_pin1on(){                               
  digitalWrite(pin1,HIGH);
}
void handle_pin1off(){                               
  digitalWrite(pin1,LOW);
}
void handle_pin2on(){                             
  digitalWrite(pin2,HIGH);
}
void handle_pin2off(){                            
  digitalWrite(pin2,LOW);
}
void handle_pin3(int val3){
  analogWrite(pin3,val3);
}
void handle_pin4(int val4a){
  val4=val4a;
  if(val4a==0)
  digitalWrite(pin4,LOW);
}
void handle_text(String s1){
  s=s1;
  f12=1;
}
void handle_val(){
  String xml="<VALUE><pin1>";
  xml+=digitalRead(pin1)?"1":"0";
  xml+="</pin1><pin2>";
  xml+=digitalRead(pin2)?"1":"0";
  xml+="</pin2><pin3>";
  xml+=String(val3);
  xml+="</pin3><pin4>";
  xml+=String(val4);
  xml+="</pin4><a5>";
  xml+=aspin<17?("0"+String(aspin,HEX)):String(aspin,HEX);
  xml+="</a5></VALUE>";
  Serial.println(xml);
  server.send(200,"text/xml",xml);
}

bool handleFileRead(String path) { 
  if (path.startsWith("/pin3")){
    pa=path.substring(5);
    val3=pa.toInt();
    if(val3>-1 && val3<101){
      handle_pin3(val3);
     return true;
    }    
    else{
      return false;
    }
  }
  else if(path.startsWith("/pin4")){
    pa=path.substring(5);
    val4=pa.toInt();
    if(val4>-1 && val4<1001){
      handle_pin4(val4);
     return true;
    }    
    else{
      return false;
    }
  }
  else if(path.startsWith("/text")){
    s=path.substring(5);
    s+="               ";
    s.replace("%20"," ");
    handle_text(s);
    return true;
  }
  Serial.println("handleuri: " + path);
  Serial.println("\tURI Not Found");
  return false; 
}
