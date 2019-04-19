#include <XBOXRECV.h>
 
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
 
USB Usb;
XBOXRECV Xbox(&Usb);
 
int ENABLE_A = 6;
int ENABLE_B = 9;
int in1 = 2;
int in2 = 3;
int in3 = 4;
int in4 = 5;
 
int bot_speed = 0;
int min_speed = 55;

void setupPins(){
  for(int i = 2;i<=5;i++){
    pinMode(i,OUTPUT);
  }
  pinMode(ENABLE_A,OUTPUT);
  pinMode(ENABLE_B,OUTPUT); 
}
 
void setup() { 
  Serial.begin(9600);
  
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }

  setupPins();
}

 
void loop() {
  Usb.Task();
  if(Xbox.XboxReceiverConnected){
    if(Xbox.Xbox360Connected[0]){
      if(Xbox.getButtonPress(R2,0)){  
        control_motor(in2,in1); //right motor forward
        control_motor(in4,in3); //left motor forward
        bot_speed = Xbox.getButtonPress(R2,0);
        turn();
      }else if(Xbox.getButtonPress(L2,0)){
        control_motor(in1,in2); //right motor backwards
        control_motor(in3,in4); //left motor backwards
        bot_speed = Xbox.getButtonPress(L2,0);
        turn();
      }else{
        turnOffMotor('r');
        turnOffMotor('l');
      }
    }
    else{
      turnOffMotor('r');
      turnOffMotor('l');
    }
  }
}

void turnOffMotor(char motor){
  if(motor == 'r'){
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);  
    analogWrite(ENABLE_A,0);
  }else if(motor == 'l'){
    digitalWrite(in3,LOW);
    digitalWrite(in4,LOW);
    analogWrite(ENABLE_B,0);
  }
}
 
void turn(){
  if(Xbox.getAnalogHat(LeftHatX,0)<-7500){
    //turning left
    int analogSpeed = map(Xbox.getAnalogHat(LeftHatX,0),-7500,-32767,0,255);
    control_enables(bot_speed,bot_speed - analogSpeed);
  }else if(Xbox.getAnalogHat(LeftHatX,0)>7500){
    //turning right
    int analogSpeed = map(Xbox.getAnalogHat(LeftHatX,0),7500,32767,0,255);
    control_enables(bot_speed-analogSpeed,bot_speed);
  }else{
    control_enables(bot_speed,bot_speed);
  }
}
 
void control_motor(int inner1, int inner2){
  digitalWrite(inner1,HIGH);
  digitalWrite(inner2,LOW);
}
 
void control_enables(int speed1, int speed2){
  if(speed1<=min_speed){
    turnOffMotor('r');
  }else{
    analogWrite(ENABLE_A,speed1);    
  }
  
  if(speed2<=min_speed){
    turnOffMotor('l');  
  }else{
    analogWrite(ENABLE_B,speed2); 
  }
}
