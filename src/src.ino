/*
	Soil mapping using color sensor
	Datasheet: https://www.mouser.com/catalog/specsheets/TCS3200-E11.pdf

	Author:     Kim Aurellano
*/

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

const char* SSID = "ssid";
const char* PASSWORD = "password";

const int S0 = D4;
const int S1 = D5;
const int S2 = D6;
const int S3 = D7;
const int OUT = D8;

void setup() {
	// Connect to WiFi
	WiFi.begin(SSID, PASSWORD);
	while (WiFi.status() != WL_CONNECTED) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(50);
		digitalWrite(LED_BUILTIN, LOW);
		delay(50);
	}
	Serial.println(F("WIFI CONNECTED."));

	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);
	pinMode(OUT, INPUT);

	// Frequency scale
	digitalWrite(S0, HIGH);
	digitalWrite(S1, LOW);

	Serial.println(F("DONE SETTING UP."));
}

void loop() {
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

