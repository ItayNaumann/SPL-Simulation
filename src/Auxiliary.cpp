#include "Auxiliary.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
/*
This is a 'static' method that receives a string(line) and returns a vector of the string's arguments.

For example:
parseArguments("settlement KfarSPL 0") will return vector with ["settlement", "KfarSPL", "0"]

To execute this method, use Auxiliary::parseArguments(line)
*/

vector<string> Auxiliary::parseArguments(const string &line)
{
    vector<string> arguments;
    istringstream stream(line);
    string argument;

    while (stream >> argument)
    {
        arguments.push_back(argument);
    }

    return arguments;
}

void Auxiliary::initialParsing(const string &configFilePath, Simulation &simulation)
{
    ifstream file(configFilePath);
    string line;
    vector<string> arguments;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            arguments = Auxiliary::parseArguments(line);

            if (arguments[0] == "settlement")
            {
                int type = stoi(arguments[2]);
                Settlement *settlement = new Settlement(arguments[1], SettlementType(type));
                simulation.addSettlement(settlement);
            }
            else if (arguments[0] == "facility")
            {
                int type = stoi(arguments[2]);
                FacilityType facility(arguments[1], FacilityCategory(type), stoi(arguments[3]), stoi(arguments[4]), stoi(arguments[5]), stoi(arguments[6]));
                simulation.addFacility(facility);
            }
            else if (arguments[0] == "plan")
            {
                Settlement &settlement = simulation.getSettlement(arguments[1]);
                SelectionPolicy *selectionPolicy = nullptr;
                if (arguments[2] == "nve")
                {
                    selectionPolicy = new NaiveSelection();
                }
                else if (arguments[2] == "bal")
                {
                    selectionPolicy = new BalancedSelection(0, 0, 0);
                }
                else if (arguments[2] == "eco")
                {
                    selectionPolicy = new EconomySelection();
                }
                simulation.addPlan(settlement, selectionPolicy);
            }
        }
    }
}

BaseAction *Auxiliary::parseToAction(vector<string> &parsedArguments)
{
    string action = parsedArguments[0];
    if (action == "step")
    {
        return new SimulateStep(stoi(parsedArguments[1]));
    }
    else if (action == "plan")
    {
        return new AddPlan(parsedArguments[1], parsedArguments[2]);
    }
    else if (action == "settlement")
    {
        return new AddSettlement(parsedArguments[1], SettlementType(stoi(parsedArguments[2])));
    }
    else if (action == "facility")
    {
        return new AddFacility(parsedArguments[1], FacilityCategory(stoi(parsedArguments[2])), stoi(parsedArguments[3]), stoi(parsedArguments[4]), stoi(parsedArguments[5]), stoi(parsedArguments[6]));
    }
    else if (action == "planStatus")
    {
        return new PrintPlanStatus(stoi(parsedArguments[1]));
    }
    else if (action == "changePolicy")
    {
        return new ChangePlanPolicy(stoi(parsedArguments[1]), parsedArguments[2]);
    }
    else if (action == "log")
    {
        return new PrintActionsLog();
    }
    else if (action == "close")
    {
        return new Close();
    }
    else if (action == "backup")
    {
        return new BackupSimulation();
    }
    else if (action == "restore")
    {
        return new RestoreSimulation();
    }
}
