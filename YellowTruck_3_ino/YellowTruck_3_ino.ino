#include <IRremote.h>

// Infrared Remote
// PIN constants
const int IR_RECIEVER_PIN = 5; // input
IRrecv irrecv(IR_RECIEVER_PIN);
const int IR_CODE_OK_PIN  = 6; // output
const int IR_CODE_KO_PIN  = 7; // output
// variable
boolean knownComand = false;
decode_results results;
// Infrared Remote END


// Motor
// Traction Motor Speed 
int motorSpeedIndex = 0;
const int maxMotorIndex = 4; // it's an array index dim-1
const int minMotorIndex = 0;
int motorSpeed[5] = {
  0, 
  50, 
  100,
  150,
  255
};
/*
* DC Motor Control with L293D
*/

const int MOTOR_ENABLED_PIN = 11;
const int FORWARD_PIN = 10; //forward in1Pin
const int BACKWARD_PIN = 9; //backward in2Pin

const int DIRECTION_ENABLED_PIN = 2;
const int LEFT_PIN = 3; //left in3Pin
const int RIGHT_PIN = 4; //right in4Pin

boolean left = false;
boolean right = false;
boolean forward = false;
boolean backward = false;

void setup()
{
  irrecv.enableIRIn(); // Start the receiver
  
  pinMode(IR_CODE_OK_PIN, OUTPUT);      // sets the digital pin as output
  pinMode(IR_CODE_KO_PIN, OUTPUT);      // sets the digital pin as output
  
  pinMode(FORWARD_PIN, OUTPUT);
  pinMode(BACKWARD_PIN, OUTPUT);
  pinMode(LEFT_PIN, OUTPUT);
  pinMode(RIGHT_PIN, OUTPUT);
  pinMode(MOTOR_ENABLED_PIN, OUTPUT);
  pinMode(DIRECTION_ENABLED_PIN, OUTPUT);
 
  Serial.begin(9600); 
}

void loop() {
  knownComand = true;
  if (irrecv.decode(&results)) {
    long int decCode = results.value;
        Serial.println(decCode);
    switch (decCode) {
      case 16716015:
      //case -1943902853 :
        Serial.println("left"); // remote button 4
        left = !left;
        right = false;
        break;
      case 16734885:
      //case 71952287:
        Serial.println("Right"); // remote button 6
        left = false;
        right = !right;
        break;
      case 16726215:
      //case 1217346747:
        Serial.println("Stop"); // remote button 5
        motorSpeedIndex = 0;
        left = false;
        right = false;
        forward = false;
        backward = false;
        break;
      case 16718055:
      //case 1033561079:
        Serial.println("Forward"); // remote button 2
        forward = !forward;
        backward = false;
        break;  
      case 16730805:
      //case 465573243:
        Serial.println("Backword"); // remote button 8
        forward = false;
        backward = !backward;
        break;
      case 16748655:
      //case -439370369:
        Serial.println("Traction Motor +"); // remote button +
        if(motorSpeedIndex < maxMotorIndex) motorSpeedIndex++;
        break;  
      case 16754775:
      //case -1547112997:
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
    setDirection(left, right);
    setMotor( forward, backward, 255);
    irrecv.resume(); // Receive the next value
  }
}

void setMotor(boolean forward, boolean backward, int motorSpeed)
{
  analogWrite(MOTOR_ENABLED_PIN, motorSpeed);
  digitalWrite(FORWARD_PIN, forward);
  digitalWrite(BACKWARD_PIN, backward);
}

void setDirection(boolean left, boolean right){
  analogWrite(DIRECTION_ENABLED_PIN, 255);
  digitalWrite(LEFT_PIN, left);
  digitalWrite(RIGHT_PIN, right);
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
