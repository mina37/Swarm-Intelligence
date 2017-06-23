#include <Arduino_FreeRTOS.h>
#include "task.h"
#include "macros.h"
#include "ultrasonic.h"
#include "colorsensor.h"
#include "motordriver.h"
#include "linetracker.h"
#include "rfrtos.h"





unsigned long ultrasonic(char i);

unsigned long colorSensor(char color);



// START ALGO
#define WHEEL_R 0.0325
#define WHEEL_L 0.09925
#define METERS_PER_TICK (2*PI*WHEEL_R)
float pose_x = 0;
float pose_y = 0;
float pose_phi = PI / 2;
int l_dir;
int r_dir;
unsigned long int prev_left_ticks = 0; 
unsigned long int prev_right_ticks = 0; 
unsigned long int delta_left; 
unsigned long int delta_right;
float d_left, d_right, d_center, d_phi; 
void update_odometry()
{
  delta_left = RPMCounter_1 - prev_left_ticks;
  delta_right = RPMCounter_2 - prev_right_ticks;
  prev_left_ticks = RPMCounter_1;
  prev_right_ticks = RPMCounter_2;
  d_left = l_dir * delta_left * METERS_PER_TICK;
  d_right = r_dir * delta_right * METERS_PER_TICK;
  d_center = (d_right + d_left) / 2.0;
  d_phi = (d_right - d_left) / WHEEL_L;
  pose_x += d_center * cos(pose_phi);
  pose_y += d_center * sin(pose_phi);
  pose_phi = atan2(sin(pose_phi + d_phi), cos(pose_phi + d_phi));
}
// END ALGO
int counter = 0;
void MainTask(void * param)
{
  /*l_dir = 1;
  r_dir = 1;
  MotorDriverLeft(255,0);
  MotorDriverRight(250,0);*/
  #if(VERBOSE == 1)
    Serial.println("Main Task");
  #endif
  for (;;) {
    /***************** Change Roles via Serial Commands *********************/
    if ( Serial.available()) {
      char c = toupper(Serial.read());
      if ( c == 'T' && role == 0 ) {
        #if (VERBOSE == 1)      
          Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
        #endif
        
        /* change role and switch reading and writing pipes */
        role = 1;               
        radio.closeReadingPipe(1);
        radio.openWritingPipe(addresses[0]);
        radio.openReadingPipe(1,addresses[1]);
  
        /* Suspend receiving task and resume sending one */
        vTaskSuspend(xRecvFrame);
        vTaskResume(xSendFrame);
        
      } else
      if ( c == 'R' && role == 1 ) {
        #if (VERBOSE == 1)
          Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
        #endif
        
        /* change role and switch reading and writing pipes */
        role = 0;
        radio.closeReadingPipe(1);
        radio.openWritingPipe(addresses[1]);
        radio.openReadingPipe(1,addresses[0]);
  
        /* Suspend sending task and resume receiving one */
        vTaskSuspend(xSendFrame);
        vTaskResume(xRecvFrame);

        /* start listening for data */
        radio.startListening();
      }
    }
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
MotorDriverLeft(250,1);
MotorDriverRight(250,0);
vTaskDelay(150);
  }
}

void setup() {
  // put your setup code here, to run once:
  init_color_sensor();
  init_motor_driver();
  init_line_tracker();
  init_ultrasonic();
  init_rf();

  Serial.begin(9600); //initialises the serial for debugging
  TaskHandle_t  First_handle; //task handles and it is redundant and can be replaced with NULL as long as it is neede
  xTaskCreate(MainTask, "Task2",128, NULL,1, &First_handle); //creating main task
  //starting the scheduler
  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
