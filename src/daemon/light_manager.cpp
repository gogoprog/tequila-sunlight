#include "light_manager.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>

#define LOG_PREFIX "[tequila-sunlight-daemon] "

// Public

LightManager::LightManager()
    :
    comPortString("/dev/rfcomm0")
{
}

void LightManager::addCommand(const string & command)
{
    boost::mutex::scoped_lock
        lock(commandMutex);

    commands.push_back(command);
}

void LightManager::update(const int ticks)
{
    vector<string>
        commands_copy;
    vector<string>::iterator
        iter;

    {
        boost::mutex::scoped_lock
            lock(commandMutex);

        commands_copy = commands;

        commands.clear();
    }

    for(iter=commands_copy.begin(); iter != commands_copy.end(); ++iter)
    {
        string
            command;

        command = *iter;

        processCommand(command);
    }
}

void LightManager::setComPort(const string & com_port_string)
{
    comPortString = com_port_string;
}

void LightManager::initializeComPort()
{
    cout << LOG_PREFIX << "Opening com port " << comPortString << endl;

    comPort.open(comPortString);

    if(comPort.isOpen())
    {
        cout << LOG_PREFIX << "Com port " << comPortString << " opened!" << endl;
    }
    else
    {
        cout << LOG_PREFIX << "Cannot open " << comPortString << endl;
    }
}

// Private

void LightManager::processCommand(const string & command)
{
    vector< string >
        parts;
    Light
        current_lamp;
    Code
        code;
    int
        extra_code;

    extra_code = DUMMY;

    cout << LOG_PREFIX << "\"" << command << "\" -> ";

    boost::split(parts, command, boost::is_any_of("/"));

    if(parts[1] == "RED")
    {
        current_lamp = RED;
        cout << "Red ";
    }
    else if(parts[1] == "GREEN")
    {
        current_lamp = GREEN;
        cout << "Green ";
    }
    else if(parts[1] == "ORANGE")
    {
        current_lamp = ORANGE;
        cout << "Orange ";
    }
    else if(parts[1] == "COMPORT")
    {
        comPortString = command.substr(8);

        cout << "Com port changed to " << comPortString << endl;

        initializeComPort();

        return;
    }
    else
    {
        cout << "unknown command" << endl;
        return;
    }

    if(parts[2] == "ON")
    {
        code = ON;
        cout << "On" << endl;
    }
    else if(parts[2] == "OFF")
    {
        code = OFF;
        cout << "Off" << endl;
    }
    else if(parts[2] == "BLINK")
    {
        code = BLINK;

        extra_code = atoi(parts[3].c_str());

        cout << "Blink " << extra_code << endl;

        extra_code += BLINK_INTERVAL_OFFSET;
    }
    else if(parts[2] == "STOP_BLINK")
    {
        code = STOP_BLINK;

        cout << "StopBlink " << endl;
    }
    else
    {
        cout << "unknown command" << endl;
        return;
    }

    if(comPort.isOpen())
    {
        comPort << RESET << current_lamp << code << extra_code;
    }
    else
    {
        cout << LOG_PREFIX << "Command skipped because com port is not opened." << endl;
    }
}
