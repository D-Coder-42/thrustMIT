#include <SoftwareSerial.h>

// GPIO definitions
#define RX_RYLR       7 // CONNECT TX OF RYLR TO THIS
#define TX_RYLR       6 // CONNECT RX OF RYLR TO THIS

SoftwareSerial RYLR (RX_RYLR, TX_RYLR);
String data,transmit,response;

void sendData(String data) {
  transmit = "AT+SEND=0," + String(data.length()) + "," + data + "\r\n";
  RYLR.print(transmit);
  delay(10);
}

String parseRYLR(String input) {
  int start = input.indexOf(',') + 1;
  start = input.indexOf(',', start) + 1;
  int end = input.indexOf(',', start);
  String parsed = input.substring(start, end);
  parsed.trim();
  return parsed;  
}

void receiveData() {
  if (RYLR.available()) {
    response = RYLR.readStringUntil('\n');
    response = parseRYLR(response);
    Serial.println(response);
    //RYLR.clear();
  }
}

void setup() {
  Serial.begin(9600);
  RYLR.begin(57600);
  sendData("CHECK");
  Serial.println("GROUNDSTATION SETUP COMPLETE.");
}

void loop() {
  if(Serial.available()) {
    data = Serial.readStringUntil('\n');
    sendData(data);
    Serial.print("TRANSMITTED: " + transmit);
    delay(250);
  }
  receiveData();
}