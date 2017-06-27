#include "macros.h"
void setup() {
  // put your setup code here, to run once:
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s1, HIGH);
  digitalWrite(s0, LOW);
Serial.begin(9600);
}
unsigned long colorSensor(char color)
{
  unsigned long counter = 0; //gets input from sensor frequency generator
  unsigned long countR = 0; //accumilates reading over 8 times to get average
  unsigned long count = 0; //saves the final average reading

  //gets 8 reading for each color
  for (char i = 0; i < 8; i++) {
    if (color == 1) {
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW); 
    }
    else if (color == 0) {
      digitalWrite(s2, LOW);
      digitalWrite(s3, HIGH);
    }
    else if (color == 2) {


      digitalWrite(s2, HIGH);
      digitalWrite(s3, HIGH);
    }
    else
    {
      digitalWrite(s2, HIGH);
      digitalWrite(s3, LOW);
    }

    counter = 0;  //resets counter
    //stops RTOS interrupt
    counter = pulseIn(out, LOW);  //reads the pulse width which is inversely proportional to the frequency
    //so the less the pulse width the more the frequency and the more the intensity

    countR = countR + counter;

  }
  //Serial.print("red=");
  
  
  ///////////////////////////
 // countR = countR >> 7;
  //count = (unsigned char) countR;
//////////////////

  count = countR>>1;
  //Serial.println(countR,DEC);
  return count;
}

void loop() {
  // put your main code here, to run repeatedly:
for(char i = 0;i<3;i++)
{
  Serial.print(i,DEC);
  Serial.print(" : ");
  Serial.println(colorSensor(i),DEC);
  delay(500);

}
}
