
#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include<SoftwareSerial.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define ss 15
#define rst 16
#define dio0 2

#define WLAN_SSID       "jaanu"
#define WLAN_PASS       "jaanu143"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "jayaprakash72"
#define AIO_KEY         "aio_AzIm462yCmKW5BuforQhAOBTcC1u"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/moisture");

 float LoRaData;

void MQTT_connect();
void setup() {
 Serial.begin(115200);
//pinMode(sensor,INPUT);

  Serial.println(F("Adafruit MQTT demo"));
  

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

}

uint32_t x=0;
void loop() {
   MQTT_connect();

   Serial.print(F("\nSending photocell val "));
  Serial.print("moisture value");
  Serial.print("...");
  //float sensor_value=readMois();
  /*float sensor_value=data;
  if (! photocell.publish(sensor_value)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
*/
  
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      //String LoRaData = LoRa.readString();
      float LoRaData = LoRa.read();
      Serial.print(LoRaData); 
     /* if (! photocell.publish(LoRaData)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }*/
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

  if (! photocell.publish(LoRaData)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(500);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
 
  Serial.println("MQTT Connected!");
}
