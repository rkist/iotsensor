#include "DhtSensor.h"


#define DHTTYPE DHT11



DhtSensor::DhtSensor(int pin)
{
	// Initialize DHT sensor
	// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
	// you need to increase the threshold for cycle counts considered a 1 or 0.
	// You can do this by passing a 3rd parameter for this threshold.  It's a bit
	// of fiddling to find the right value, but in general the faster the CPU the
	// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
	// Arduino Due that runs at 84mhz a value of 30 works.
	// This is for the ESP8266 processor on ESP-01
	this->dht = new DHT(pin, DHTTYPE, 11); // 11 works fine for ESP8266
	dht->begin();           // initialize temperature sensor
}

float DhtSensor::ReadTemperature()
{
	// Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)    
    float temp = this->dht->readTemperature(false);     
    // Check if any reads failed and exit early (to try again).
    if (isnan(temp)) 
    {
      Serial.println("Failed to read from DHT sensor!");
    }
	
	return temp;
}

float DhtSensor::ReadHumidity()
{
	float humidity = this->dht->readHumidity();          // Read humidity (percent)
	
	if (isnan(humidity)) 
    {
      Serial.println("Failed to read from DHT sensor!");
    }
	return humidity;
}

