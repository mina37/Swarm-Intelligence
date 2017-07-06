#include <BlueSM.h>

/*** Macros ***/
#define rightUltraSonic       ultrasonic(0)
#define midUltraSonic         ultrasonic(2)
#define leftUltraSonic        ultrasonic(1)

#define defaultLPWM           170
#define defaultRPWM           170   //PWM that makes robot move st.

#define defaultLeftRotatePWM  190
#define defaultRightRotatePWM 190
#define distanceTOAvoid       30
#define reqTicks 3
#define stablePWM 180
#define deltaPWM 5
#define distanceToFollow 30
#define BLUE 0
#define RED 1
#define GREEN 2
#define WHITE 3

/*** Globals ***/
//unsigned char dist_l      = 0;
//unsigned char dist_m      = 0;
//unsigned char dist_r      = 0;
//unsigned char maxReturned = 0;

//for st Motion : 
unsigned long right_ticks;
unsigned long left_ticks;
unsigned int LPWM;
unsigned int RPWM;

unsigned char cminimum(unsigned char& x, unsigned char &y, unsigned char &z) {
  if (x < y) {
    if (x < z) return 0; 
    else return 2;
  } else {
    if (y < z) return 1;  
    else return 2;     
  }  
}


unsigned char lminimum(unsigned long& x, unsigned long &y, unsigned long &z) {
  if (x < y) {
    if (x < z) return 0; 
    else return 2;
  } else {
    if (y < z) return 1;  
    else return 2;     
  }  
}



unsigned char maximum (unsigned char& x,unsigned char& y,unsigned char& z) {
  if (x > y) {
    if (x > z) return 0; 
    else return 2;
  } else {
    if (y > z) return 1;  
    else return 2;     
  } 
}

void goStraight(){
  delay(100);
  right_ticks=LineDetectorCounterRight;
  left_ticks=LineDetectorCounterLeft;
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
    
}
void setup() {
  // Serial.begin(9600);
  init_motor_driver();
  
  //init_line_tracker();
  //init_color_sensor();
//  
   init_ultrasonic();
  MotorDriverLeft(180,1);
  MotorDriverRight(180,1);
}


unsigned long dist_m;
unsigned long dist_l;
unsigned long dist_r;
unsigned char minReturned;
void loop() {
  dist_l=leftUltraSonic;
  dist_m=midUltraSonic;
  dist_r=rightUltraSonic;
  minReturned=lminimum(dist_l,dist_m,dist_r);
 // Serial.print("Min : ");
  //Serial.println(minReturned);
  /*acc alg to follow head ina constant distance */ 
  if (minReturned==1){
   if (dist_m<30){
     MotorDriverLeft(180,1);
     MotorDriverRight(180,1); 
     //goStraight(); 
     
  }
  else{
//    BrakeMotorLeft();
//    BrakeMotorRight();
     MotorDriverLeft(160,1);
     MotorDriverRight(160,1); 
  }
  
  
  }
  else if (minReturned==2){
    //while(lminimum(dist_l,dist_m,dist_r)!=1){//go right
      MotorDriverLeft(defaultLeftRotatePWM,1);
      BrakeMotorRight();
      //MotorDriverLeft(150,1);
        dist_l=leftUltraSonic;
        dist_m=midUltraSonic;
        dist_r=rightUltraSonic;
    //} 

     MotorDriverLeft(180,1);
     MotorDriverRight(180,1); 
}
  else if (minReturned==0){
     // while(lminimum(dist_l,dist_m,dist_r)!=1){
      MotorDriverRight(190,1);
      MotorDriverLeft(150,1);
      BrakeMotorLeft();
        dist_l=leftUltraSonic;
        dist_m=midUltraSonic;
        dist_r=rightUltraSonic;
    //}
  }
     MotorDriverLeft(180,1);
     MotorDriverRight(180,1);
}

