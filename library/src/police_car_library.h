#ifndef POLICE_CAR_LIBRARY_H
#define POLICE_CAR_LIBRARY_H 

#include <Arduino.h>

class PoliceCar 
{
    
private:
    // Timer1 (tone)
    static void _setupTimer(int TimerIndex, float period_s);
    static void _disableTimer(int TimerIndex);


}

#endif // POLICE_CAR_LIBRARY_H