#include <Action.h>
#include <typeinfo>

#include <iostream>

extern Simulation *backup;

// BaseAction
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::ERROR) {}
ActionStatus BaseAction::getStatus() const
{
    return status;
}
void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string newErrorMsg)
{
    status = ActionStatus::ERROR;
    errorMsg = newErrorMsg;
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}

// SimulateStep
SimulateStep::SimulateStep(const int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
}
const string SimulateStep::toString() const
{
    const string output = "step " + std::to_string(numOfSteps) + " " + statusToString();
    return output;
}
SimulateStep *SimulateStep::clone() const
{
    SimulateStep *clone = new SimulateStep(numOfSteps);
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}

// AddPlan
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : BaseAction(), settlementName(settlementName), selectionPolicy(selectionPolicy) {}
void AddPlan::act(Simulation &simulation)
{
    if (!simulation.isSettlementExists(settlementName))
    {
        error("Cannot create this plan");
        cout << "Error: " << this->getErrorMsg() << endl;
    }
    else
    {
        SelectionPolicy *policy = SelectionPolicyFactory::createPolicy(selectionPolicy, 0, 0, 0);
        Settlement &settlement = simulation.getSettlement(settlementName);
        simulation.addPlan(settlement, policy);
        complete();
    }
}
const string AddPlan::toString() const
{
    const string output = "plan " + settlementName + " " + selectionPolicy + " " + statusToString();
    return output;
}
AddPlan *AddPlan::clone() const
{
    AddPlan *clone = new AddPlan(settlementName, selectionPolicy);
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}

// AddSettlement
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : BaseAction(), settlementName(settlementName), settlementType(settlementType) {}
void AddSettlement::act(Simulation &simulation)
{
    Settlement *settlement = new Settlement(settlementName, settlementType);
    if (simulation.isSettlementExists(settlementName))
    {
        error("Settlement already exists");
        cout << "Error: " << this->getErrorMsg() << endl;
    }
    else
    {
        simulation.addSettlement(settlement);
        complete();
    }
}
AddSettlement *AddSettlement::clone() const
{
    AddSettlement *clone = new AddSettlement(settlementName, settlementType);
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}
const string AddSettlement::toString() const
{
    const string output = "settlement " + settlementName + " " + std::to_string((int)(settlementType)) + " " + statusToString();
    return output;
}

// AddFacility
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : BaseAction(), facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}
void AddFacility::act(Simulation &simulation)
{
    FacilityType facility = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (!simulation.addFacility(facility))
    {
        error("Facility already exists");
        cout << "Error: " << this->getErrorMsg() << endl;
    }
    else
    {
        simulation.addFacility(facility);
        complete();
    }
}
AddFacility *AddFacility::clone() const
{
    AddFacility *clone = new AddFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}
const string AddFacility::toString() const
{
    const string output = "facility " + facilityName + " " + std::to_string((int)(facilityCategory)) + " " +
                          std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " + std::to_string(environmentScore) + " " + statusToString();
    return output;
}

// PrintPlanStatus
PrintPlanStatus::PrintPlanStatus(int planId) : BaseAction(), planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation)
{
    if (!simulation.isPlanExists(planId))
    {
        error("Plan doesn't exist");
        cout << "Error: " << this->getErrorMsg() << endl;
    }
    else
    {
        Plan &plan = simulation.getPlan(planId);
        plan.printStatus();
        complete();
    }
}
PrintPlanStatus *PrintPlanStatus::clone() const
{
    PrintPlanStatus *clone = new PrintPlanStatus(planId);
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}
const string PrintPlanStatus::toString() const
{
    const string output = "planStatus " + std::to_string(planId) + " " + statusToString();
    return output;
}

// ChangePlanPolicy
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) : BaseAction(), planId(planId), newPolicy(newPolicy) {}
void ChangePlanPolicy::act(Simulation &simulation)
{
    Plan &plan = simulation.getPlan(planId);
    SelectionPolicy *policy = SelectionPolicyFactory::createPolicy(newPolicy, plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore());

    if (policy->toString() == plan.getSelectionPolicy()->toString())
    {
        delete policy;
        error("Cannot change selection policy");
        cout << "Error: " << this->getErrorMsg() << endl;
    }
    else
    {
        Plan &plan = simulation.getPlan(planId);
        cout << "planID: " << plan.getPlanId() << endl;
        cout << "previousPolicy: " << plan.getSelectionPolicy()->toLongString() << endl;
        plan.setSelectionPolicy(policy);
        cout << "newPolicy: " << plan.getSelectionPolicy()->toLongString() << endl;

        complete();
    }
}
ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    ChangePlanPolicy *clone = new ChangePlanPolicy(planId, newPolicy);
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}
const string ChangePlanPolicy::toString() const
{
    const string output = "changePolicy " + std::to_string(planId) + " " + newPolicy + " " + statusToString();
    return output;
}

// PrintActionsLog
PrintActionsLog::PrintActionsLog() : BaseAction() {}
void PrintActionsLog::act(Simulation &simulation)
{
    const vector<BaseAction *> &actions = simulation.getActionsLog();
    for (BaseAction *action : actions)
    {
        cout << action->toString() << endl;
    }
    complete();
}
PrintActionsLog *PrintActionsLog::clone() const
{
    PrintActionsLog *clone = new PrintActionsLog();
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}
const string PrintActionsLog::toString() const
{
    const string output = "log " + statusToString();
    return output;
}

// Close
Close::Close() : BaseAction() {}
void Close::act(Simulation &simulation)
{
    simulation.close();
    complete();
}
Close *Close::clone() const
{
    Close *clone = new Close();
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}
const string Close::toString() const
{
    const string output = "close " + statusToString();
    return output;
}

// BackupSimulation
BackupSimulation::BackupSimulation() : BaseAction() {}
void BackupSimulation::act(Simulation &simulation)
{
    backup = new Simulation(simulation);
    complete();
}
BackupSimulation *BackupSimulation::clone() const
{
    BackupSimulation *clone = new BackupSimulation();
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}
const string BackupSimulation::toString() const
{
    const string output = "backup " + statusToString();
    return output;
}

// RestoreSimulation
RestoreSimulation::RestoreSimulation() : BaseAction() {}
void RestoreSimulation::act(Simulation &simulation)
{
    if (backup == nullptr)
    {
        error("No backup available");
        cout << "Error: " << this->getErrorMsg() << endl;
    }
    else
    {
        simulation = *backup;
        complete();
    }
}

RestoreSimulation *RestoreSimulation::clone() const
{
    RestoreSimulation *clone = new RestoreSimulation();
    if (this->getStatus() == ActionStatus::COMPLETED)
    {
        clone->complete();
    }
    else
    {
        clone->error(this->getErrorMsg());
    }
    return clone;
}

const string RestoreSimulation::toString() const
{
    const string output = "restore " + statusToString();
    return output;
}

// Converts ActionStatus to proper string representation
string BaseAction::statusToString() const
{
    if (this->status == ActionStatus::COMPLETED)
    {
        return "COMPLETED";
    }
    else if (this->status == ActionStatus::ERROR)
    {
        return "ERROR";
    }
    else
    {
        return "";
    }
}
