#pragma once

#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <iostream>
#include <sstream>

using namespace std;

/// \brief User command base class
class userCommand_t
{
public:
        /// \brief Parse a command definition string into a command
        /// \param commandDef_ Command definition string
        /// \return User command on success, null pointer on failure
        static userCommand_t const *parse (std::string const &commandDef_);

        virtual ~userCommand_t ();

        /// \brief Get the timestamp associated with this command
        int getTimeStamp () const;
        string getCommand() const;

        /// \param timestamp_ Timestamp of command
        /// \command_ Name of the command
        userCommand_t (int timestamp_, string command_);

private:
        // Timestamp of the command
        int m_timestamp;
        // Name of the command
        string command;
};


/***************
* Class for SET_TEMP commands
*   -> A derived userCommand_t class
***************/

class set_temp : public userCommand_t
{
public:
        /**
        * \param int timestamp_ is the timestamp of the command
        * \param string command_ is the name of the command
        * \param int temp_ is the target temp
        * \param int state_ is wheather this set_temp is comming from a routine
        *   (0 if not a routine and 1 if from a routine)
        * \param string routine_name is the name of the routine
        * \param routine_created_ is the timestamp of routine command that created it
        */
        set_temp(int,string,int,int=0,string="(none)",int=0);

        // \return target temp
        int getTemp() const;
        // \return State
        int getState() const;
        // \return routine_name
        string getRoutineName() const;
        // \return time routine was created
        int getRoutineCreated() const;
private:
        // \target temp
        int temp;
        // command state
        int state;
        // routine name that created this command
        string routine_name;
        // time routine command created it
        int routine_created;
};

/***************
* Class for PRINT_STATUS commands
*   -> A derived userCommand_t class
***************/
class print_command : public userCommand_t
{
public:
        /**
        * \param int timestamp_ is the timestamp of the command
        * \param string command_ is the name of the command
        */
        print_command(int,string);
};

/***************
* Class for routine related commands (START_ROUTINE/CONTINUE_ROUTINE)
*   -> A derived userCommand_t class
***************/
class routine_command : public userCommand_t
{
public:
        /**
        * \param int timestamp_ is the timestamp of the command
        * \param string command_ is the name of the command
        * \param routine_name_ is the name of the routine
        */
        routine_command(int,string,string);

        // returns routine Name
        string getRoutineName() const;
private:
        string routine_name;
};

/***************
* Class routine to store routine information (CREATE_ROUTINE)
***************/
class routine
{

public:
        /**
        * \param int routine_created is the time the routine was created
        * \param string routine_name is the name of the routine
        * \param queue routine_commands_ is all the commands in the routine
        */
        routine(int,string,queue<userCommand_t*>);

        // returns time routine was created
        int getRoutineCreated() const;
        // returns routine name
        string getRoutineName() const;
        // returns all commands realted to that routine
        queue<userCommand_t*> getRoutineCommands() const;

private:
        // time routine was created
        int routine_created;
        // name of the routine
        string routine_name;
        // all commands related to that routine
        queue<userCommand_t*> routine_commands;


};






