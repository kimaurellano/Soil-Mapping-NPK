/*
	Soil mapping using color sensor
	Datasheet: https://www.mouser.com/catalog/specsheets/TCS3200-E11.pdf

	Author:     Kim Aurellano
*/

#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

// ESP8266 interface
WiFiClient wifiClient;
PubSubClient client(wifiClient);

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

void setup() {
	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);

	// Connect to WiFi
	Serial.print(F("Waiting for WiFi connection."));
	WiFi.begin(SSID, PASSWORD);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(F("."));
		digitalWrite(LED_BUILTIN, HIGH);
		delay(50);
		digitalWrite(LED_BUILTIN, LOW);
		delay(50);
	}
	Serial.println(F("OK"));

	// Device IP
	Serial.print(F("Device IP: "));
	Serial.println(WiFi.localIP());

	// MQTT setup
	client.setServer(MQTT_SERVER, MQTT_PORT);
	client.setCallback(callback);

	Serial.print(F("Waiting for MQTT connection."));
	while (!client.connected()) {
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
	// Send message to MQTT server
	client.publish("esp8266", "hello rpi");
	client.subscribe("esp8266");
	client.loop();

	// Red
	int r = CheckColor(LOW, LOW);
	Serial.print(r); Serial.print("\t");

	// Green
	int g = CheckColor(HIGH, HIGH);
	Serial.print(g); Serial.print("\t");

	// Blue
	int b = CheckColor(LOW, HIGH);
	Serial.println(b);
}

int CheckColor(int stateS2, int stateS3) {
	digitalWrite(S2, stateS2);
	digitalWrite(S3, stateS3);

	return pulseIn(OUT, LOW);
}

// Response of MQTT on data sent
void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();
}

