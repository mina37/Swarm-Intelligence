#include <SPI.h>
#include <RF24.h>
#include <BlueSM.h>


#define BLUE 0
#define RED 1
#define GREEN 2
#define WHITE 3

#define rightUltraSonic   ultrasonic(0)
#define midUltraSonic   ultrasonic(1)
#define leftUltraSonic   ultrasonic(2)

#define defaultLPWM 170
#define defaultRPWM 170   //PWM that makes robot move st.

#define defaultLeftRotatePWM 160
#define defaultRightRotatePWM 160
#define distanceTOAvoid 30

#define reqTicks 3
#define stablePWM 180
#define deltaPWM 5

unsigned char dist_l      = 0;
unsigned char dist_m      = 0;
unsigned char dist_r      = 0;
unsigned char maxReturned = 0;

//----ggostraight vars :

unsigned long right_ticks;
unsigned long left_ticks;
unsigned int LPWM = 160;
unsigned int RPWM = 160;


unsigned char minimum(unsigned char& x, unsigned char &y, unsigned char &z) {
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


void vAvoidObstacles(void * param){
  for(;;)
  {
  dist_l = leftUltraSonic;
  dist_m = midUltraSonic;
  dist_r = rightUltraSonic;

  /* Obstacle at left */
  if (dist_l < distanceTOAvoid) {
    
    BrakeMotorRight();
    MotorDriverLeft(defaultLeftRotatePWM, 1);
    taskENTER_CRITICAL();
    while(dist_l < distanceTOAvoid || dist_m < distanceTOAvoid) {
      MotorDriverLeft(defaultLeftRotatePWM, 1);
      dist_l = leftUltraSonic;
      dist_m = midUltraSonic;
    }
    taskEXIT_CRITICAL();
  }

  /* Obstacle at right */
  else if(dist_r < distanceTOAvoid) {
    
    BrakeMotorLeft();
    MotorDriverRight(defaultRightRotatePWM,1);
    taskENTER_CRITICAL();
    while(dist_r < distanceTOAvoid || dist_m < distanceTOAvoid){
      MotorDriverRight(defaultRightRotatePWM, 1);
      dist_r = rightUltraSonic;
      dist_m = midUltraSonic;
    }
    taskEXIT_CRITICAL();
  }

  /* Obstacle at middle */
  else if (dist_m < distanceTOAvoid) {
    BrakeMotorRight();
    BrakeMotorLeft();
    taskENTER_CRITICAL();
    dist_l = leftUltraSonic;
    dist_r = rightUltraSonic;
    if( dist_l > dist_r) {
      MotorDriverRight(defaultRightRotatePWM,1);
      while(dist_m < distanceTOAvoid) {
        dist_m = midUltraSonic;
      }
      MotorDriverLeft(LPWM,1);
      MotorDriverRight(RPWM,1);     
    } else if(dist_l <= dist_r) {
        MotorDriverLeft(defaultLeftRotatePWM,1);
        while(dist_m < distanceTOAvoid) {
          dist_m = midUltraSonic;
        }
        MotorDriverLeft(LPWM, 1);
        MotorDriverRight(RPWM, 1);    
     }
     taskEXIT_CRITICAL();
  }
  else {
    MotorDriverLeft(LPWM,1);
    MotorDriverRight(RPWM,1);
  }

vTaskDelay(2);  
}
}


void vMoveStraight(void * param){


unsigned long prevRight = 0;
  unsigned long prevLeft = 0;
  MotorDriverRight(RPWM,1)
  
for(;;)
{
  signed int e = (LineDetectorCounterRight - prevRight) - (LineDetectorCounterLeft - prevLeft);
  LPWM = 160-(30*e);
    MotorDriverLeft(LPWM,1);
    prevRight = LineDetectorCounterRight;
    prevLeft = LineDetectorCounterLeft;
    vTaskDelay(10);
      
}


  /*----------------------------------*/
//  unsigned long right_ticks;
//  unsigned long left_ticks;
//  for(;;)
//  {
//    BrakeMotorLeft();
//    BrakeMotorRight();
//  right_ticks=LineDetectorCounterRight;
//  left_ticks=LineDetectorCounterLeft;
//LineDetectorCounterRight=0;
//LineDetectorCounterLeft=0;
////------------------------//
//if (right_ticks>reqTicks) //check on ticks in each wheel
//  {
//    if (RPWM>stablePWM){ //check on PWM damping or acc
//        RPWM=RPWM-deltaPWM; //damp
//        
//    }
//        else if (RPWM<stablePWM){
//            LPWM=LPWM+deltaPWM; //acc
//        }
//  }
//  else if (right_ticks<reqTicks){
//    if (RPWM>stablePWM) //check on PWM damping or acc
//    {
//        LPWM=LPWM-deltaPWM;//damp
//    }
//    else if (RPWM<stablePWM)
//    {
//        RPWM=RPWM+deltaPWM*2;//accelerate
//    }
//    
//    }
//
//if (left_ticks>reqTicks) //check on ticks in each wheel
//  {
//    if (LPWM>stablePWM){ //check on PWM damping or acc
//        LPWM=LPWM-deltaPWM; //damp
//        
//    }
//        else if (LPWM<stablePWM){
//            RPWM=RPWM+deltaPWM; //acc
//        }
//  }
//  else if (left_ticks<reqTicks){
//    if (LPWM>stablePWM) //check on PWM damping or acc
//    {
//        RPWM=RPWM-deltaPWM;//damp
//    }
//    else if (LPWM<stablePWM)
//    {
//        LPWM=LPWM+deltaPWM;//accelerate
//    }
//    
//    }
//    if (LPWM<=170){LPWM=170;}
//    if (LPWM>=220){LPWM=220;}
//    if (RPWM<=170){RPWM=170;}
//    if (RPWM>=220){RPWM=220;}
//    MotorDriverLeft(LPWM,1);
//    MotorDriverRight(RPWM,1);
//    vTaskDelay(18);
//  }
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  init_motor_driver();
  init_line_tracker();
  //init_color_sensor();
  init_ultrasonic();
  LPWM=160;
  RPWM=160;
  MotorDriverLeft(stablePWM,1);
  MotorDriverRight(stablePWM,1);
  TaskHandle_t  First_handle,Second_handle; //task handles and it is redundant and can be replaced with NULL as long as it is neede
  xTaskCreate(vAvoidObstacles, "Task1",128, NULL,3, &First_handle); //creating main task
  xTaskCreate(vMoveStraight, "Task2",128, NULL,2, &Second_handle); //creating main task
  //starting the scheduler
  vTaskStartScheduler();
}

void loop() {
  
   //avoidObstacles();
   //goStraight();

    }

