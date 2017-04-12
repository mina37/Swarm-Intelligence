#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "task.h"
#include "macros.h"

QueueHandle_t UltraSonicQ =  xQueueCreate( 10, sizeof(int) ); //Queue holding last 10 readings of the Ultrasonic sensor
bool UltrasonicZeroFlag[3];  //states weather the last reading of the ultrasonic sensor was zero or not
                                  // as it gives fake value of zero a lot
QueueHandle_t UltraSonicValue =  xQueueCreate( 3, sizeof(int) );   //actual normalized reading of the ultrasonic

//main task
void MainTask(void * param)
{
  Serial.println("in main task"); //for debugging
  for(char i = 0;i<3;i++)
  {
    UltrasonicZeroFlag[i] = false;
    //UltraSonicQ[i] = xQueueCreate( 10, sizeof(int) );
  }
  Serial.println("queue");
  for(char i = 0;i<10;i++)
  {
    xQueueSend( UltraSonicQ,( void * ) NULL, (TickType_t )0);      //fills the queue initially with zeros
  }
  unsigned int UltraSonic;
  Serial.println("before while");
  char i = 2;
  while(true)
  {
    i = (i+1)%3;
    vTaskDelay(60);
    if(!UltrasonicZeroFlag[i])
    {
      Serial.print(i,DEC);
    xQueueReceive(UltraSonicValue,&UltraSonic,(TickType_t )1000);
      Serial.println(UltraSonic);
    }
          //blocks current task for 60 ticks
  }
}

void UltrasonicTask(void * param) //task that gets the reading of the ultrasonic and normalizes it
{
  Serial.println("in ultrasonic task"); //for debugging
  for(char i=0;i<3;i++)
  {
   pinMode(ultraSoundSignal+i,OUTPUT);    //set the signal pin to OUTPUT
   pinMode(ultraSonicEcho+i,INPUT);       //sets the echo pin to INPUT
   digitalWrite(ultraSonicEcho+i, HIGH);  // Turn on pullup resistor 
  }
   unsigned long echo;                  //duration of the echo pulse
  while(true){
    for(char i = 0;i<3;i++)
    {
    taskENTER_CRITICAL();               //disables the scheduler
    digitalWrite(ultraSoundSignal+i, LOW); // Send low pulse 
    delayMicroseconds(2); // Wait for 2 microseconds
    digitalWrite(ultraSoundSignal+i, HIGH); // Send high pulse
    delayMicroseconds(5); // Wait for 5 microseconds
    digitalWrite(ultraSoundSignal+i, LOW); // Holdoff
    // please note that pulseIn has a 1sec timeout, which may
    // not be desirable. Depending on your sensor specs, you
    // can likely bound the time like this -- marcmerlin
    echo = pulseIn(ultraSonicEcho+i, HIGH); //Listen for echo
//if(i==0)
//{
//      digitalWrite(A3, LOW); // Send low pulse 
//    delayMicroseconds(2); // Wait for 2 microseconds
//    digitalWrite(A3, HIGH); // Send high pulse
//    delayMicroseconds(5); // Wait for 5 microseconds
//    digitalWrite(A3, LOW); // Holdoff
//    // please note that pulseIn has a 1sec timeout, which may
//    // not be desirable. Depending on your sensor specs, you
//    // can likely bound the time like this -- marcmerlin
//    echo = pulseIn(A0, HIGH); //Listen for echo
//
//}
//else if(i==1)
//{
//  digitalWrite(A4, LOW); // Send low pulse 
//    delayMicroseconds(2); // Wait for 2 microseconds
//    digitalWrite(A4, HIGH); // Send high pulse
//    delayMicroseconds(5); // Wait for 5 microseconds
//    digitalWrite(A4, LOW); // Holdoff
//    // please note that pulseIn has a 1sec timeout, which may
//    // not be desirable. Depending on your sensor specs, you
//    // can likely bound the time like this -- marcmerlin
//    echo = pulseIn(A1, HIGH); //Listen for echo
//
//}
//else if(i==2)
//{
//  digitalWrite(A5, LOW); // Send low pulse 
//    delayMicroseconds(2); // Wait for 2 microseconds
//    digitalWrite(A5, HIGH); // Send high pulse
//    delayMicroseconds(5); // Wait for 5 microseconds
//    digitalWrite(A5, LOW); // Holdoff
//    // please note that pulseIn has a 1sec timeout, which may
//    // not be desirable. Depending on your sensor specs, you
//    // can likely bound the time like this -- marcmerlin
//    echo = pulseIn(A2, HIGH); //Listen for echo
//
//}
    taskEXIT_CRITICAL();              //enables the scheduler
    //gets the average of last 10 non zero readings
    if(echo != 0) //discards zero reading
    {
      unsigned long ultrasoundValue;  //ultrasound value
      //xQueueReceive( UltraSonicQ, &( ultrasoundValue ), (TickType_t)0); //discards oldest of the the last 10 readings
      ultrasoundValue = (echo / 58.138); //convert to CM
      if(ultrasoundValue<300)
      {
        
      
      //xQueueSend( UltraSonicQ,( void * )&ultrasoundValue , (TickType_t )100); //adds the newest reading to the queue
      //ultrasoundValue = 0;  //resets the variable
      //gets the average of the last 10 readings
      //for(char i = 0;i<10;i++)
      //{
       // unsigned int tmp; //temproray variable
      //  xQueueReceive( UltraSonicQ, &( tmp ), (TickType_t)20);      //sets tmp to the oldest of the last 10 readings
       // ultrasoundValue = ultrasoundValue + tmp;                    //adds tmp to the sum of values so far
      //  xQueueSend( UltraSonicQ,( void * )&tmp , (TickType_t )100); //adds the value in temp to the back of the queue
      //}
      //ultrasoundValue = ultrasoundValue/10;  //gets the avearge of the last 10 values
      xQueueSend(UltraSonicValue,(void*)&ultrasoundValue,(TickType_t )1000);
      UltrasonicZeroFlag[i] = false;     //gets the zero flag down stating that the last reading was not zero
      }
      else
      {
        UltrasonicZeroFlag[i] = true;
      }
     }
     else
     {
      UltrasonicZeroFlag[i] = true;                                    //raises the zero flag
     }
    vTaskDelay(5);
    }//blocks the task for 5 ticks
  }
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //initialises the serial for debugging
  TaskHandle_t  First_handle,Second_handle; //task handles and it is redundant and can be replaced with NULL as long as it is needed
  
  xTaskCreate(UltrasonicTask, "Task1",128, NULL,1, &First_handle);  //creating ultrasonic task
  xTaskCreate(MainTask, "Task2",128, NULL,2, &Second_handle); //creating main task
  //starting the scheduler
  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
