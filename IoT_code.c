#include <WiFi.h>
#include <ThingSpeak.h>

const char* ssid = "C wing comfort";
const char* password = "Comfort@420";
const char* thingSpeakApiKey = "PAHC48MWSX07B9MS";

const int trigPin = 2; // Connect trigger pin of HC-SR04 to GPIO 2
const int echoPin = 4; // Connect echo pin of HC-SR04 to GPIO 4
const int mq135Pin = A0; // Analog pin connected to MQ135 sensor

WiFiClient client;

unsigned long previousMillis = 0;
const long interval = 20000; // Update every 20 seconds

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    float distance = getDistance();
    float gasConcentration = getGasConcentration();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm, Gas Concentration: ");
    Serial.print(gasConcentration);
    Serial.println(" unit");

    // Send distance and gas concentration data to ThingSpeak channel
    ThingSpeak.setField(1, distance);
    ThingSpeak.setField(2, gasConcentration);
    //ThingSpeak.writeField(2489789, 2, gasConcentration, thingSpeakApiKey) && ThingSpeak.writeField(2489789, 1, distance, thingSpeakApiKey);
    ThingSpeak.writeFields(2489789, thingSpeakApiKey);

    previousMillis = currentMillis;
  }
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2; // Speed of sound in air is 340 m/s
  return distance;
}

float getGasConcentration() {
  // Replace with your actual code to read analog value from MQ135 sensor
  int sensorValue = analogRead(mq135Pin);
  
  // Calibration parameters
  float sensorMin = 100; // Minimum analog value in clean air
  float sensorMax = 1000; // Maximum analog value in high gas concentration
  
  // Map analog value to gas concentration range (ppm)
  float gasConcentration = map(sensorValue, sensorMin, sensorMax, 0, 1000); // Assuming the range is 0-1000 ppm
  
  return gasConcentration;
}
