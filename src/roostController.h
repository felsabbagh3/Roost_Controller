#pragma once

#include "environment.h"
#include "userCommand.h"
#include "userCommandController.h"

#include <iomanip>
#include <vector>
#include <cmath>

/// \brief This is where you should implement the firmware logic that will drive
/// the Roost (tm) device. The primary role is to take in user commands and
/// output the appropriate hardware commands
class roostController_t
{
public:

    ~roostController_t ();

    /// \param config_ Environment config
    /// \param initTemp_ Initial desired temperature
    explicit roostController_t (environment_t::config_t config_, double initTemp_);

    /**
    * \param currState - the current state of the enviroment
    * \ This member function checks the current state of the enviroment
    *   for saftey conditions like overheating and cooling down period
    */
    void checkSafe(environment_t::state_t*);

    /**
    * \param enviroment_t * is the enviroment
    * \param userCommandController * userCommand is the object that provides the user commands
    *   to the roostController
    * \param state_t currState is the current state of the enviroment in each iteration
    * \returns bool for whether the program shouls end or not
    *
    * \This member functino is the main controller, it deals with the user commands,
    *   makes sure the controller is safe and also deals with efficiency 
    */ 
    bool checkDst(environment_t *, userCommandController *, environment_t::state_t *);



private:
	// enviroment config
	environment_t::config_t envConfig;
	// changing temp variables
		// Current temp
		double currTemp;
		// Taraget temp
		double dstTemp;
		// Target temp that changes for effeciency
		double actualDstTemp;

	// Routine Variables
		// Current name of the routine
		string routine_name;
		// time routine set_temp was created
		int routine_created;
		// If we are currently working on a routine or not
		bool command_state;

	// Saftey variables
		// Is it safe to open AC?
		bool acSafeToOpen;
		// Do we need to close the AC?
		bool acSafeClose;
		// Is it safe to open heater?
		bool heaterSafeToOpen;
		// Do we need to close the heater?
		bool heaterSafeClose;
};
