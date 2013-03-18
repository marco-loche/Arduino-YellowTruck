/*
* DC Motor Control with L293D
*/

int motorEnablePin = 11;
int in1Pin = 10; //forward
int in2Pin = 9; //backward

int directionEnablePin = 2;
int in3Pin = 3; //left
int in4Pin = 4; //right


int potPin = 0;

const int xPin = 0; // X Axis
const int yPin = 1; // Y Axis
void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);
  pinMode(directionEnablePin, OUTPUT);
//  pinMode(switchPin, INPUT_PULLUP);  
  Serial.begin(9600);
}
 
void loop()
{
  int speed = analogRead(potPin) / 4;
  int directionAnalogValue = analogRead(yPin);
  int directionValue = map(directionAnalogValue, 0, 1023, 0, 3); 
  if(directionValue == 3) directionValue = 2;
  
  switch(directionValue){
    case 0:
       setDirection(true, false);
      break;
    case 2:
       setDirection(false, true);
      break;
    default:
       setDirection(false, false);
      break;
      
  }
  
  int tractionAnalogValue = analogRead(xPin);
  int tractionValue = map(tractionAnalogValue, 0, 1023, 0, 3);
  int motorSpeed =  map(tractionAnalogValue, 0, 1023, 0, 510);
  
  if(motorSpeed > 255) 
    motorSpeed = motorSpeed - 255;
  else 
    motorSpeed = -(motorSpeed - 255);
  
  Serial.println("--------------------");
  if(tractionValue == 3) tractionValue = 2;
  switch(tractionValue){
    case 0:
     Serial.print("Motor forward " );
       setMotor( true, false, motorSpeed);
      break;
    case 2:
      Serial.print("Motor backword " );
       setMotor(false, true, motorSpeed);
      break;
    default:
    Serial.print("Motor stop " );
      setMotor( false, false, 0);
      break;
      
  }
  Serial.print("Motor Speed : " + String(motorSpeed) );
  Serial.print("Motor Direction " + String(tractionValue) + " ");
  Serial.println("--------------------");
  
}
 
void setMotor(boolean forward, boolean backward, int motorSpeed)
{
  analogWrite(motorEnablePin, motorSpeed);
  digitalWrite(in1Pin, forward);
  digitalWrite(in2Pin, backward);
}

void setDirection(boolean left, boolean right){
  analogWrite(directionEnablePin, 255);
  digitalWrite(in3Pin, left);
  digitalWrite(in4Pin, right);
}
