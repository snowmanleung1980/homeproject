// Display thirsty, OK and full on OLED Display

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>




#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*
void blinkdry(){

  for(int i = 0; i< 5; i++){

  display.clearDisplay();
  display.fillScreen(WHITE);
  display.display(); 
  delay(200);
  display.fillScreen(BLACK);
  display.display(); 
  delay(200);
   }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.printf("I'm thirsty");
  display.display(); 
  delay(1000);
}


void blinkfull(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.printf("I'm FULL");
  display.display(); 
  delay(1000);
}
*/

void setup() {
 Serial.begin(115200);
 
/*
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  */
}

void loop() {

  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);

  delay(1000);
    /*
    if (sensorValue > 480){     //550 before
         blinkdry();
        }else if (sensorValue <400) //350 before
        {
          blinkfull();
        }  else {     
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.printf("I'm OK ^^");
  display.display(); 
  delay(1000);
  display.clearDisplay();
        }



  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(sensorValue);
  display.display(); 
*/

}
