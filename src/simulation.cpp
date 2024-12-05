#include "Simulation.h"
#include "Auxiliary.h"

#include <vector>
#include <string>
#include <iostream>
using namespace std;

Simulation::Simulation(const string &configFilePath)
{
    planCounter = 0;
    Auxiliary::initialParsing(configFilePath, *this);
}

void Simulation::start()
{
    isRunning = true;
    while (isRunning)
    {
        string line;
        getline(std::cin, line);
        vector<string> arguments = Auxiliary::parseArguments(line);
        BaseAction *action = Auxiliary::parseToAction(arguments);
        action->act(*this);
        actionsLog.push_back(action);
    }
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
    for (FacilityType facilityType : facilitiesOptions)
    {
        if (facilityType.getName() == facility.getName())
        {
            return false;
        }
    }

    facilitiesOptions.push_back(facility);
    return true;
}

Plan &Simulation::getPlan(const int planID)
{
    for (Plan &plan : plans)
    {
        if (plan.getPlanId() == planID)
        {
            return plan;
        }
    }
}

Simulation &Simulation::operator=(const Simulation &other)
{
    if (this != &other)
    {
        isRunning = other.isRunning;
        planCounter = other.planCounter;

        for (FacilityType facility : facilitiesOptions)
        {
            delete &facility;
        }
        delete &facilitiesOptions;
        facilitiesOptions = vector<FacilityType>();
        for (FacilityType facility : other.facilitiesOptions)
        {
            facilitiesOptions.push_back(FacilityType(facility));
        }

        for (BaseAction *action : actionsLog)
        {
            delete action;
        }
        delete &actionsLog;
        actionsLog = vector<BaseAction *>();
        for (BaseAction *action : other.actionsLog)
        {
            actionsLog.push_back(action->clone());
        }

        for (Plan plan : plans)
        {
            delete &plan;
        }
        plans = vector<Plan>();
        for (Plan plan : other.plans)
        {
            plans.push_back(Plan(plan, this->facilitiesOptions));
        }

        for (Settlement *settlement : settlements)
        {
            delete settlement;
        }
        delete &settlements;
        settlements = vector<Settlement *>();
        for (Settlement *settlement : other.settlements)
        {
            settlements.push_back(new Settlement(*settlement));
        }
    }
    return *this;
}

Simulation::~Simulation()
{
    for (FacilityType facility : facilitiesOptions)
    {
        delete &facility;
    }
    delete &facilitiesOptions;

    for (BaseAction *action : actionsLog)
    {
        delete action;
    }
    delete &actionsLog;

    for (Plan plan : plans)
    {
        delete &plan;
    }
    delete &plans;

    for (Settlement *settlement : settlements)
    {
        delete settlement;
    }
    delete &settlements;
}

Simulation::Simulation(const Simulation &other)
{
    isRunning = other.isRunning;
    planCounter = other.planCounter;

    for (FacilityType facility : facilitiesOptions)
    {
        delete &facility;
    }
    delete &facilitiesOptions;
    facilitiesOptions = vector<FacilityType>();
    for (FacilityType facility : other.facilitiesOptions)
    {
        facilitiesOptions.push_back(FacilityType(facility));
    }

    for (BaseAction *action : actionsLog)
    {
        delete action;
    }
    delete &actionsLog;
    actionsLog = vector<BaseAction *>();
    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }

    for (Plan plan : plans)
    {
        delete &plan;
    }
    plans = vector<Plan>();
    for (Plan plan : other.plans)
    {
        plans.push_back(Plan(plan, this->facilitiesOptions));
    }

    for (Settlement *settlement : settlements)
    {
        delete settlement;
    }
    delete &settlements;
    settlements = vector<Settlement *>();
    for (Settlement *settlement : other.settlements)
    {
        settlements.push_back(new Settlement(*settlement));
    }
}
