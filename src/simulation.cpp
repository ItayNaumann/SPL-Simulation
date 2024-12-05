#include "Simulation.h"
#include "Auxiliary.h"

#include <vector>
#include <string>
using namespace std;

Simulation::Simulation(const string &configFilePath)
{
    planCounter = 0;
    Auxiliary::initialParsing(configFilePath, *this);
}

void Simulation::step()
{
    for (Plan &plan : plans)
    {
        plan.step();
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    Plan plan(planCounter++, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(plan);
}

bool Simulation::addSettlement(Settlement *settlement)
{
    if (isSettlementExists(settlement->getName()))
    {
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for (Settlement *settlement : settlements)
    {
        if (settlement->getName() == settlementName)
        {
            return true;
        }
    }
    return false;
}

bool Simulation::addFacility(FacilityType facility)
{

    facilitiesOptions.push_back(facility);
}