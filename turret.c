#include <Servo.h>

// digital pins
int motors[2][4] = {{8,9,10,11},{3,4,5,6}}; // array of pins for each motor connected. 0: vertical; 1: horizontal
int servoPin     = 7;  // trigger servo
int sensorPin    = 2;  // PIR sensor
int buzzerPin    = 12; // really fucking annoying buzzer

// analog pins
int sw   = A0; //SW  - thumbstick button press
int horz = A1; //VRX - thumbstick x-axis
int vert = A2; //VRY - thumbstick y-axis


// value of currents for thumbstick inputs
int swValue   = 0;  
int horzValue = 0;
int vertValue = 0;

Servo trigServo; // instanciate trigger servo

int motorSpeed = 1200;  // variable to set stepper speed
bool isFiring  = false; // true implies gun is firing
int lookup[8]  = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001}; // control codes for stepper motors


void setup() {

  // initialise the trigger servo at 0 degrees
  trigServo.attach(trigServoPin);
  trigServo.write(0);

  // set all motor pins to output mode
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < (int)(sizeof(motors) / sizeof(motors[0]); j++)  // dynamically calculates number of motors in list
    {  
        pinMode(motors[j][i], OUTPUT);  
    }
  }

  // set mode for non-motor components
  pinMode(sensorPin,INPUT);
  pinMode(buzzerPin,OUTPUT);

  Serial.begin(9600);

}


void loop() {
  
  // read the values from the thumbstick
  swValue = analogRead(sw);
  horzValue = analogRead(horz);
  vertValue = analogRead(vert);

  // check triggers for firing
  isFiring = (swValue==0 || digitalRead(sensor) == HIGH);

  // move motor 0 according to vertical input
  if(vertValue>800)
  {
    anticlockwise(0); // moves up
  } else if(vertValue<200) {
   clockwise(0); // moves down
  }

  // move motor 1 according to horizonal input
  if(horzValue<200)
  {
    anticlockwise(1);
  } else if(horzValue>800) {
    clockwise(1);
  }
  
  // fire one round if supposed to be firing
  if(isFiring)
  {
    fireOnce();
    digitalWrite(buzzerPin,HIGH); // sound the buzzer
  } else {
    digitalWrite(buzzerPin,LOW);  // stop the buzzer
  }

}


// moves motor j anticlockwise
void anticlockwise(int j)
{
  for(int i = 0; i < 8; i++)
  {
    setOutput(i, j);
    delayMicroseconds(motorSpeed);
  }
}

// moves motor j clockwise
void clockwise(int j)
{
  for(int i = 7; i >= 0; i--)
  {
    setOutput(i, j);
    delayMicroseconds(motorSpeed);
  }
}

// writes control codes to corresponding motor pins to turn
void setOutput(int out, int j)
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(motors[j][i], bitRead(lookup[out], i));  
  }
}

// rotates trigger servo 90 degrees and back again
void fireOnce()
{
  trigServo.write(90);
  delay(100);
  trigServo.write(0); 
  delay(100);
}
