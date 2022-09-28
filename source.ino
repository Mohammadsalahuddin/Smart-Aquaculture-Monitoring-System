#define THINGER_SERIAL_DEBUG
#include <ThingerESP32.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_60X);
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
int tdsPin = A0;
float red, green, blue;  
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

float getTemperature()
{
  sensors.requestTemperatures(); 
  return sensors.getTempCByIndex(0);
}


void setup() {

  sensors.begin();  
  Serial.begin(115200);
  if (tcs.begin()) {
    Serial.println("Color sensor found. WOW!");
  } else {
    Serial.println("No TCS34725 color sensor found ... check your sensor connections");
    while (1);
  }
  Serial.println("Moving Forward.");
  thing.add_wifi(SSID, SSID_PASSWORD);
  
  thing["aqua"] >> [] (pson& out)
  {
    out["Temp"] = getTemperature();
    out["TDS"] = analogRead(tdsPin);
    tcs.getRGB(&red, &green, &blue);
    out["red"] = int(red);
    out["green"] = int(green);
    out["blue"] = int(blue);
  };
}

void loop() {
  thing.handle();
}
