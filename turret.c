#include <Servo.h>

/*int motorPin1[2] = {8,3};    // Blue   - 28BYJ48 pin 1
int motorPin2[2] = 9;    // Pink   - 28BYJ48 pin 2
int motorPin3[2] = 10;    // Yellow - 28BYJ48 pin 3
int motorPin4[2] = 11;    // Orange - 28BYJ48 pin 4
                        // Red    - 28BYJ48 pin 5 (VCC) */

int motor_pins[8] = {8,3,9,4,10,5,11,6};
int motors[2][4] = {{8,9,10,11},{3,4,5,6}};


int s_pin = 7;
int sensor = 2;
int buzz=12;

bool fire = false;
int motorSpeed = 1200;  //variable to set stepper speed
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
//pins of different directions
int sw = A0; //SW
int horz = A1; //VRX
int vert = A2; //VRY


//value of currents for different directions
int swValue = 0;  
int horzValue = 0;
int vertValue = 0;


Servo s;
void setup() {
    // ouput pins
  s.attach(s_pin);
  s.write(0);
  /*pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(m21, OUTPUT);
  pinMode(m22, OUTPUT);
  pinMode(m23, OUTPUT);
  pinMode(m24, OUTPUT);*/
  
  for(int i = 0; i < 4; i++){
      for(int j = 0; j < 2; j++){
        pinMode(motors[j][i], OUTPUT);  
      }
  }

  pinMode(sensor,INPUT);
  pinMode(buzz,OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(2),Sw,RISING);
  Serial.begin(9600);
}

void loop() {
  
  // read the value from the sensor:
  swValue = analogRead(sw);
  horzValue = analogRead(horz);
  vertValue = analogRead(vert);

//if the switch is pressed
////////////////////////////////////////

  if(digitalRead(sensor) == HIGH){
    fire = true;
    
  }else if(digitalRead(sensor) == LOW){
    fire=false;  
  }
  
  if(swValue==0){
    fire=true;
  }


 //calls horizontal function
 /////////////////////////////////////////
  if(horzValue<200){
    anticlockwise(1);
  }else if(horzValue>800){
    clockwise(1);
  }
  
  //Calls the veritcal function
  ////////////////////////////////////
  if(vertValue>800){//moves up
    anticlockwise(0);
  }else if(vertValue<200){//moves down
   clockwise(0);
  }
  ////////////////////////////////////
 // Serial.println(horzValue);
  if(fire == true){
    Sw();
    delay(400);  
    digitalWrite(buzz,HIGH);
  }else{
    digitalWrite(buzz,LOW); 
  }
  s.write(0);
}

/*void Horizontal(bool d)
{

  //true as arg - knob moved right
  //false as arg - knob moved left
  if(d)
  {
     digitalWrite(left,HIGH);
     digitalWrite(right,LOW);
  }else
  {
     digitalWrite(left,LOW);
     digitalWrite(right,HIGH);
  }
}

void Vertical(bool d)
{
  //true as arg - knob moved up
  //false as arg - knob moved down
  if(d)
  {
     digitalWrite(up,HIGH);
      digitalWrite(down,LOW);
  }else
  { digitalWrite(up,LOW);
   digitalWrite(down,HIGH);
    
  }
}
*/
void anticlockwise(int j)
{
  for(int i = 0; i < 8; i++)
  {
    setOutput(i, j);
    delayMicroseconds(motorSpeed);
  }
}

void clockwise(int j)
{
  for(int i = 7; i >= 0; i--)
  {
    setOutput(i, j);
    delayMicroseconds(motorSpeed);
  }
}

void setOutput(int out, int j)
{
  for(int i = 0; i < 4; i++){
    digitalWrite(motors[j][i], bitRead(lookup[out], i));  
  }
  /*digitalWrite(motorPin1, bitRead(lookup[out], 0));
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));*/
}
void Sw()
{
  Serial.println("cool");
  s.write(90);
  delay(100);
  s.write(0);
  
  
}
