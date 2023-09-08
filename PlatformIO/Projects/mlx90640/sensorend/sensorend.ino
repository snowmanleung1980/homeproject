#include <WiFi.h>

#include <PubSubClient.h>

#include <WiFiClientSecure.h>




#define RXD2 16

#define TXD2 17




const char *ssid = "iot";  // Enter your WiFi name

const char *password = "25978333";  // Enter WiFi password




// MQTT Broker

const char *mqtt_broker = "192.168.2.35";

const char *mqtt_username = "admin";

const char *mqtt_password = "Castco@ctc2021";

const int mqtt_port = 1883;

WiFiClient espClient;

PubSubClient client(espClient);

uint8_t Re_buf[10];

uint16_t counter = 0;

unsigned char sign = 0;

int led = 13;


byte raw[1536] = {};
int rawCounter = 0;

bool canSend = false;

TaskHandle_t publishToMQTTTask;

//-----------------------------------------------------------

void setup() {
  Serial.begin(460800);
  WiFi.begin(ssid, password);




  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

  }

  xTaskCreatePinnedToCore(publishToMQTT, "publishToMQTTTask", 10000, NULL, 0, &publishToMQTTTask, 0);
}

//-------------------------------------------------------------
void loop() {

  if (Serial.available() > 0) {

    Re_buf[counter] = (unsigned char)Serial.read();


    if (counter == 0 && Re_buf[0] != 0x5A) return;  // 检查帧头

    else if (counter == 1 && Re_buf[1] != 0x5A) {

      counter = 0;

      return;
    };


    if (counter < 6) {

      counter++;
    }

    if (counter == 4)  //接收到数据

    {

      if (Re_buf[2] == 0x02 && Re_buf[3] == 0x06) {

        sign = 1;

      } else {

        counter = 0;
      }
    }

    if (counter > 5) {

      raw[rawCounter] = (byte)Re_buf[4] ;
      
      rawCounter++;
      
      raw[rawCounter] = (byte)Re_buf[5];
      
      rawCounter++;

      counter = 4;

      if(rawCounter==1536)
       {

        canSend = true;

        counter = 0;

        rawCounter = 0;
       }

    }
  }
}

void publishToMQTT(void *parameter) {
  //connecting to a mqtt broker

  client.setServer(mqtt_broker, mqtt_port);




  while (!client.connected()) {

    String client_id = "esp32-client-";

    client_id += String(WiFi.macAddress());


    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {


      //  client.subscribe("TTEST");…

    } else {

      delay(2000);
    }
  }



  client.setBufferSize(2048);

  delay(1000);
  for (;;) {
    if (canSend == true) {
      client.publish("TTTTEST", raw, sizeof(raw));
      canSend = false;
    } else {
      delay(1);
    }
  }
}
