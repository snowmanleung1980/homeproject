#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

const char* ssid ="iot"; //修改為你家的WiFi網路名稱
const char* pass ="25978333"; //修改為你家的WiFi密碼
const char* mqtt_server = "ckrouter.synology.me";
const int mqtt_port = 1883;
const char* mqtt_user = "user";
const char* mqtt_pass = "20130310";
const char* mqtt_topic = "esp32";
const char* sub_topic = "esp32p";


WiFiClient espClient;
PubSubClient client(espClient);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

void dispConnecting() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Connecting to WiFi...");
  display.display();
}

void dispIP() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("IP Address");
  display.println(WiFi.localIP());
  display.println("RSSI");
  display.println(WiFi.RSSI());
    if (!client.connected()) {
  display.println("Connecting to MQTT...");
    } else{
  display.println("MQTT Connected");
    }
  display.display();
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void frommqtt(char* topic, byte* payload, unsigned int length) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print("Message arrived [");
  for (int i = 0; i < length; i++) {
    display.print((char)payload[i]);
  }
  display.print("] ");
  display.display();
}








void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
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
/*
 // Display static text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Hello World!");
  display.println("Hello World!!!!");
  display.display();
*/
}




void loop(){

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  publishData();
  
  client.setCallback(frommqtt);
  delay(3000);

 dispIP();
 delay(3000);










}