// This Arduino sketch reads DS18B20 "1-Wire" digital
// temperature sensors.
// Tutorial:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-tutorial.html

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS A0

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress insideThermometer = { 0x28, 0x15, 0x70, 0xDC, 0x06, 0x00, 0x00, 0x01 };
DeviceAddress outsideThermometer = { 0x28, 0x6B, 0xDF, 0xDF, 0x02, 0x00, 0x00, 0xC0 };
DeviceAddress dogHouseThermometer = { 0x28, 0x59, 0xBE, 0xDF, 0x02, 0x00, 0x00, 0x9F };

const int ledPin =  5; 

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(insideThermometer, 10);
  //sensors.setResolution(outsideThermometer, 10);
  //sensors.setResolution(dogHouseThermometer, 10);

  pinMode(ledPin, OUTPUT); 
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  float tempF = DallasTemperature::toFahrenheit(tempC);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {

    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(tempF);

    // ignore the temp below 110
    if (tempF < 115) {
      digitalWrite(ledPin, LOW);
      return;
    }

    // temp is going down util 110 and alarm user to put yogurt. 
    if (tempF < 118) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }

  }
}

void loop(void)
{ 
  delay(1000);
  Serial.print("Getting temperatures...\n\r");
  sensors.requestTemperatures();
  
  Serial.print("Inside temperature is: ");
  printTemperature(insideThermometer);
  Serial.print("\n\r");
//  Serial.print("Outside temperature is: ");
//  printTemperature(outsideThermometer);
//  Serial.print("\n\r");
//  Serial.print("Dog House temperature is: ");
//  printTemperature(dogHouseThermometer);
  Serial.print("\n\r\n\r");
}

