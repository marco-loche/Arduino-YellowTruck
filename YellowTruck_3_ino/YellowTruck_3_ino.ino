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
const int maxMotorIndex = 5; // it's an array index dim-1
const int minMotorIndex = 0;
int motorSpeed[6] = {
  0, 
  150, 
  175,
  200,
  225,
  250
};
/*
* DC Motor Control with L293D
*/

const int MOTOR_ENABLED_PIN = 11;
const int FORWARD_PIN = 9; //forward in1Pin
const int BACKWARD_PIN = 10; //backward in2Pin

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
        Serial.println("left"); // remote button 4
        left = !left;
        right = false;
        break;
      case 16734885:
        Serial.println("Right"); // remote button 6
        left = false;
        right = !right;
        break;
      case 16726215:
        Serial.println("Stop"); // remote button 5
        motorSpeedIndex = 0;
        left = false;
        right = false;
        forward = false;
        backward = false;
        break;
      case 16718055:
        Serial.println("Forward"); // remote button 2
        forward = !forward;
        backward = false;
        break;  
      case 16730805:
        Serial.println("Backward"); // remote button 8
        forward = false;
        backward = !backward;
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

/* Debugging
    Serial.println(knownComand);
    String output = "Motor Speed :";
    output += motorSpeed[motorSpeedIndex];
    Serial.println(output);
*/
    setIRComandLed(knownComand);
    setDirection(left, right);
    setMotor( forward, backward, motorSpeed[motorSpeedIndex]);
    
    irrecv.resume(); // Receive the next value
  }
}

void setMotor(boolean forward, boolean backward, int motorSpeed)
{
 digitalWrite(MOTOR_ENABLED_PIN, HIGH);
 if(forward){
  analogWrite(FORWARD_PIN, motorSpeed);
  analogWrite(BACKWARD_PIN, LOW);
 }
 if(backward){
  analogWrite(FORWARD_PIN, LOW);
  analogWrite(BACKWARD_PIN, motorSpeed);
 }
  if(!backward && !forward){
  analogWrite(FORWARD_PIN, LOW);
  analogWrite(BACKWARD_PIN, LOW);
  digitalWrite(MOTOR_ENABLED_PIN, LOW);
  motorSpeedIndex = 0;
 }
}

void setDirection(boolean left, boolean right){
  
  digitalWrite(DIRECTION_ENABLED_PIN, HIGH);  
  if(left){
    analogWrite(LEFT_PIN, 255);
    analogWrite(RIGHT_PIN, 0);
  }
  if(right){
     analogWrite(LEFT_PIN, 0);
    analogWrite(RIGHT_PIN, 255);
  }
  if(!left && !right){
    analogWrite(LEFT_PIN, 0);
    analogWrite(RIGHT_PIN, 0);
    digitalWrite(DIRECTION_ENABLED_PIN, LOW);
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
