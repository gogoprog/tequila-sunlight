#include "light_manager.h"

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
