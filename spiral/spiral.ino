#include "BlueSM.h"
#include "RF24.h"
#include <SPI.h>


#define WHEEL_D 65
#define WHEEL_L 99.25
#define TICKS_PER_REV 8.0
#define MILLIMETERS_PER_TICK (PI * WHEEL_D / TICKS_PER_REV)
#define DEG_PER_TICK ((360 * WHEEL_D) / (2 * WHEEL_L * TICKS_PER_REV))


void rotateAngle(int desiredDegrees, bool cw = true) {
  Serial.print("rotate ");
  unsigned int requiredTicks = round(desiredDegrees / DEG_PER_TICK);
  Serial.println(requiredTicks);
  unsigned int startTicks;
  unsigned int doneTicks = 0;
  if (cw) {
    startTicks = LineDetectorCounterRight;
    MotorDriverLeft(170 , 1);
    BrakeMotorRight();
  } else {
    startTicks = LineDetectorCounterLeft;
    MotorDriverRight(170 , 1);
    BrakeMotorLeft();
  }
  while(doneTicks < requiredTicks) {
    if(cw) {
      doneTicks = LineDetectorCounterRight - startTicks;      
    } else {
      doneTicks = LineDetectorCounterLeft - startTicks;            
    }
    Serial.println(doneTicks);
    delay(300);
  }
  BrakeMotorLeft();
  BrakeMotorRight();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init_motor_driver();
  init_line_tracker();

}

void loop() {
//  // put your main code here, to run repeatedly:
  long t1 = millis();
  while(millis() - t1 < 4000){
    unsigned long prevRight = 0;
    unsigned long prevLeft = 0;
    MotorDriverLeft(160,1);
    signed int e = (LineDetectorCounterRight - prevRight) - (LineDetectorCounterLeft - prevLeft);
    MotorDriverRight(160-(30*e),1)
    prevRight = LineDetectorCounterRight;
    prevLeft = LineDetectorCounterLeft;
    Serial.print("LEFT : ");
    Serial.println(LineDetectorCounterLeft);
    Serial.print("RIGHT : ");
    Serial.println(LineDetectorCounterRight);
  
  }
  BrakeMotorLeft();
  BrakeMotorRight();
  delay(500);
  LineDetectorCounterRight=0;
  LineDetectorCounterLeft=0;
 //rotateAngle(90,false);
}
