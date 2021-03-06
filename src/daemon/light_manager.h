#include <boost/thread.hpp>
#include <string>
#include <memory>

#include "com_port.h"

using namespace std;

class LightManager
{
public:
    LightManager();
    void addCommand(const string & command);
    void update(const int ticks);
    void setComPort(const string & com_port_string);
    void initializeComPort();

private:

    enum Light
    {
        GREEN = '1',
        ORANGE,
        RED
    };

    enum Code
    {
        DUMMY = 0,
        RESET = 13,
        BLINK_INTERVAL_OFFSET = 48,
        BLINK = 98,
        STOP_BLINK = 110,
        ON = 111,
        OFF = 112
    };

    void processCommand(const string & command);

    boost::mutex
        commandMutex;
    vector<string>
        commands;
    std::shared_ptr<ComPort>
        comPort;
    string
        comPortString;
};
