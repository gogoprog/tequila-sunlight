#include "light_manager.h"

// Public

LightManager::LightManager()
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

        cout << "Command: " << command << endl;
    }
}

// Private

void LightManager::initializeComPort()
{
    for(int i=0; i<4; ++i)
    {
        cout << "TequilaSunlight: opening com port " << i << endl;
        comPort.open(i);

        if(comPort.isOpen())
        {
            cout << "TequilaSunlight: com port " << i << " opened!" << endl;
            return;
        }
    }
}
