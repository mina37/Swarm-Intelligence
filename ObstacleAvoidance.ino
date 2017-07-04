#include <SPI.h>
#include <RF24.h>
#include <BlueSM.h>
//#include <colorsensor.h>
//#include <linetracker.h>
//#include <motordriver.h>
//#include <PinsDefinition.h>
//#include <RFnetwork.h>
//#include <ultrasonic.h>
//#include <Arduino_FreeRTOS.h>
#define BLUE 0
#define RED 1
#define GREEN 2
#define WHITE 3

#define rightUltraSonic   ultrasonic(0)
#define midUltraSonic   ultrasonic(1)
#define leftUltraSonic   ultrasonic(2)

#define defaultLPWM 190
#define defaultRPWM 190   //PWM that makes robot move st.

#define defaultLeftRotatePWM 200
#define defaultRightRotatePWM 190
#define distanceTOAvoid 30

#define reqTicks 3
#define stablePWM 180
#define deltaPWM 5

unsigned char _minimum(unsigned long x,unsigned long y,unsigned long z){
  if (x<y){
    if (x<z){
      return 0;
      }
      else{
        
        return 2;
      
        }

    }
    else {
      if (y<z){
        return 1;
        
        }
        else {
          return 2; 
          }
      }
  
  }



unsigned char minimum(unsigned char& x,unsigned char &y,unsigned char&z){
  if (x<y){
    if (x<z){
      return 0;
      }
      else{
        
        return 2;
      
        }

    }
    else {
      if (y<z){
        return 1;
        
        }
        else {
          return 2; 
          }
      }
  
  }

unsigned char maximum (unsigned char& a,unsigned char& b,unsigned char& c) //maximum should have type int.
{
   if(a>b)
   {
      if(a>c)
      {
         return 0;
      }
   }// You were missing a } here
   else if(b>=a ) //You had elsif, that doesn't mean anything
   {
      if(b>=c)
      {
         return 1; // return instead of cout
      }
   }
   else if(c>=b)
   {
     if(c>=a)
      {
        return 2;
      }
   }
}

void goStraight(){
    
    
unsigned long right_ticks=LineDetectorCounterRight;
unsigned long left_ticks=LineDetectorCounterLeft;
LineDetectorCounterRight=0;
LineDetectorCounterLeft=0;
//------------------------//
if (right_ticks>reqTicks) //check on ticks in each wheel
  {
    if (RPWM>stablePWM){ //check on PWM damping or acc
        RPWM=RPWM-deltaPWM+5; //damp
        
    }
        else if (RPWM<stablePWM){
            LPWM=LPWM+deltaPWM; //acc
        }
  }
  else if (right_ticks<reqTicks){
    if (RPWM>stablePWM) //check on PWM damping or acc
    {
        LPWM=LPWM-deltaPWM;//damp
    }
    else if (RPWM<stablePWM)
    {
        RPWM=RPWM+deltaPWM*2;//accelerate
    }
    
    }

if (left_ticks>reqTicks) //check on ticks in each wheel
  {
    if (LPWM>stablePWM){ //check on PWM damping or acc
        LPWM=LPWM-deltaPWM; //damp
        
    }
        else if (LPWM<stablePWM){
            RPWM=RPWM+deltaPWM*2; //acc
        }
  }
  else if (left_ticks<reqTicks){
    if (LPWM>stablePWM) //check on PWM damping or acc
    {
        RPWM=RPWM-deltaPWM;//damp
    }
    else if (LPWM<stablePWM)
    {
        LPWM=LPWM+deltaPWM;//accelerate
    }
    
    }
    if (LPWM<=160){LPWM=160;}
    if (LPWM>=255){LPWM=255;}
    if (RPWM<=160){RPWM=160;}
    if (RPWM>=255){RPWM=255;}
    MotorDriverLeft(LPWM,1);
    MotorDriverRight(RPWM,1);
    delay(200);
    
}

void avoidObstacles(){
  unsigned char  dist_l =leftUltraSonic;
  unsigned char dist_m=midUltraSonic;
  unsigned char dist_r=rightUltraSonic;
//Serial.print("dist_l : ");
//Serial.println(dist_l);
//Serial.print("dist_m : ");
//Serial.println(dist_m);
//Serial.print("dist_r : ");
//Serial.println(dist_r);

  
  /*unsigned char i = 0;*/
  if ((dist_l<distanceTOAvoid)||(dist_m<distanceTOAvoid) || (dist_r<distanceTOAvoid)){
    BrakeMotorLeft();
    BrakeMotorRight();
    int tmp=10;
      unsigned char maxReturned=maximum(dist_r,dist_m,dist_l);      
    
        if (maxReturned==0){ //right direction is good for robot now : 
          //MotorDriverLeft(190,1);
          MotorDriverLeft(defaultLeftRotatePWM,1);
          BrakeMotorRight();
          while(dist_l<distanceTOAvoid||dist_m<distanceTOAvoid){
            MotorDriverLeft(defaultLeftRotatePWM,1);
            dist_l =leftUltraSonic;
            dist_m=midUltraSonic;
            }
    MotorDriverLeft(defaultLPWM,1);
    MotorDriverRight(defaultRPWM,1);
    }
    else if (maxReturned==1){
      //middle direction is good for robot now : 
         MotorDriverLeft(defaultLPWM,1);
         MotorDriverRight(defaultRPWM,1);

      }
    else if (maxReturned==2){//left direction is good
          MotorDriverRight(defaultRightRotatePWM,1);
          Serial.print("now better direction is left !! ");
          BrakeMotorLeft();
          while(dist_r<distanceTOAvoid||dist_m<distanceTOAvoid){
            MotorDriverRight(defaultRightRotatePWM,1);
            dist_r =rightUltraSonic;
            dist_m=midUltraSonic;
            }
    MotorDriverLeft(defaultLPWM,1);
    MotorDriverRight(defaultRPWM,1);
    }
}

  
}

unsigned long right_ticks;
unsigned long left_ticks;
unsigned int LPWM;
unsigned int RPWM;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  init_motor_driver();
  //init_line_tracker();
  //init_color_sensor();
  init_ultrasonic();
  LPWM=160;
  RPWM=160;
  MotorDriverLeft(stablePWM,1);
  MotorDriverRight(stablePWM,1);
}

void followingAlgorithm(){
  while(true){
  unsigned char dist_l=leftUltraSonic;
  unsigned char dist_m=midUltraSonic;
  unsigned char dist_r=rightUltraSonic;
  if (minimum(dist_l,dist_m,dist_r)){
    
  }
}
}
void loop() {
  
  unsigned char counter=0;
  unsigned char i=0;
   for (;i<10;i++){
        if(_minimum(colorSensor((char)RED),colorSensor((char)GREEN),colorSensor((char)BLUE))==(unsigned char)BLUE)
      {
      counter++;
      }
    }
if (counter>=7)
  {
  followingAlgorithm();
  }
    
    }
