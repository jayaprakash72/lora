#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#include <DHT.h>

//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//#define OLED_RESET     LED_BUILTIN // Reset pin # (or -1 if sharing Arduino reset pin)
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//#define DHTPIN 0          //pin where the dht11 is connected
#define ss 15
#define rst 16
#define dio0 2

#define sensorPower 7
#define sensorPin A0
int counter = 0;

//DHT dht(DHTPIN, DHT11);


void setup() 
{
  pinMode(sensorPower, OUTPUT);
  
  // Initially keep the sensor OFF
  digitalWrite(sensorPower, LOW);
  Serial.begin(115200);
  
  while (!Serial);
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);
    if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    delay(100);
    while (1);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  
}

void loop() {
  {
  //get the reading from the function below and print it
  Serial.print("Analog output: ");
  Serial.println(readSensor());
  
  delay(1000);
  }

//  This function returns the analog soil moisture measurement
/*int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = analogRead(sensorPin);  // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}*/
 /* float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
  Serial.println("Failed to read from DHT sensor!");
  return;
  }*/
  //Serial.print("Temperature: ");
  //
  //Serial.print(t);
  //Serial.print(" degrees Celcius, Humidity: ");
  //Serial.println(h);
  //Serial.println();
  Serial.print("Sending packet: ");
  Serial.print(counter);

  /*display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print(F("Pkt No:"));
  display.print(counter);
  
  display.setCursor(0,20); 
  display.print("Tem:");
  display.print(t);
  display.print("C");

  display.setCursor(0,40); 
  display.print("Hum:");
  display.print(h);
  display.print("%");
  display.display();*/
  
  // send packet
  LoRa.beginPacket();
  LoRa.print(F("Pkt No:"));
  LoRa.println(counter);
  
  LoRa.print("Analog output: ");
  LoRa.print(readSensor());
  //LoRa.println("??C");
  
  //LoRa.print("Hum:  ");
  //LoRa.print(h);
 // LoRa.print("%");
  LoRa.println("");
  
  LoRa.endPacket();

  counter++;

  delay(3000);
}

int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = analogRead(sensorPin);  // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}
