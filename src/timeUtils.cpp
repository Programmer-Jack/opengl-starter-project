#include "timeUtils.h"

namespace sc = std::chrono;

ClockUtil::ClockUtil()
{
    auto time = sc::system_clock::now();
    auto sinceEpoch = time.time_since_epoch();
    auto ms = sc::duration_cast<sc::milliseconds>(sinceEpoch);
    t_0 = ms.count();
}

void ClockUtil::update()
{
    t_last = getTimeSinceInit();
}

/// @brief 
/// @return time in seconds since last update()
float ClockUtil::deltaTime()
{
    return (getTimeSinceInit() - t_last) / 1000.0f;
}

long ClockUtil::getTimeSinceInit()
{
    // Solution by deft_code (2010)
    // https://stackoverflow.com/a/2834294

    auto time = sc::system_clock::now();
    auto sinceEpoch = time.time_since_epoch();
    auto msSinceEpoch = sc::duration_cast<sc::milliseconds>(sinceEpoch);

    long msSinceT_0 = msSinceEpoch.count() - t_0;

    return msSinceT_0;
}

/// @brief 
/// @return time in milliseconds since clock initialization 
long ClockUtil::getT_0()
{
    return t_0;
}
