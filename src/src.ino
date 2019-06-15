/*
	Soil mapping using color sensor
	Datasheet: https://www.mouser.com/catalog/specsheets/TCS3200-E11.pdf

	Author:     Kim Aurellano
*/

#include <MySQL_Cursor.h>
#include <MySQL_Connection.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

// Change according to your settings
IPAddress server_addr(192, 168, 0, 1);
char serverUser[] = "root";	// MySQL user
char serverPass[] = "pass";	// MySQL pass
const int PORT = 3306;		// Common port use for MySQL database

// ESP8266 interface
WiFiClient wifiClient;

// MySQL 
MySQL_Connection conn(&wifiClient);
MySQL_Cursor* cursor;

// WiFi credentials
const char* SSID = "Xiaomi_0FBC";
const char* PASSWORD = "ukesAs!52637";

// TCS3200
const int S0 = D4;
const int S1 = D5;
const int S2 = D6;
const int S3 = D7;
const int OUT = D8;

// Insert query
char INSERT_SQL[] = "INSERT INTO test_arduino.hello_arduino (message) VALUES ('Hello, Arduino!')";

void setup() {
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

	// Connection to MySQL server
	Serial.print(F("Connecting to server."));
	while (!conn.connect(server_addr, PORT, serverUser, serverPass)) {
		Serial.print(F("."));
		delay(500);
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

