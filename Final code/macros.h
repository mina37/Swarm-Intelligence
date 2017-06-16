//conststants
#define SWITCHTASKSTACKSIZE        128  //Stack Size
#define MotorDriverLeft( pwm , dir){    digitalWrite( motorL1, LOW );    digitalWrite( motorR1, LOW );   switch (dir)  { case 1: analogWrite(motorL1, pwm);  break; case 0: analogWrite(motorR1, pwm);   break;  }}
#define MotorDriverRight( pwm , dir){    digitalWrite( motorL2, LOW );    digitalWrite( motorR2, LOW );   switch (dir)  { case 1: analogWrite(motorL2, pwm);  break; case 0: analogWrite(motorR2, pwm);   break;  }}
#define TICKS_PER_SECOND 55

#define ROTATION_90_TIME 2 * TICKS_PER_SECOND
int length_arena = 10 * TICKS_PER_SECOND;
int width_arena = 5* TICKS_PER_SECOND;

//pins used
#define ultraSonicEcho  A3 //Ultrasound signal pin
#define ultraSoundSignal  A0 // Ultrasound signal pin
#define s0  4
#define s1  3
#define s2  5
#define s3  6
#define out  2
#define motorL1 3
#define motorR1 5
#define motorL2 6
#define motorR2 9


//macros and enums
#define BLUE 0
#define RED 1
#define Green 2
#define WHITE 3


