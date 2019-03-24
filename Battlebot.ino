#include <XBOXRECV.h>
 
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
 
USB Usb;
XBOXRECV Xbox(&Usb);
 
int ENABLE_A = 5;
int ENABLE_B = 3;
int in1 = 2;
int in2 = 4;
int in3 = 7;
int in4 = 8;
 
int bot_speed = 0;
int min_speed = 55;
 
void setup() {
  //5 -> Enable 1
  //2 -> In1
  //4 -> In2
  //7 -> In3
  //8 -> In4
  //3 -> Enable 2
  //Dont use pin 6 and 9!!!
  
  Serial.begin(115200); 
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  for(int i = 2;i<=8;i++){
    pinMode(i,OUTPUT);
  }
}
 
void turn(){
  if(Xbox.getAnalogHat(LeftHatX, 0)<-7500){
    int analogSpeed = map(Xbox.getAnalogHat(LeftHatX, 0),-7500,-32767,0,255);
    control_enables(bot_speed,bot_speed - analogSpeed);
  }else if(Xbox.getAnalogHat(LeftHatX, 0)>7500){
    int analogSpeed = map(Xbox.getAnalogHat(LeftHatX, 0),7500,32767,0,255);
    control_enables(bot_speed - analogSpeed,bot_speed);
  }else{
    control_enables(bot_speed,bot_speed);
    }
  }
 
void control_motor(int inner1, int inner2){
  digitalWrite(inner1,HIGH);
  digitalWrite(inner2,LOW);
}
 
void control_enables(int speed1, int speed2){
  if(speed1<min_speed){
    speed1 = 0;
  }
  if(speed2<min_speed){
    speed2 = 0;
  }
}

void loop() {
  Usb.Task();
  if(Xbox.XboxReceiverConnected){
    if(Xbox.Xbox360Connected[0]){
          int dir = 0;
          if(Xbox.getButtonPress(R2, 0)){  
            bot_speed = Xbox.getButtonPress(R2, 0);
            control_motor(in2,in1);
            control_motor(in3,in4);
            turn();
          }else if(Xbox.getButtonPress(L2, 0)){
            bot_speed = Xbox.getButtonPress(L2, 0);
            control_motor(in1,in2);
            control_motor(in4,in3);
            turn();
          }
   }
  }
}
 
