#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid ="iot"; //修改為你家的WiFi網路名稱
const char* pass ="25978333"; //修改為你家的WiFi密碼
const char* mqtt_server = "ckrouter.synology.me";
const int mqtt_port = 1883;
const char* mqtt_user = "user";
const char* mqtt_pass = "20130310";
const char* mqtt_topic = "esp32/sensor";


WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi RSSI:");
  Serial.println(WiFi.RSSI()); //讀取WiFi強度
}


/*void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  }
*/

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


void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected


  setup_wifi();

  client.setServer(mqtt_server, 1883);
  // client.setCallback(callback);

    while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {

      Serial.println("connected");  
      client.publish("outTopic","hello world");
      client.subscribe("inTopic");
    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
    }


}

void loop(){

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  client.publish(mqtt_topic, ssid);
  delay(300);


  // Read WiFi Signal Strength
  long rssi = WiFi.RSSI();

  // Publish to MQTT Topic 
  char msg[10];
  sprintf(msg,"%ld",rssi);
  client.publish("esp32/rssi", msg);
  




  delay(3000);
}