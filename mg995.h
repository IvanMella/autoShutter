/* mg995.h Header .
 * General purpose library for the use of MG995 stepper motor
  */

#ifndef MG995_H
#define MG995_H

#include <Arduino.h>

constexpr int LEDC_TIMER_BIT = 12;      // bit precision for LEDC timer
constexpr int LEDC_BASE_FREQ = 50;      // LEDC base frequency
// Specific duty limits
constexpr int ledcLoDuty = 130;
constexpr int ledcHiDuty = 528;


class mg995
{
	private:
        
		int mgPin;			// Connected pin		
                unsigned long timeout;          // Keep the last execution timestamp
                unsigned int duty;              // 9999 do nothing ;)
                unsigned int servoTimeout = 0;  // default setvo timeOut (Max speed)
        public:
		mg995();                        // MG995 constructor		
                void attach(int, int);          // port, delay in ms
		void write(int);                // rotate servo at defined speed	
};
#endif
