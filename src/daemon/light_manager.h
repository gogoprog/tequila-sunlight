#include <boost/thread.hpp>
#include <string>
#include "com_port.h"

using namespace std;

class LightManager
{
public:
    LightManager();
    void addCommand(const string & command);
    void update(const int ticks);

private:

    enum Light
    {
        GREEN = '1',
        ORANGE,
        RED
    };

    enum Code
    {
        RESET = 13,
        BLINK_INTERVAL_OFFSET = 48,
        BLINK = 98,
        STOP_BLINK = 110,
        ON = 111,
        OFF = 112
    };

    void initializeComPort();
    void processCommand(const string & command);

    boost::mutex
        commandMutex;
    vector<string>
        commands;
    ComPort
        comPort;
};
