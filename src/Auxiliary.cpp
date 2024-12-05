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

            if (arguments.at(0) == "settlement")
            {
                int type = stoi(arguments.at(2));
                Settlement *settlement = new Settlement(arguments.at(1), SettlementType(type));
                simulation.addSettlement(settlement);
            }
            else if (arguments.at(0) == "facility")
            {
                int type = stoi(arguments.at(2));
                FacilityType facility(arguments.at(1), FacilityCategory(type), stoi(arguments.at(3)), stoi(arguments.at(4)), stoi(arguments.at(5)), stoi(arguments.at(6)));
                simulation.addFacility(facility);
            }
            else if (arguments.at(0) == "plan")
            {
                Settlement &settlement = simulation.getSettlement(arguments.at(1));
                SelectionPolicy *selectionPolicy = nullptr;
                if (arguments.at(2) == "nve")
                {
                    selectionPolicy = new NaiveSelection();
                }
                else if (arguments.at(2) == "bal")
                {
                    selectionPolicy = new BalancedSelection(0, 0, 0);
                }
                else if (arguments.at(2) == "eco")
                {
                    selectionPolicy = new EconomySelection();
                }
                simulation.addPlan(settlement, selectionPolicy);
            }
        }
    }
    file.close();
}

BaseAction *Auxiliary::parseToAction(vector<string> &parsedArguments)
{
    try
    {
        string action = parsedArguments[0];
        if (action == "step")
        {
            return new SimulateStep(stoi(parsedArguments.at(1)));
        }
        else if (action == "plan")
        {
            return new AddPlan(parsedArguments.at(1), parsedArguments.at(2));
        }
        else if (action == "settlement")
        {
            return new AddSettlement(parsedArguments.at(1), SettlementType(stoi(parsedArguments.at(2))));
        }
        else if (action == "facility")
        {
            return new AddFacility(parsedArguments.at(1), FacilityCategory(stoi(parsedArguments.at(2))), stoi(parsedArguments.at(3)), stoi(parsedArguments.at(4)), stoi(parsedArguments.at(5)), stoi(parsedArguments.at(6)));
        }
        else if (action == "planStatus")
        {
            return new PrintPlanStatus(stoi(parsedArguments.at(1)));
        }
        else if (action == "changePolicy")
        {
            return new ChangePlanPolicy(stoi(parsedArguments.at(1)), parsedArguments.at(2));
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
        else
        {
            std::cout << "Invalid action " << action << std::endl;
        }
    }
    catch (const runtime_error &e)
    {
        std::cout << "Invalid action" << std::endl;
    }
}
