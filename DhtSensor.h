#ifndef ClassA_Def
#define ClassA_Def



class DhtSensor
{
	public:
		DhtSensor(int pin);
		
		float ReadTemperature();
		float ReadHumidity();
		
	private:
		int pin;
};

#endif