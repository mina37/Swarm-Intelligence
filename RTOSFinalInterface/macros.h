//conststants
#define SWITCHTASKSTACKSIZE        128  //Stack Size
#define MotorDriverLeft( pwm , dir){    digitalWrite( motorL1, LOW );    digitalWrite( motorR1, LOW );   switch (dir)  { case 1: analogWrite(motorL1, pwm);  break; case 0: analogWrite(motorR1, pwm);   break;  }}
#define MotorDriverRight( pwm , dir){    digitalWrite( motorL2, LOW );    digitalWrite( motorR2, LOW );   switch (dir)  { case 1: analogWrite(motorL2, pwm);  break; case 0: analogWrite(motorR2, pwm);   break;  }}
//1 tick = 17.5 ms


#define ROTATION_90_TIME 2 * TICKS_PER_SECOND



//pins used
#define ultraSonicEcho  A3 //Ultrasound signal pin
#define ultraSoundSignal  A0 // Ultrasound signal pin
//#define s0  4
//#define s1  3
#define s2  10
#define s3  12
#define out  4
#define motorL1 11
#define motorR1 5
#define motorL2 6
#define motorR2 9

#define LineDetectorPin_1 2
#define LineDetectorPin_2 3 
#define RPMCounter_1 LineDetectorCounter_1
#define RPMCounter_2 LineDetectorCounter_2



//macros and enums
#define BLUE 0
#define RED 1
#define Green 2
#define WHITE 3


