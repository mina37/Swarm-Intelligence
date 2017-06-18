#include <Arduino_FreeRTOS.h>
#include "task.h"
#include "macros.h"
long LineDetectorCounter_1 = 0 ;
long LineDetectorCounter_2 = 0 ;

void LineDetectorFunction_1()
{
 LineDetectorCounter_1++;
 }
void LineDetectorFunction_2()
{
 LineDetectorCounter_2++;
 }


unsigned long ultrasonic(char i)
{
  unsigned long ultrasoundValue;
    pinMode(ultraSoundSignal + i, OUTPUT); //set the signal pin to OUTPUT
    pinMode(ultraSonicEcho + i, INPUT);    //sets the echo pin to INPUT
    digitalWrite(ultraSonicEcho + i, HIGH); // Turn on pullup resistor
  unsigned long echo = 0;                  //duration of the echo pulse
      while(echo ==0){
      digitalWrite(ultraSoundSignal + i, LOW); // Send low pulse
      delayMicroseconds(2); // Wait for 2 microseconds
      digitalWrite(ultraSoundSignal + i, HIGH); // Send high pulse
      delayMicroseconds(5); // Wait for 5 microseconds
      digitalWrite(ultraSoundSignal + i, LOW); // Holdoff
      // please note that pulseIn has a 1sec timeout, which may
      // not be desirable. Depending on your sensor specs, you
      // can likely bound the time like this -- marcmerlin
      echo = pulseIn(ultraSonicEcho + i, HIGH); //Listen for echo
                   //enables the scheduler
      //gets the average of last 10 non zero readings
          //ultrasound value
        //xQueueReceive( UltraSonicQ, &( ultrasoundValue ), (TickType_t)0); //discards oldest of the the last 10 readings
        ultrasoundValue = (echo / 58.138); //convert to CM
      }
      if (ultrasoundValue < 300)
        {
          return ultrasoundValue;
        }
  else
  return 0;
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


int counter = 0;
void MainTask(void * param)
{
  for(;;)
  {
    for(char i =0;i<3;i++)
  {
    Serial.print("ultrasonic: ");
    Serial.println(i,DEC);
    Serial.println(ultrasonic(i));
  }
  //for testing 
Serial.print("COUNTER_1 = ");
Serial.println(RPMCounter_1);
Serial.print("COUNTER_2 = ");
Serial.println(RPMCounter_2);
for(char i = 0;i<4;i++)
{
  Serial.print("ColorSensor: ");
  Serial.println(i,DEC);
  Serial.println(colorSensor(i),DEC);

}
MotorDriverLeft(200,1);
MotorDriverRight(200,1);
vTaskDelay(200);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode( LineDetectorPin_1 , INPUT );
pinMode( LineDetectorPin_2 , INPUT );
pinMode(LineDetectorPin_1 , INPUT_PULLUP);
pinMode(LineDetectorPin_2 , INPUT_PULLUP);
attachInterrupt ( digitalPinToInterrupt(LineDetectorPin_1) , LineDetectorFunction_1 , FALLING);
attachInterrupt ( digitalPinToInterrupt(LineDetectorPin_2) , LineDetectorFunction_2 , RISING);

  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  Serial.begin(9600); //initialises the serial for debugging
  TaskHandle_t  First_handle; //task handles and it is redundant and can be replaced with NULL as long as it is neede
  xTaskCreate(MainTask, "Task2",128, NULL,1, &First_handle); //creating main task
  //starting the scheduler
  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
