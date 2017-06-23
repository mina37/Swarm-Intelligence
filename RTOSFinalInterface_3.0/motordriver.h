#ifndef motor 
#define motor    


#define MotorDriverLeft( pwm , dir){    switch (dir)  { case 1: analogWrite(motorL1, 0); analogWrite(motorR1,pwm);  break; case 0: analogWrite(motorL1,  pwm); analogWrite(motorR1,0);   break;  }}
#define MotorDriverRight( pwm , dir){    switch (dir)  { case 1: analogWrite(motorL2, 255 - pwm); analogWrite(motorR2,255);  break; case 0: analogWrite(motorL2, pwm); analogWrite(motorR2,0);   break;  }}

#define BreakMotorLeft() digitalWrite( motorL1, HIGH );    digitalWrite( motorR1, HIGH )
#define BreakMotorRight() digitalWrite( motorL2, HIGH );    digitalWrite( motorR2, HIGH )

#define StopMotorLeft() digitalWrite( motorL1, LOW );    digitalWrite( motorR1, LOW )
#define StopMotorRight() digitalWrite( motorL2, LOW );    digitalWrite( motorR2, LOW )

#define motorL1 5 //11
#define motorR1 A6 //5
#define motorL2 6
#define motorR2 A7 //9


void init_motor_driver()
{
	pinMode(motorL1,OUTPUT);
	pinMode(motorR1,OUTPUT);
	pinMode(motorL2,OUTPUT);
	pinMode(motorR2,OUTPUT);
  BreakMotorLeft();
  BreakMotorRight();

}

#endif
