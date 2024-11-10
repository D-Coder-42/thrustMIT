#include <SoftwareSerial.h>

// GPIO definitions
#define RX_RYLR       7 // CONNECT TX OF RYLR TO THIS
#define TX_RYLR       6 // CONNECT RX OF RYLR TO THIS
#define D4184_DROGUE  3
#define D4184_MAIN    4

// Object Instantiations
SoftwareSerial RYLR(RX_RYLR, TX_RYLR);;

// Variable Definitions
String send, response;

// Prototype Function Definitions
String parseRYLR(String input) {
  int start = input.indexOf(',') + 1;
  start = input.indexOf(',', start) + 1;
  int end = input.indexOf(',', start);
  String parsed = input.substring(start, end);
  parsed.trim();  
  return parsed;
}

void sendState(String message) {
  send = "TESTBED MESSAGE: " + message;
  send = "AT+SEND=0,"+ String(send.length()) + "," + send + "\r\n";
  RYLR.print(send);
  delay(10);
}

void checkInput(String receive) {
  if (receive == "CHECK") {
    Serial.println("COMMS ESTABLISHED.");
    sendState("COMMS CHECK COMPLETE");
    return;
  }
  else if (receive == "DROGUE") {
    Serial.println("DROGUE WENT BOOM.");
    Serial.println("DROGUE D4184 LATCHED");
    digitalWrite(D4184_DROGUE, HIGH);
    sendState("DROGUE LATCHED");
    delay(5000);
    Serial.println("DROGUE D4184 UNLATCHED");
    digitalWrite(D4184_DROGUE, LOW);
    sendState("DROGUE UNLATCHED");
    return;
  }
  else if (receive == "MAIN") {
    Serial.println("MAIN WENT BOOM.");
    Serial.println("MAIN D4184 LATCHED");
    digitalWrite(D4184_MAIN, HIGH);
    sendState("MAIN LATCHED");
    delay(5000);
    Serial.println("MAIN D4184 UNLATCHED");
    digitalWrite(D4184_MAIN, LOW);
    sendState("MAIN UNLATCHED");
    return;
  }
  else {
    Serial.println("INVALID INPUT PROVIDED. ");
    sendState("ERROR");
    return;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(D4184_DROGUE, OUTPUT);
  pinMode(D4184_MAIN, OUTPUT);
  digitalWrite(D4184_DROGUE, LOW);
  digitalWrite(D4184_MAIN, LOW);

  //RYLR setup
  RYLR.begin(57600);
  Serial.println("TESTBED SETUP COMPLETE.");
}

void loop() {
  if (RYLR.available()) {
    response = RYLR.readStringUntil('\n');
    response = parseRYLR(response);
    if (response.length() > 3) {
      Serial.println("MESSAGE RECEIVED: " + response);
      checkInput(response);
    }
  }
}