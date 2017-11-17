#include "environment.h"
#include "roostController.h"
#include "userCommand.h"
#include "userCommandController.h" 

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
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

void printUsage ()
{
        cout << "Usage: ./roost <scenario file>" << endl;
        cout << "<scenario file>\t\tRoost scenario file" << endl;
}

int main (int argc, char **argv)
{
        if (argc != 2)
        {
                printUsage ();
                return 1;
        }

        ifstream scenarioFile (argv[1]);

        if (!scenarioFile)
        {
                cerr << "Error reading scenario file" << endl;
                printUsage ();
                return 2;
        }

        // Setup environment config
        string line;
        int init_temp = -1;
        getline(scenarioFile, line);
        vector<string> elements = split(line);
        if (elements[0] == "INIT_TEMP") {
                init_temp = std::stoi(elements[1]);
        }
        environment_t::config_t const envDesc = { { 0.75, 40, 65 }, { 0.6, 10, 80 }, 100, init_temp };


        // initializing the enviroment
        environment_t environment (envDesc);
        environment_t::state_t currState = environment.update(0);

        // initializing the userCommandController
        userCommandController * commandCont = new userCommandController(&scenarioFile);

        // initializing the roostController
        roostController_t roost (envDesc, envDesc.initTemp);

        bool cont = true;
        while (cont)
        {
                cont = roost.checkDst(&environment,commandCont,&currState);
                currState = environment.update(1);
        }
        if (!cont)
        {
                cout << "Simulation complete. Total energy usage: " << currState.ac.energyUsed + currState.heater.energyUsed << endl;
        }

        delete commandCont;
        return 0;
}
