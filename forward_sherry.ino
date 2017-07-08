#include "BlueSM.h"
#include "RF24.h"
#include <SPI.h>

void setup() 
{
 init_motor_driver();
 init_line_tracker();
}

void loop()
{
 
 unsigned long prevRight = 0;
 unsigned long prevLeft = 0;
 MotorDriverLeft(160,1);
 

 signed int e = (LineDetectorCounterRight - prevRight) - (LineDetectorCounterLeft - prevLeft);
 MotorDriverRight(160-(30*e),1)
 prevRight = LineDetectorCounterRight;
 prevLeft = LineDetectorCounterLeft;

}
