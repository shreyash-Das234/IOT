#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#define DHTPIN D7
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
const char* ssid = "shreyash"; // Change hotspot Name
const char* password = "shreyash"; // Change password  

// Twilio API credentials
const char* accountSid = "AC85b2d7df4c28315a4c3e99448fbe73e7"; //Your account SID
const char* authToken  = "056033469f09713678015133987e1885";// Your authToken

// Messaging Service SID (instead of From number)
const char* messagingServiceSid = "MG2c696ebc421f304111fcb94e27e5a412"; // Change messaging Service ID
const char* toNumber = "+917977529772"; // Destination number

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi");
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }
  
  // Build message body
  String message = "Weather Update Temp: " + String(temperature) + "C, Humidity: " + String(humidity) + "%";
  sendSMS(message);
  
  delay(60000); // send every 1 minute
}

void sendSMS(String body) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure(); // skip SSL verification
    
    String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(accountSid) + "/Messages.json";
    
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.setAuthorization(accountSid, authToken);
    
    // Using MessagingServiceSid instead of From
    String postData = "MessagingServiceSid=" + String(messagingServiceSid) + "&To=" + String(toNumber) + "&Body=" + body;
    
    int httpCode = http.POST(postData);
    String payload = http.getString();
    
    Serial.println("HTTP Code: " + String(httpCode));
    Serial.println("Response: " + payload);
    
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
