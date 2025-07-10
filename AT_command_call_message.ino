#include <SoftwareSerial.h>

#define RX_PIN 2  // EC200U TX
#define TX_PIN 3  // EC200U RX

SoftwareSerial ec200u(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(115200);
  ec200u.begin(115200);
  delay(5000);

  Serial.println("Initializing EC200U...");

  sendCommand("AT");
  sendCommand("ATI");
  sendCommand("AT+CSQ");
  sendCommand("AT+CREG?");
  sendCommand("AT+CMGF=1");

  sendSMS("+919101050885", "Hello, I am Bishwajit!");
  delay(5000);
  makeCall("+919101050885");
}

void loop() {
}

void sendSMS(String phoneNumber, String message) {
  Serial.println("Sending SMS...");

  ec200u.print("AT+CMGS=\"");
  ec200u.print(phoneNumber);
  ec200u.println("\"");

  waitForResponse(2000);  // Wait for prompt

  ec200u.print(message);
  waitForResponse(500);

  ec200u.write(26);  // CTRL+Z to send SMS
  Serial.println("SMS Sent!");
}

void makeCall(String phoneNumber) {
  Serial.println("Making a call...");
  ec200u.print("ATD");
  ec200u.print(phoneNumber);
  ec200u.println(";" ); // Semicolon is required for voice call
  waitForResponse(5000);
}

void sendCommand(String command) {
  Serial.print("Sending: ");
  Serial.println(command);
 
  while (ec200u.available()) ec200u.read();  // Clear buffer

  ec200u.println(command);
  waitForResponse(1000);
}

void waitForResponse(unsigned long timeout) {
  unsigned long start = millis();
  while (millis() - start < timeout) {
    while (ec200u.available()) {
      Serial.write(ec200u.read());
    }
  }
  Serial.println();
}
