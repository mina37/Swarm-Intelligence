unsigned long echo = 0;
int ultraSoundSignal = 7; // Ultrasound signal pin
int ultraSonicEcho = 6;
unsigned long ultrasoundValue = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(ultraSoundSignal,OUTPUT);
  pinMode(ultraSonicEcho,INPUT);
}

unsigned long ping()
{ 
  //pinMode(ultraSoundSignal, OUTPUT); // Switch signalpin to output
  digitalWrite(ultraSoundSignal, LOW); // Send low pulse 
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(ultraSoundSignal, HIGH); // Send high pulse
  delayMicroseconds(5); // Wait for 5 microseconds
  digitalWrite(ultraSoundSignal, LOW); // Holdoff
  //pinMode(ultraSoundSignal, INPUT); // Switch signalpin to input
  digitalWrite(ultraSonicEcho, HIGH); // Turn on pullup resistor
  // please note that pulseIn has a 1sec timeout, which may
  // not be desirable. Depending on your sensor specs, you
  // can likely bound the time like this -- marcmerlin
  // echo = pulseIn(ultraSoundSignal, HIGH, 38000)
  echo = pulseIn(ultraSonicEcho, HIGH); //Listen for echo
  ultrasoundValue = (echo / 58.138); //convert to CM then to inches
  return ultrasoundValue;
}

void loop()
{
  unsigned int x = 0;
  x = ping();
  if(x!=0) //due to a problem that cause it to read "0" a lot
    Serial.println(x);
  delay(400); //delay 1/4 seconds.
}
