#include "light_manager.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#define LOG_PREFIX "[tequila-sunlight-daemon] "

// Public

LightManager::LightManager()
{
    initializeComPort();
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

// Private

void LightManager::initializeComPort()
{
    for(int i=0; i<4; ++i)
    {
        cout << LOG_PREFIX << "Opening com port " << i << endl;
        comPort.open(i);

        if(comPort.isOpen())
        {
            cout << LOG_PREFIX << "Com port " << i << " opened!" << endl;

            return;
        }
    }

    cout << LOG_PREFIX << "No com port found!" << endl;
}

void LightManager::processCommand(const string & command)
{
    vector< string >
        parts;
    Light
        current_lamp;
    Code
        code;

    cout << LOG_PREFIX;

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
    else
    {
        // :TODO: BLINK
        cout << "unknown command" << endl;
        return;
    }

    comPort << RESET << current_lamp << code;
}
