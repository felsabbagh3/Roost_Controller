#pragma once

#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <queue> 
#include "userCommand.h"

// temp
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
//

using namespace std;


class userCommandController
{
public:
	/**
	* \param ifsteam * should be the pointer for the command file
	*/
	userCommandController(ifstream *);
	/**
	* Destructor
	*/
	~userCommandController();

	/**
	* \param int currTime is the currTime of the enviroment
	* \This member function returns the current command that corrosponds
	*   to the currTime of the enviroment
	*/
	userCommand_t * getCurrCommand(int);
	/**
	* \Returns the current routine we are working with
	*/
	string getCurrRoutine() const;
	/**
	* \Gets all the commands from the input file. 
	*   it also creates the routines, and fills up the queues
	*/
	void getAllCommands();
	/**
	* \param currTime of the enviroment
	* \returns if the program should end or not
	*/
	bool shouldContinue(int);

	/**
	* \returns the currCommand string name
	*/
	string getCommand() const;
	/**
	* \returns the option for the currCommand
	*/
	int getOption() const;

private:
	// scenarioFile
	ifstream * scenarioFile;
	// the current command that should be sent to roost Controller
	userCommand_t * currCommand;
	// A constant command when there is no commands for the currTime
	userCommand_t * contCommand;
	// A queue for all the non-routine commands from the scenario files
	queue<userCommand_t*> allCommands;
	// A queue for the commands in the routine specifies by START_ROUTINE or CONTINUE
	queue<userCommand_t*> routine_commands;
	// A vector that has all the routine objects which has the routine commands
	vector<routine> routines;
	// If there is a routine we are currently working on
	bool routine_bool;
	// The name of the current routine
	string currRoutine;
};