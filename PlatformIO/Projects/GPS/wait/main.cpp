#include <Arduino.h> 
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

char* r0="Row1";
char* r1="Row2";
char* r2="Row3";
char* r3="Row4";
char* r4="Row5";
char* r5="Row6";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void TaskHelloworld1(void *pvParam ) {

while (1) {
 
  Serial.println("Hello World_1");
  vTaskDelay( pdMS_TO_TICKS(250) );
  yield();   // 讓出資源
}
}


void TaskHelloworld2( void *pvParam ) {

while (1) {
 
  Serial.println("Hello World_2");
  vTaskDelay( pdMS_TO_TICKS(250) );
  yield();   // 讓出資源
}
}





void oledDisp( void *pvParam ) {

while (1) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(r0);
  display.setCursor(0, 10);
  display.println(r1);
  display.setCursor(0, 20);
  display.println(r2);
  display.setCursor(0, 30);
  display.println(r3);
  display.setCursor(0, 40);
  display.println(r4);
  display.setCursor(0, 50);
  display.println(r5);
  display.display();
  
  vTaskDelay( pdMS_TO_TICKS(1000) );
  yield();   // 讓出資源
}
}



void setup() {

  Serial.begin(115200);

  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();










  // Now set up two Tasks to run independently.
  xTaskCreate(
        TaskHelloworld1
    ,  "Helloworld1"  // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL //Parameters for the task
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL ); //Task Handle

  xTaskCreate(
    TaskHelloworld2
    ,  "Helloworld2" // A name just for humans
    ,  1024  // Stack size
    ,  NULL //Parameters for the task
    ,  2  // Priority
    ,  NULL ); //Task Handle

  xTaskCreate(
    oledDisp
    ,  "oledDisp" // A name just for humans
    ,  1024  // Stack size
    ,  NULL //Parameters for the task
    ,  1  // Priority
    ,  NULL ); //Task Handle





}


void loop()
{
  // Empty. Things are done in Tasks.
}

