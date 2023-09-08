#include <Arduino.h> 

#include <TinyGPSPlus.h>      //GPS
#include <SoftwareSerial.h>   //GPS

#include <Adafruit_GFX.h>


//GPS---
static const int RXPin = 18, TXPin = 19;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
TinyGPSCustom pdop(gps, "GPGSA", 15); // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GPGSA", 16); // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17); // $GPGSA sentence, 17th element
//GPS---




// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only one Task is accessing this resource at any time.
SemaphoreHandle_t xSerialSemaphore;
                                            
// define two Tasks for DigitalRead & AnalogRead

/*
void TaskGPSRead( void *pvParameters ) {
 char temp;

while (1) {

	// Every time anything is updated, print everything.
	if (gps.altitude.isUpdated() || gps.satellites.isUpdated() || pdop.isUpdated() || hdop.isUpdated() || vdop.isUpdated()) {
		Serial.print(F("ALT="));   Serial.print(gps.altitude.meters()); 
		Serial.print(F(" PDOP=")); Serial.print(pdop.value()); 
		Serial.print(F(" HDOP=")); Serial.print(hdop.value()); 
		Serial.print(F(" VDOP=")); Serial.print(vdop.value());
		Serial.print(F(" SATS=")); Serial.println(gps.satellites.value());
	}

if (ss.available() > 0) {
	
  temp=ss.read();
  gps.encode(temp);
  Serial.println(temp);

} else {
 Serial.println("Checking Wiring...");
    }

  }
}
*/

void TaskHelloworld1(void *pvParameters ) {

while (1) {
 
  Serial.println("Hello World_1");
  vTaskDelay( pdMS_TO_TICKS(250) );
  yield();   // 讓出資源
}
}


void TaskHelloworld2( void *pvParameters ) {

while (1) {
 
  Serial.println("Hello World");
  vTaskDelay( pdMS_TO_TICKS(250) );
  yield();   // 讓出資源
}
}


void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
 // ss.begin(GPSBaud);


  // Now set up two Tasks to run independently.
  xTaskCreate(
        TaskHelloworld1
    ,  "Helloworld1"  // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL //Parameters for the task
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL ); //Task Handle

  xTaskCreate(
    TaskHelloworld2
    ,  "Helloworld2" // A name just for humans
    ,  1024  // Stack size
    ,  NULL //Parameters for the task
    ,  2  // Priority
    ,  NULL ); //Task Handle


vTaskStartScheduler();
  // Now the Task scheduler, which takes over control of scheduling individual Tasks, is automatically started.
}


void loop()
{
  // Empty. Things are done in Tasks.
}


/*

void TaskDigitalRead( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
  
  // digital pin 2 has a pushbutton attached to it. Give it a name:
  uint8_t pushButton = 2;

  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    // read the input pin:
    int buttonState = digitalRead(pushButton);

    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
      // We were able to obtain or "Take" the semaphore and can now access the shared resource.
      // We want to have the Serial Port for us alone, as it takes some time to print,
      // so we don't want it getting stolen during the middle of a conversion.
      // print out the state of the button:
      Serial.println(buttonState);

      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

void TaskAnalogRead( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

  for (;;)
  {
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);

    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
      // We were able to obtain or "Take" the semaphore and can now access the shared resource.
      // We want to have the Serial Port for us alone, as it takes some time to print,
      // so we don't want it getting stolen during the middle of a conversion.
      // print out the value you read:
      Serial.println(sensorValue);

      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}

*/