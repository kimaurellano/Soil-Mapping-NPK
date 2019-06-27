/*
	Soil mapping using color sensor
	Datasheet: https://www.mouser.com/catalog/specsheets/TCS3200-E11.pdf

	Author:     Kim Aurellano
*/

#include <ArduinoJson.h>
#include <MQTTClient.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

// ESP8266 interface
WiFiClient wifiClient;
MQTTClient mqttClient;

// WiFi credentials
const char* SSID = "Xiaomi_0FBC";
const char* PASSWORD = "ukesAs!52637";

// MQTT
const int MQTT_PORT = 1883;
const char* MQTT_SERVER = "192.168.31.154"; // IP Address of the Raspi
const char* MQTT_USER = "";
const char* MQTT_PASS = "";

// TCS3200
const int S0 = D4;
const int S1 = D5;
const int S2 = D6;
const int S3 = D7;
const int OUT = D8;

//
const int STATUS_LED = 2;

void setup() {
	Serial.begin(115200);

	pinMode(STATUS_LED, OUTPUT);

	// Connect to WiFi
	Serial.print(F("Waiting for WiFi connection."));
	WiFi.begin(SSID, PASSWORD);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(F("."));
		digitalWrite(STATUS_LED, HIGH);
		delay(50);
		digitalWrite(STATUS_LED, LOW);
		delay(50);
	}
	Serial.println(F("OK"));

	// Device IP
	Serial.print(F("Device IP: "));
	Serial.println(WiFi.localIP());

	// MQTT broker connection
	Serial.print(F("Waiting for MQTT broker connection."));
	mqttClient.begin(MQTT_SERVER, wifiClient);

	while (!mqttClient.connect(WiFi.localIP().toString().c_str())) {
		Serial.print(F("."));
	}
	Serial.println(F("OK"));

	// Set pins for TCS32000 color sensor
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);
	pinMode(OUT, INPUT);

	// Frequency scale 20%
	digitalWrite(S0, HIGH);
	digitalWrite(S1, LOW);

	Serial.println(F("DONE SETTING UP."));
}

void loop() {
	digitalWrite(STATUS_LED, !WiFi.isConnected());

	// Create JSON object
	StaticJsonDocument<200> jsonDoc;
	jsonDoc["SensorID"] = WiFi.localIP().toString();
	jsonDoc["Nitrogen"] = 0;
	jsonDoc["Phosphorous"] = 0;
	jsonDoc["Potassium"] = 0;

	// The data to send
	String payload;
	serializeJsonPretty(jsonDoc, payload);

	Serial.println("published at esp8266/dev2 topic");
	Serial.println(payload);

	// Publish 'esp8266' topic and message to MQTT broker
	mqttClient.publish("esp8266/dev2", payload); // Provide a json object message
	mqttClient.loop();
	delay(5000);

	//// Red
	//int r = CheckColor(LOW, LOW);
	//Serial.print(r); Serial.print("\t");

	//// Green
	//int g = CheckColor(HIGH, HIGH);
	//Serial.print(g); Serial.print("\t");

	//// Blue
	//int b = CheckColor(LOW, HIGH);
	//Serial.println(b);
}

int CheckColor(int stateS2, int stateS3) {
	digitalWrite(S2, stateS2);
	digitalWrite(S3, stateS3);

	return pulseIn(OUT, LOW);
}

