
// Joystick Analog Pins
const int xPin = 0; // X Axis
const int yPin = 1; // Y Axis
const int bPin = 2; // button

// Storage variable for joystig input
// the y axis control the traction motor
// 0 = back
// 1 = stop
// 2 = forward
int tractionYValue = 1;

// Storage variable for joystig input
// the x axis control the direction motor
// 0 = left
// 1 = center
// 2 = right
int directionXValue = 1;

int bValue = 0;

//Motor Control throug a 74HC595
//Pin connected to latch pin (ST_CP) of 74HC595 - green
const int latchPin = 8;
//Pin connected to clock pin (SH_CP) of 74HC595 - yellow
const int clockPin = 12;
////Pin connected to Data in (DS) of 74HC595 - blue
const int dataPin = 11;

const byte tractionByte[3] = {
  B00000011, 
  B00000000, 
  B00001100
};

const String tractionString[3] = {
  "Forward", 
  "Stop", 
  "Back"
};

const byte directionByte[3] = {
  B11000000,
  B00000000,
  B00110000
};

const String directionString[3] = {
  "Left", 
  "Center", 
  "Right"
};

// Double H-Bridge L293E

const int motorsEnablePin = 10;
boolean motorsEnabled = false;

void setup() {
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(bPin, INPUT);
  
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  pinMode(motorsEnablePin, OUTPUT);
  digitalWrite(motorsEnablePin, LOW);
  
  Serial.begin(9600);
  
  // Check
  byte allHig = B11111111;
  registerWrite(allHig, HIGH);
  delay(1000);
  byte allLow = B00000000;
  registerWrite(allLow, LOW);
  Serial.println("***READY****");
}

void loop() {
  //Retrieve joystick values and map them
  int directionAnalogValue = analogRead(xPin);
  directionXValue = map(directionAnalogValue, 0, 1023, 0, 3);
  int tractionAnalogValue = analogRead(yPin);
  tractionYValue = map(tractionAnalogValue, 0, 1023, 0, 3); 
 
  if(directionXValue == 3) directionXValue = 2;
  if(tractionYValue == 3) tractionYValue = 2;
  
  // compute the final instruction combining traction and direction byte 
  byte instruction = tractionByte[tractionYValue] | directionByte[directionXValue] ;
  registerWrite(instruction, HIGH);
 
  // 
  bValue = analogRead(bPin);
  if (bValue == 0) {
     if(motorsEnabled){
      digitalWrite(motorsEnablePin, LOW);
      motorsEnabled = false;
     }
     else{
      digitalWrite(motorsEnablePin, HIGH);
      motorsEnabled = true;
     }
  }
  
  output(directionXValue, tractionYValue, bValue,instruction )
  delay(100);
}

// This method sends bits to the shift register:

void registerWrite(byte byteToSet, int whichState) {

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);

  // turn on the next highest bit in bitsToSend:
  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, byteToSet);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

}

void output(int directionXValue, int tractionYValue, int bValue, byte instruction)
{
 Serial.print(String(directionXValue) + " ");
  Serial.print(directionString[directionXValue]);
  Serial.println("--------------------");
  Serial.print(String(tractionYValue) + " ");
  Serial.print(tractionString[tractionYValue]);
  Serial.println("--------------------");
  if(bValue == 0){
    Serial.println("--------------------");
    Serial.print("button pushed");
  }
  Serial.println("++++++++++++++++++++");
  Serial.println(String(instruction, BIN));
  Serial.println("++++++++++++++++++++");
  
}
