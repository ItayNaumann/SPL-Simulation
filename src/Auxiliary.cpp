#include "Auxiliary.h"
#include <iostream>
#include <vector>
using namespace std;
/*
This is a 'static' method that receives a string(line) and returns a vector of the string's arguments.

For example:
parseArguments("settlement KfarSPL 0") will return vector with ["settlement", "KfarSPL", "0"]

To execute this method, use Auxiliary::parseArguments(line)
*/

std::vector<std::string> Auxiliary::parseArguments(const std::string &line)
{
    std::vector<std::string> arguments;
    std::istringstream stream(line);
    std::string argument;

    while (stream >> argument)
    {
        arguments.push_back(argument);
    }

    return arguments;
}

BaseAction Auxiliary::parseToAction(std::vector<std::string> &parsedArguments)
{
    string action = parsedArguments[0];
    if (action == "step")
    {
        return SimulateStep(stoi(parsedArguments[1]));
    }
    else if (action == "plan")
    {
        return AddPlan(parsedArguments[1], parsedArguments[2]);
    }
    else if (action == "settlement")
    {
        return AddSettlement(parsedArguments[1], SettlementType(stoi(parsedArguments[2])));
    }
    else if (action == "facility")
    {
        return AddFacility(parsedArguments[1], FacilityCategory(stoi(parsedArguments[2])), stoi(parsedArguments[3]), stoi(parsedArguments[4]), stoi(parsedArguments[5]), stoi(parsedArguments[6]));
    }
    else if (action == "planStatus")
    {
        return PrintPlanStatus(stoi(parsedArguments[1]));
    }
    else if (action == "changePolicy")
    {
        return ChangePlanPolicy(stoi(parsedArguments[1]), parsedArguments[2]);
    }
    else if (action == "log")
    {
        return PrintActionsLog();
    }
    else if (action == "close")
    {
        return Close();
    }
    else if (action == "backup")
    {
        return BackupSimulation();
    }
    else if (action == "restore")
    {
        return RestoreSimulation();
    }
    else
    {
        return BaseAction();
    }
}
