#include <iostream>
#include "unistd.h"
using namespace std;
namespace CoffeeMaker {
	enum class WarmerPlateStatus {
		WARMER_EMPTY, POT_EMPTY, POT_NOT_EMPTY
	};
	enum class BoilerStatus {
		EMPTY, NOT_EMPTY
	};
	enum class BrewButtonStatus {
		NOT_PUSHED, PUSHED
	};
	enum class BoilerHeaterState {
		OFF, ON
	};
	enum class WarmerState {
		OFF, ON
	};
	enum class IndicatorState {
		OFF, ON
	};
	enum class ReliefValveState {
		CLOSED, OPEN
	};
	/*
	 * This function returns the status of the warmer-plate
	 * sensor. This sensor detects the presence of the pot
	 * and whether it has coffee in it.
	 */
	WarmerPlateStatus GetWarmerPlateStatus(){
		return WarmerPlateStatus::POT_EMPTY;
	}
	
	/*
	 * This function returns the status of the boiler switch.
	 * The boiler switch is a float switch that detects if
	 * there is more than 1/2 cup of water in the boiler.
	 */
	BoilerStatus GetBoilerStatus()
	{
		return BoilerStatus::EMPTY;
	}
	/*
	 * This function returns the status of the brew button.
	 * The brew button is a momentary switch that remembers
	 * its state. Each call to this function returns the
	 * remembered state and then resets that state to
	 * NOT_PUSHED.
	 *
	 * Thus, even if this function is polled at a very slow
	 * rate, it will still detect when the brew button is
	 * pushed.
	 */
	BrewButtonStatus GetBrewButtonStatus()
	{
		return BrewButtonStatus::PUSHED;
	}
	/*
	 * This function turns the heating element in the boiler
	 * on or off.
	 */
	void SetBoilerHeaterState(BoilerHeaterState s)
	{

	}
	/*
	 * This function turns the heating element in the warmer
	 * plate on or off.
	 */
	void SetWarmerState(WarmerState s)
	{

	}
	/*
	 * This function turns the indicator light on or off.
	 * The indicator light should be turned on at the end
	 * of the brewing cycle. It should be turned off when
	 * the user presses the brew button.
	 */
	void SetIndicatorState(IndicatorState s)
	{

	}
	/*
	 * This function opens and closes the pressure-relief
	 * valve. When this valve is closed, steam pressure in
	 * the boiler will force hot water to spray out over
	 * the coffee filter. When the valve is open, the steam
	 * in the boiler escapes into the environment, and the
	 * water in the boiler will not spray out over the filter.
	 */
	void SetReliefValveState(ReliefValveState s)
	{

	}

	/*
	* This is main loop.
	*/ 
	void AutoRun()
	{
		enum coffee_sts{
		STARTING,
		FINISHED,
			ERR_CUP_NOT_EMPTY,
			ERR_CUP_NOT_ON
		} coffee_sts;
		coffee_sts = FINISHED;
		for (;;)
		{
			usleep(1000*100);
			// 按键检测
			if(BrewButtonStatus::NOT_PUSHED == GetBrewButtonStatus()){
				continue;
			} 
			enum WarmerPlateStatus tmp_sts = GetWarmerPlateStatus();
			if(WarmerPlateStatus::WARMER_EMPTY == tmp_sts){
				std::cout << "错误-使用空杯再重新开始"<<endl;
				continue;
			}else if (WarmerPlateStatus::POT_NOT_EMPTY == tmp_sts){
				std::cout<<"错误-咖啡壶不在保温盘上"<<endl;
				continue;
			}else{
				int isBoilerEnd = 0;
				for(;;){
					if (BoilerStatus::EMPTY == GetBoilerStatus()){
						SetBoilerHeaterState(BoilerHeaterState::OFF);
						SetWarmerState(WarmerState::ON);
						SetIndicatorState(IndicatorState::ON);
						SetReliefValveState(ReliefValveState::OPEN);
						isBoilerEnd = 1;
						break;
					}else{
						SetBoilerHeaterState(BoilerHeaterState::ON);
						SetWarmerState(WarmerState::OFF);
						SetIndicatorState(IndicatorState::OFF);
						SetReliefValveState(ReliefValveState::CLOSED);
					}
					usleep(1000*100);
				}
				if (isBoilerEnd > 0){
					continue;
				}
			}
		}
	}
}
int main()
{
	CoffeeMaker::AutoRun();
	return 0;
}

