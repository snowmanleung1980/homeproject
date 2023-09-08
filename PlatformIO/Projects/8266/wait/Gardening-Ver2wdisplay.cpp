// Try to MQTT back the value
//Not test Yet


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// #include <WiFi.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid ="smart"; //修改為你家的WiFi網路名稱
const char* pass ="20130310"; //修改為你家的WiFi密碼
const char* mqtt_server = "ckrouter.synology.me";
const int mqtt_port = 1883;
const char* mqtt_user = "user";
const char* mqtt_pass = "20130310";
const char* mqtt_topic = "esp32";
const char* sub_topic = "esp32p";

WiFiClient espClient;
PubSubClient client(espClient);


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


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


void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
    void dispConnecting();
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi RSSI:");
  Serial.println(WiFi.RSSI()); //讀取WiFi強度
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishData(){

  DynamicJsonDocument doc(1024);

  String ssid = WiFi.SSID();
  int rssi = WiFi.RSSI();
  const char id[] = "esp01";


  doc["ID"] = id;
  doc["ssid"] = ssid;
  doc["rssi"] = rssi;

  char output[100];
  serializeJson(doc, output);

  // Publish JSON
  client.publish(mqtt_topic, output);
}


void setup() {
 Serial.begin(115200);
 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();

setup_wifi();

  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);

    while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {

      Serial.println("connected");  
      client.subscribe(sub_topic);
    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
    }





}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
    client.loop();

  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
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




  publishData();

/*
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(sensorValue);
  display.display(); 
*/

}
