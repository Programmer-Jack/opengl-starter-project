#ifndef CLOCK_CLASS_H
#define CLOCK_CLASS_H

#include <chrono>

class ClockUtil
{
    private:
        long t_0;
        long t_last;

    public:
        ClockUtil();
        
        void update();
        long getTimeSinceInit();
        long getT_0();

        float deltaTime();
};

#endif