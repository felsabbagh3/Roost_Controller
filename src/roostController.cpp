#include "roostController.h"

using namespace std;


roostController_t::~roostController_t ()
{
}

roostController_t::roostController_t (environment_t::config_t config_, double initTemp_)
{
    envConfig = config_;
    currTemp = initTemp_;
    dstTemp = -1;
}


void roostController_t::checkSafe(environment_t::state_t * currState)
{
	if ((*currState).ac.timeOff < (1 - envConfig.ac.dutyCycle) * 60)
	{
		acSafeToOpen = false;
	} else
	{
		acSafeToOpen = true;
	}

	if ((*currState).heater.timeOff < (1 - envConfig.heater.dutyCycle) * 60)
	{
		heaterSafeToOpen = false;
	} else
	{
		heaterSafeToOpen = true;
	}

	if (((*currState).ac.timeOn + 2) > 60 * envConfig.ac.dutyCycle)
	{
		acSafeClose = true;
	} else
	{
		acSafeClose = false;
	}

	if (((*currState).heater.timeOn + 2) > 60 * envConfig.heater.dutyCycle)
	{
		heaterSafeClose = true;
	} else
	{
		heaterSafeClose = false;
	}

}

bool roostController_t::checkDst(environment_t * env, userCommandController * usrCommand, environment_t::state_t * currState) 
{

	userCommand_t * currCommand = usrCommand->getCurrCommand(currState->time);

	// This loop executes commands from either a routine or an independant command
	while (((currCommand->getCommand() != "CONT"))) 
	{
		if (currCommand->getCommand() == "SET_TEMP") // Deals with SET_TEMP commands
		{
			set_temp * set_temp_command = dynamic_cast<set_temp*>(currCommand);
			dstTemp = set_temp_command->getTemp();
			if (dstTemp != -1) {
				if (dstTemp < envConfig.ac.equilibrium)
				{
					dstTemp = envConfig.ac.equilibrium;
				} else if (dstTemp > envConfig.heater.equilibrium)
				{
					dstTemp = envConfig.heater.equilibrium;
				}
			}

			routine_name = set_temp_command->getRoutineName();
			routine_created = set_temp_command->getTimeStamp();
			if (set_temp_command->getState() == 0)
			{
				command_state = false;
			}  else
			{
				command_state = true;
			}
			actualDstTemp = dstTemp;

		} else if (currCommand->getCommand() == "PRINT_STATUS") // Deals with PRINT_STATUS command
		{
			cout << "Roost Status =====================" << endl;
			cout << "| Time:            " << currState->time << endl;
			cout << "| Cur. Temp:       " << currState->temperature << endl;
			cout << "| Desired Temp:    " << dstTemp << endl;
			if (command_state)
			{
				cout << "| Current routine: " << usrCommand->getCurrRoutine() << " (" << routine_created << ")" << endl;
			} else {
				cout << "| Current routine: " << usrCommand->getCurrRoutine() << " (" << currState->time << ")" << endl;
			}
			
			cout << "==================================" << endl;
		}
		currCommand = usrCommand->getCurrCommand(currState->time);

		if (currCommand == nullptr) // if this is true, that means no more user commands/program should end
		{
			return false;
		}
	}

	double diff = actualDstTemp - ((currState -> temperature));
	double absDiff = abs(diff);

	if (absDiff < 0.5 || dstTemp == -1) // Turns off the AC/Heater for effeciency
	{
		actualDstTemp = -1;
	}

	if (actualDstTemp != -1) // Deals with getting to the target temp
	{
		this->checkSafe(currState);

		if (acSafeClose)
		{
			env->enableAC(false);
		} else
		{
			if ((diff < -2) && acSafeToOpen)
			{
				if (currState->heater.on)
				{
					env->enableHeater(false);
					currState->heater.on = false;
				}
				env->enableAC(true);
				currState->ac.on = true;
			}
		}

		if (heaterSafeClose)
		{
			env->enableHeater(false);
		} else
		{
			if ((diff > 2) && heaterSafeToOpen)
			{
				if (currState->ac.on)
				{
					env->enableAC(false);
					currState->ac.on = false;
				}
				env->enableHeater(true);
				currState->heater.on = true;
			}
		}
	} else
	{
		env->enableHeater(false);
		env->enableAC(false);
	}


	return true;
}




