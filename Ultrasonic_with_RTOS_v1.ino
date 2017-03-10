#include <Arduino_FreeRTOS.h>
#include "queue.h"
#include "task.h"

#define SWITCHTASKSTACKSIZE        128

int ultraSoundSignal = 7; // Ultrasound signal pin
int ultraSonicEcho = 6;
QueueHandle_t UltraSonicQ =  xQueueCreate( 10, sizeof(long) );;
bool UltrasonicZeroFlag = false;
unsigned int UltraSonicValue;

void MainTask(void * param)
{
  Serial.println("in main task");
  char zero = 0;
  for(int i = 0;i<10;i++)
  {
    xQueueSend( UltraSonicQ,( void * ) &zero, (TickType_t )0);
  }
  while(true)
  {
    if(!UltrasonicZeroFlag)
      Serial.println(UltraSonicValue);
      vTaskDelay(600);
  }
}

void UltrasonicTask(void * param)
{
  Serial.println("in ultrasonic task");
   pinMode(ultraSoundSignal,OUTPUT);
   pinMode(ultraSonicEcho,INPUT);
   digitalWrite(ultraSonicEcho, HIGH); // Turn on pullup resistor
   unsigned long echo;
  while(true){
    taskENTER_CRITICAL();
    digitalWrite(ultraSoundSignal, LOW); // Send low pulse 
    delayMicroseconds(2); // Wait for 2 microseconds
    digitalWrite(ultraSoundSignal, HIGH); // Send high pulse
    delayMicroseconds(5); // Wait for 5 microseconds
    digitalWrite(ultraSoundSignal, LOW); // Holdoff
    // please note that pulseIn has a 1sec timeout, which may
    // not be desirable. Depending on your sensor specs, you
    // can likely bound the time like this -- marcmerlin
    echo = pulseIn(ultraSonicEcho, HIGH); //Listen for echo
    taskEXIT_CRITICAL();
    if(echo != 0)
    {
      unsigned long ultrasoundValue;
      xQueueReceive( UltraSonicQ, &( ultrasoundValue ), (TickType_t)0);
      ultrasoundValue = (echo / 58.138); //convert to CM
      xQueueSend( UltraSonicQ,( void * )&ultrasoundValue , (TickType_t )100);
      ultrasoundValue = 0;
      for(char i = 0;i<10;i++)
      {
        unsigned int tmp;
        xQueueReceive( UltraSonicQ, &( tmp ), (TickType_t)20);
        ultrasoundValue = ultrasoundValue + tmp;
        xQueueSend( UltraSonicQ,( void * )&tmp , (TickType_t )100);
      }
      UltraSonicValue = ultrasoundValue/10;
      UltrasonicZeroFlag = false; 
     }
     else
     {
      UltrasonicZeroFlag = true;
     }
    vTaskDelay(50);
  }
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  TaskHandle_t  First_handle,Second_handle;
  
  xTaskCreate(UltrasonicTask, "Task1",SWITCHTASKSTACKSIZE, NULL,1, &First_handle);
  xTaskCreate(MainTask, "Task2",SWITCHTASKSTACKSIZE, NULL,2, &Second_handle);
  
  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
