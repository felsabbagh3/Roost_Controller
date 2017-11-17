#include "userCommandController.h"

using namespace std;

namespace
{
/// \brief Splits a space-delimited line into words:
/// \param line_ Space-delimited line. Terminating new lines are ignored if present
/// \return List of words in the line
vector<string> split (string const &line_)
{
        vector<string> result;
        istringstream iss (line_);

        for (string s; iss >> s;)
                result.push_back (s);

       return result;
}
}

userCommandController::~userCommandController()
{
	delete contCommand;
	delete currCommand;
}

userCommandController::userCommandController(ifstream * scenarioFile_)
{
	scenarioFile = scenarioFile_;
	contCommand = new userCommand_t(0, "CONT");
	currCommand = nullptr;
	routine_bool = false;
	currRoutine = "(none)";
	this->getAllCommands();
}

void userCommandController::getAllCommands()
{
	string line;
	int timeStamp;
	string command;
	while (std::getline((*scenarioFile), line))
	{
		vector<string> elements = split(line);
		timeStamp = std::stoi(elements[0]);
		command = elements[1];

		if (command == "SET_TEMP")
		{

			allCommands.push(new set_temp(timeStamp, command, std::stoi(elements[2])));

		} else if (command == "PRINT_STATUS")
		{

			allCommands.push(new print_command(timeStamp, command));

		} else if (command == "CREATE_ROUTINE")
		{

			queue<userCommand_t*> routine_commands;
			string routine_name = elements[2];
			int time;
			int temp;
			for (int i = 3; i < elements.size() - 1; i++)
			{
				time = std::stoi(elements[i]);
				temp = std::stoi(elements[i+1]);
				routine_commands.push(new set_temp(time,"SET_TEMP",temp,1,routine_name,timeStamp));
			}
			routine rtn(timeStamp, routine_name, routine_commands);
			routines.push_back(rtn);

		} else if ((command == "START_ROUTINE") || (command == "RESUME_ROUTINE"))
		{

			allCommands.push(new routine_command(timeStamp, command, elements[2]));
		}
	}

}

userCommand_t * userCommandController::getCurrCommand(int currTime)
{
	if (allCommands.size() != 0)
	{
		if (currTime == allCommands.front()->getTimeStamp())
		{
			currCommand = allCommands.front();
			allCommands.pop();

			if (currCommand->getCommand() == "SET_TEMP")
			{
				routine_bool = false;
				currRoutine = "(none)";


			} else if ((currCommand->getCommand() == "START_ROUTINE") || (currCommand->getCommand() == "RESUME_ROUTINE"))
			{
				routine_command * routine_command_curr = dynamic_cast<routine_command*>(currCommand);
				for (int i = 0; i < routines.size(); i++)
				{
					if (routines[i].getRoutineName() == routine_command_curr->getRoutineName())
					{
						routine_commands = routines[i].getRoutineCommands();
						routine_bool = true;
						currRoutine = routine_command_curr->getRoutineName();
						while ((routine_commands.size() != 0) && (routine_commands.front()->getTimeStamp() < currTime))
						{
							currCommand = routine_commands.front();
							routine_commands.pop();
						}
					}
				}
			}
			return currCommand;
		}

		if (routine_bool)
		{
			if (currTime == routine_commands.front()->getTimeStamp())
			{
				currCommand = routine_commands.front();
				routine_commands.pop();
				return currCommand;
			}
		}

		return contCommand;

	} else {
		return nullptr;
	}
}

string userCommandController::getCurrRoutine() const
{
	return currRoutine;
}

bool userCommandController::shouldContinue(int currTime)
{
	return (currTime <= (currCommand->getTimeStamp()));
}


string userCommandController::getCommand() const
{
	return currCommand->getCommand();
}

int userCommandController::getOption() const
{
	if ((currCommand->getCommand()) == "SET_TEMP")
	{
		set_temp * set_temp_command = dynamic_cast<set_temp*>(currCommand);
		return set_temp_command->getTemp();
	} else {
		return 99999;
	}
}
