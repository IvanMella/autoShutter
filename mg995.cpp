#include "mg995.h"
//#define VERBOSE

/* Contructor */
mg995::mg995()
{
}

/* Attach the servo */
void mg995::attach(int _mgPin, int _servoTimeout) 
{
        mgPin = _mgPin;
        servoTimeout = _servoTimeout;
        timeout = 0;
        /* configure port for servo */
        ledcAttach(mgPin, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
}

void mg995::write(int targetDegree)
{
        // Do nothing until a real target degree is triggered
        if(targetDegree == -1) 
                return;

        /* map 0 to 180 to correct pwm pulses           */
        unsigned int targetDuty = map(targetDegree, 180, 0, ledcLoDuty, ledcHiDuty);

        /* nothing changed */
        if(duty == targetDuty) 
                return;

        /* Rotate servo non blocking, non reentrant routine*/ 
        if(millis() - timeout >= servoTimeout)
        {
                /* calculates new timeout */
                timeout = millis();

                if(duty < targetDuty)
                {
                        duty++;
                }
                else if (duty > targetDuty)
                {
                        duty--;        
                }

                /* send PWM to port */
                #ifdef VERBOSE
                Serial.print("@ ");
                Serial.println(degree);
                #endif
                ledcWrite(mgPin, duty);
        }
}        



