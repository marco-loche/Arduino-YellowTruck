#include <IRremote.h>

// Infrared Remote
// PIN constants
const int IR_RECIEVER_PIN = 2; // input
IRrecv irrecv(IR_RECIEVER_PIN);
const int IR_CODE_OK_PIN  = 3; // output
const int IR_CODE_KO_PIN  = 4; // output
// variable
boolean knownComand = false;
decode_results results;
// Infrared Remote END


// Motor
int motorSpeedIndex = 0;
const int maxMotorIndex = 5;
const int minMotorIndex = 0;
int motorSpeed[5] = {
  0, 
  50, 
  100,
  150,
  255
};

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(IR_CODE_OK_PIN, OUTPUT);      // sets the digital pin as output
  pinMode(IR_CODE_KO_PIN, OUTPUT);      // sets the digital pin as output
}

void loop() {
  knownComand = true;
  if (irrecv.decode(&results)) {
    long int decCode = results.value;
    switch (decCode) {
      case 16716015:
        Serial.println("left"); // remote button 4
        break;
      case 16734885:
        Serial.println("Right"); // remote button 6
        break;
      case 16726215:
        Serial.println("Stop"); // remote button 5
        motorSpeedIndex = 0;
        break;
      case 16718055:
        Serial.println("Forward"); // remote button 2
        break;  
      case 16730805:
        Serial.println("Backword"); // remote button 8
        break;
      case 16748655:
        Serial.println("Traction Motor +"); // remote button +
        if(motorSpeedIndex < maxMotorIndex) motorSpeedIndex++;
        break;  
      case 16754775:
        Serial.println("Traction Motor -"); // remote button -
        if(motorSpeedIndex > minMotorIndex) motorSpeedIndex--;
        break;  
      default: 
        knownComand = false;
        Serial.println("Waiting ...");
        break;
    }
    Serial.println(knownComand);
    setIRComandLed(knownComand);
     Serial.println(motorSpeedIndex);
    irrecv.resume(); // Receive the next value
  }
}


void setIRComandLed(boolean known){
    if(known > 0){
      digitalWrite(IR_CODE_OK_PIN, HIGH);
    }
    else{
      digitalWrite(IR_CODE_KO_PIN, HIGH);
    }
    delay(200);
    digitalWrite(IR_CODE_OK_PIN, LOW);
    digitalWrite(IR_CODE_KO_PIN, LOW);
  
}
