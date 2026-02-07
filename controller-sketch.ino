#include <WiFi.h>
#include <ModbusIP_ESP8266.h>

// ---------------------------------------------------------
// 1. WIFI SETTINGS (CHANGE THESE!)
// ---------------------------------------------------------
const char* ssid = "<ENTER_WIFI_SSID_NAME>";         // Keep the quotes!
const char* password = "<ENTER_WIFI_SSID_PASSWORD>"; // Keep the quotes!

// ---------------------------------------------------------
// 2. PIN DEFINITIONS
// ---------------------------------------------------------
const int LIGHT_PIN = 4;   // GPIO 26 -> Relay IN1
const int FAN_PIN = 27; // GPIO 27 -> Relay IN2

// Modbus Registers (The "Addresses" for the Python script)
const int FAN_COIL = 100;
const int LIGHT_COIL = 101;

ModbusIP mb;

void setup() {
  Serial.begin(115200); // Start the serial monitor

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.print("MY IP ADDRESS: ");
  Serial.println(WiFi.localIP()); // <--- IMPORTANT: Look for this!

  // Configure Pins
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  
  // Set default state (OFF) - Note: Some relays are "Active Low"
  // If your fan starts ON, change LOW to HIGH here.
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LIGHT_PIN, LOW);

  // Start Modbus Server
  mb.server();
  mb.addCoil(FAN_COIL);
  mb.addCoil(LIGHT_COIL);
}

void loop() {
  // Listen for commands
  mb.task();

  // Update hardware based on Modbus commands
  digitalWrite(FAN_PIN, mb.Coil(FAN_COIL));
  digitalWrite(LIGHT_PIN, mb.Coil(LIGHT_COIL));
}
