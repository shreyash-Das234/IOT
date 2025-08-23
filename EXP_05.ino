#define BLYNK_TEMPLATE_ID "TMPL6EQyNZGmf"
#define BLYNK_TEMPLATE_NAME "Weather Monitor"
#define BLYNK_AUTH_TOKEN "csO29wsYOwiV9aGX311ifPXNF23CTmy-"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "shreyash";   // Your WiFi SSID
char pass[] = "shreyash";   // Your WiFi Password

BlynkTimer timer;

#define DHTPIN D7          // DHT sensor pin
#define DHTTYPE DHT11      
DHT dht(DHTPIN, DHTTYPE);

// ===== LED Pins =====
#define LED1 D5   // Connect LED1 here (with resistor ~220Ω)
#define LED2 D6   // Connect LED2 here

// ===== Blynk Virtual Pins =====
#define VPIN_LED1 V2   // Virtual button in Blynk app
#define VPIN_LED2 V3   // Virtual button in Blynk app

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Send sensor values to Blynk
  Blynk.virtualWrite(V0, t); 
  Blynk.virtualWrite(V1, h);

  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print(" °C    Humidity : ");
  Serial.print(h);
  Serial.println(" %");
}

// ===== Blynk Button Handlers =====
BLYNK_WRITE(VPIN_LED1) {
  int value = param.asInt(); // Get value from Blynk (0/1)
  digitalWrite(LED1, value); // Write to LED1
  Serial.print("LED1 is ");
  Serial.println(value ? "ON" : "OFF");
}

BLYNK_WRITE(VPIN_LED2) {
  int value = param.asInt(); 
  digitalWrite(LED2, value); 
  Serial.print("LED2 is ");
  Serial.println(value ? "ON" : "OFF");
}

void setup() {   
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  dht.begin();

  // Setup LEDs as OUTPUT
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  // Send sensor data every 1 second
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
