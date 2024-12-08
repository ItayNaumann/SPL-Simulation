#include "Simulation.h"
#include "Auxiliary.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(vector<BaseAction *>()), plans(vector<Plan>()), settlements(vector<Settlement *>()), facilitiesOptions(vector<FacilityType>())
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
                this->addSettlement(settlement);
            }
            else if (arguments.at(0) == "facility")
            {
                int type = stoi(arguments.at(2));
                FacilityType facility(arguments.at(1), FacilityCategory(type), stoi(arguments.at(3)), stoi(arguments.at(4)), stoi(arguments.at(5)), stoi(arguments.at(6)));
                this->addFacility(facility);
            }
            else if (arguments.at(0) == "plan")
            {
                Settlement &settlement = this->getSettlement(arguments.at(1));
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
                this->addPlan(settlement, selectionPolicy);
            }
        }
    }
    file.close();
    isRunning = true;
}

void Simulation::start()
{
    isRunning = true;
    std::cout << "The simulation has started" << std::endl;
    BaseAction *action;
    while (isRunning)
    {
        string line;
        getline(std::cin, line);
        vector<string> arguments = Auxiliary::parseArguments(line);
        action = nullptr;
        try
        {
            string parsedAction = arguments.at(0);
            if (parsedAction == "step")
            {
                action = new SimulateStep(stoi(arguments.at(1)));
            }
            else if (parsedAction == "plan")
            {
                action = new AddPlan(arguments.at(1), arguments.at(2));
            }
            else if (parsedAction == "settlement")
            {
                action = new AddSettlement(arguments.at(1), SettlementType(stoi(arguments.at(2))));
            }
            else if (parsedAction == "facility")
            {
                action = new AddFacility(arguments.at(1), FacilityCategory(stoi(arguments.at(2))), stoi(arguments.at(3)), stoi(arguments.at(4)), stoi(arguments.at(5)), stoi(arguments.at(6)));
            }
            else if (parsedAction == "planStatus")
            {
                action = new PrintPlanStatus(stoi(arguments.at(1)));
            }
            else if (parsedAction == "changePolicy")
            {
                action = new ChangePlanPolicy(stoi(arguments.at(1)), arguments.at(2));
            }
            else if (parsedAction == "log")
            {
                action = new PrintActionsLog();
            }
            else if (parsedAction == "close")
            {
                action = new Close();
            }
            else if (parsedAction == "backup")
            {
                action = new BackupSimulation();
            }
            else if (parsedAction == "restore")
            {
                action = new RestoreSimulation();
            }
            else
            {
                std::cout << "Invalid action " << parsedAction << std::endl;
            }
            if (action == nullptr)
                continue;

            action->act(*this);
            actionsLog.push_back(action);
        }
        catch (const runtime_error &e)
        {
            std::cout << "Invalid action" << std::endl;
        }
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
    Plan plan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(plan);
    planCounter++;
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
    throw "Error: Plan doesn't exists";
}

Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (Settlement *settlement : settlements)
    {
        if (settlement->getName() == settlementName)
        {
            return *settlement;
        }
    }
    throw "Settlement not found";
}

void Simulation::close()
{
    isRunning = false;
}

void Simulation::open()
{
    isRunning = true;
}

const vector<BaseAction *> Simulation::getActionsLog() const
{
    return actionsLog;
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

bool Simulation::isPlanExists(const int planId)
{
    for (Plan plan : plans)
    {
        if (plan.getPlanId() == planId)
        {
            return true;
        }
    }
    return false;
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
        facilitiesOptions.clear();
        for (FacilityType facility : other.facilitiesOptions)
        {
            facilitiesOptions.push_back(FacilityType(facility));
        }

        for (BaseAction *action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();
        for (BaseAction *action : other.actionsLog)
        {
            actionsLog.push_back(action->clone());
        }

        plans.clear();
        for (Plan plan : other.plans)
        {
            Plan newPlan(plan, this->facilitiesOptions);
            plans.push_back(newPlan);
        }

        for (Settlement *settlement : settlements)
        {
            delete settlement;
        }
        settlements.clear();
        for (Settlement *settlement : other.settlements)
        {
            settlements.push_back(new Settlement(*settlement));
        }
    }
    return *this;
}

Simulation::~Simulation()
{
    // for (FacilityType facility : facilitiesOptions)
    // {
    //     delete &facility;
    // }

    // for (Plan plan : plans)
    // {
    //     delete &plan;
    // }

    for (BaseAction *action : actionsLog)
    {
        delete action;
    }

    for (Settlement *settlement : settlements)
    {
        delete settlement;
    }
}

Simulation::Simulation(const Simulation &other) : isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(vector<BaseAction *>()), plans(vector<Plan>()), settlements(vector<Settlement *>()), facilitiesOptions(vector<FacilityType>())
{
    for (FacilityType facility : other.facilitiesOptions)
    {
        facilitiesOptions.push_back(FacilityType(facility));
    }

    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }

    for (Plan plan : other.plans)
    {
        plans.push_back(Plan(plan, this->facilitiesOptions));
    }

    settlements = vector<Settlement *>();
    for (Settlement *settlement : other.settlements)
    {
        settlements.push_back(new Settlement(*settlement));
    }
}

Simulation::Simulation(Simulation &&other) : isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(std::move(other.actionsLog)),
                                             plans(std::move(other.plans)), settlements(std::move(other.settlements)), facilitiesOptions(std::move(other.facilitiesOptions))
{
    // Move pointers to null
    for (BaseAction *&ba : other.actionsLog)
    {
        ba = nullptr;
    }
    other.actionsLog.clear();
}

Simulation &Simulation::operator=(Simulation &&other)
{
    if (this != &other)
    {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        actionsLog = std::move(other.actionsLog);
        plans = std::move(other.plans);
        settlements = std::move(other.settlements);
        facilitiesOptions = std::move(other.facilitiesOptions);

        // Clear the source object
        other.isRunning = false;
        other.planCounter = 0;
        other.actionsLog.clear();
        other.plans.clear();
        other.settlements.clear();
        other.facilitiesOptions.clear();
    }
    return *this;
}
