#ifndef DsSensor_Def
#define DsSensor_Def

#include <DallasTemperature.h>

class DsSensor
{
	public:
		DsSensor(int pin);
		
		float ReadTemperature();
		float ReadHumidity();
		
	private:
		//DS18B20
		DallasTemperature* sensors;
};

#endif