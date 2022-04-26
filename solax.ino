/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Your WIFI Informations
const char* ssid = "WIFI SSID";
const char* password = "WIFI PASSWORD";

//Put here API from Solax Cloud
String serverName = "API LINK HERE";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

void setup() {
  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
}

void loop() {
  
  //Send an HTTP POST request every 10 Seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        //Serial.println(" ");
        
        StaticJsonDocument<768> doc;

        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
         Serial.print("deserializeJson() failed: ");
         Serial.println(error.c_str());
         return;
        }

        bool success = doc["success"]; 
        const char* exception = doc["exception"]; 

        JsonObject result = doc["result"];
        int solax_acpower = result["acpower"]; 
        float solax_powertoday = result["yieldtoday"]; 
        int solax_powertotal = result["yieldtotal"]; 
        const char* solax_inverterStatus = result["inverterStatus"]; 
        int solax_powerdc1 = result["powerdc1"]; 
        int solax_powerdc2 = result["powerdc2"]; 

        //If data connection to cloud is ok, print some values...
        if (success = "true"){
         Serial.println(solax_acpower);
         Serial.println(solax_powertoday);
         Serial.println(solax_powertotal);
         Serial.println(solax_powerdc1);
        }
        
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
