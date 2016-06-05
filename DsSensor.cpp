#include "DsSensor.h"

#include <OneWire.h>

#define SENSOR_RESOLUTION 12 // How many bits to use for temperature values: 9, 10, 11 or 12
#define SENSOR_INDEX 0 // Index of sensors connected to data pin, default: 0

DsSensor::DsSensor(int pin)
{
	OneWire oneWire(pin);
	this->sensors = new DallasTemperature(&oneWire);
	DeviceAddress sensorDeviceAddress;
	
	this->sensors->begin();
	this->sensors->getAddress(sensorDeviceAddress, 0);
	this->sensors->setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);
}

float DsSensor::ReadTemperature()
{	
	this->sensors->requestTemperatures(); // Measurement may take up to 750ms
    float temperatureInCelsius = this->sensors->getTempCByIndex(SENSOR_INDEX); 
	return temperatureInCelsius;
}

float DsSensor::ReadHumidity()
{
	return -1;
}