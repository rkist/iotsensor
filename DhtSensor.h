#ifndef DhtSensor_Def
#define DhtSensor_Def

#include <DHT.h>

class DhtSensor
{
	public:
		DhtSensor(int pin);
		
		float ReadTemperature();
		float ReadHumidity();
		
	private:
		DHT* dht;
};

#endif