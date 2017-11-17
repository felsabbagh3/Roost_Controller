#include "userCommand.h"

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

userCommand_t const *userCommand_t::parse (string const &commandDef_)
{
        // TODO: Implement me
        cerr << "Unhandled user command: " << commandDef_ << endl;
        return nullptr;
}

userCommand_t::~userCommand_t ()
{
}

userCommand_t::userCommand_t (int timestamp_, string command_)
        : m_timestamp (timestamp_), command(command_)
{
}

int userCommand_t::getTimeStamp () const
{
        return m_timestamp;
}

string userCommand_t::getCommand() const
{
        return command;
}

/**************
* Definition of set_temp
**************/
set_temp::set_temp(int timestamp_, string command_, int temp_, int state_, string routine_name_, int routine_created_) : userCommand_t(timestamp_, command_)
{
        temp = temp_;
        state = state_;
        routine_name = routine_name_;
        routine_created = routine_created_;
}

int set_temp::getTemp() const
{
        return temp;
}

int set_temp::getState() const
{
        return state;
}

string set_temp::getRoutineName() const
{
        return routine_name;
}

int set_temp::getRoutineCreated() const
{
        return routine_created;
}

/**************
* Definition of print_command
**************/
print_command::print_command(int timestamp_, string command_) : userCommand_t(timestamp_, command_)
{
}

/**************
* Definition of routine
**************/
routine::routine(int routine_created_, string routine_name_, queue<userCommand_t*> routine_commands_) {
        routine_created = routine_created_;
        routine_name = routine_name_;
        routine_commands = routine_commands_;
}

int routine::getRoutineCreated() const
{
        return routine_created;
}

string routine::getRoutineName() const
{
        return routine_name;
}

queue<userCommand_t*> routine::getRoutineCommands() const
{
        return routine_commands;
}

/**************
* Definition of routine_command
**************/
routine_command::routine_command(int timestamp_, string command_, string routine_name_) : userCommand_t(timestamp_, command_)
{
        routine_name = routine_name_;
}

string routine_command::getRoutineName() const
{
        return routine_name;
}



