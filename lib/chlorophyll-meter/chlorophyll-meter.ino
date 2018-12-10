#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "inova.farm";
const char* password = "";

int R = 5;
int G = 4;
int B = 0;
int LDR = A0;

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  
  delay(10);
 
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
}
 
void loop() {
  if(WiFi.status() == WL_CONNECTED){
    String str;
      
    str = chlorophyllMeter();
      
    Serial.println(str);
    sendJSON(str);
      
    delay(1000);
  }
  else{
    Serial.println("Error in WiFi connection");
  }

  //delay(3600000); 
  delay(5000); 
}

String chlorophyllMeter(){
  String photoresistor;
  
  analogWrite(R, 255);
  analogWrite(G, 0);
  analogWrite(B, 0);
  delay(50);
  
  photoresistor = analogRead(LDR);
  
  return photoresistor;
}

void sendJSON(String json){
  HTTPClient http;  

  http.begin("http://produs.herokuapp.com/api/data/");
  http.addHeader("Content-Type", "application/json");
  
  int httpResponseCode = http.POST({datum: json});
    
    if(httpResponseCode > 0){
      String response = http.getString();

      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    else{
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode); 
    }
   http.end();
}
