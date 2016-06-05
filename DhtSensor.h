#ifndef ClassA_Def
#define ClassA_Def

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