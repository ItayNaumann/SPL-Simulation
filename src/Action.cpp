#include <Action.h>

#include <iostream>

extern Simulation *backup = nullptr;


// BaseAction
BaseAction::BaseAction(): errorMsg(""), status(ActionStatus::ERROR){}
ActionStatus BaseAction::getStatus() const{
    return status;
}
void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR;
    this -> errorMsg = errorMsg;
}

const string &BaseAction::getErrorMsg() const{
    return errorMsg;
}

// SimulateStep
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps), BaseAction(){}

void SimulateStep::act(Simulation &simulation)
{
    for (int i = 0; i < numOfSteps; i++){
        simulation.step();
    }
}
const string SimulateStep::toString() const
{
    const string output = "step " + std::to_string(numOfSteps) + " " + statusToString(getStatus());
    return output;
}
SimulateStep *SimulateStep::clone() const
{
    SimulateStep *clone = new SimulateStep(numOfSteps);
    return clone;
}

// AddPlan
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy): BaseAction(), settlementName(settlementName), selectionPolicy(selectionPolicy){}
void AddPlan::act(Simulation &simulation){
    SelectionPolicy *encoded = encodePolicy(selectionPolicy);
    if (!simulation.isSettlementExists(settlementName)){
        error("Cannot create this plan");
    }
    else{
        Settlement settlement = simulation.getSettlement(settlementName);
        simulation.addPlan(settlement, encoded);
        complete();
    }
}
const string AddPlan::toString() const{
    const string output = "plan " + settlementName + " " + selectionPolicy + " " + statusToString(getStatus());
    return output;
}
AddPlan *AddPlan::clone() const{
    AddPlan *clone = new AddPlan(settlementName, selectionPolicy);
    return clone;
}

// AddSettlement
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType): BaseAction(), settlementName(settlementName), settlementType(settlementType){}
void AddSettlement::act(Simulation &simulation){
    Settlement *settlement = new Settlement(settlementName,settlementType);
    if (simulation.isSettlementExists(settlementName)){
        error("Settlement already exists");
    }
    else{
        simulation.addSettlement(settlement);
        complete();
    }
}
AddSettlement *AddSettlement::clone() const{
    AddSettlement *clone = new AddSettlement(settlementName, settlementType);
    return clone;
}
const string AddSettlement::toString() const{
    const string output = "settlement " + settlementName + " " + std::to_string(static_cast<int>(settlementType)) + " " + statusToString(getStatus());
    return output;
}

// AddFacility
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : BaseAction(), facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}
void AddFacility::act(Simulation &simulation){
    FacilityType *facility = new FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (!simulation.addFacility(*facility)){
        error("Facility already exists");
    }
    else{
        simulation.addFacility(*facility);
        complete();
    }
}
AddFacility *AddFacility::clone() const{
    AddFacility *clone = new AddFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    return clone;
}
const string AddFacility::toString() const{
    const string output = "facility " + facilityName + " " +  std::to_string(static_cast<int>(facilityCategory)) + " " + 
        std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " 
            + std::to_string(economyScore) + " " + std::to_string(environmentScore) + " " + statusToString(getStatus());
    return output;
}

// PrintPlanStatus
PrintPlanStatus::PrintPlanStatus(int planId) : BaseAction(), planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation){
    if (!simulation.isPlanExists(planId)){
        error("Plan doesn't exist");
    }
    else{
        Plan plan = simulation.getPlan(planId);
        plan.printStatus();
        complete();
    }
    
}
PrintPlanStatus *PrintPlanStatus::clone() const{
    PrintPlanStatus *clone = new PrintPlanStatus(planId);
    return clone;
}
const string PrintPlanStatus::toString() const{
    const string output = "planStatus " + std::to_string(planId) + " " + statusToString(getStatus());
    return output;
}

// ChangePlanPolicy
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) : BaseAction(), planId(planId), newPolicy(newPolicy){}
void ChangePlanPolicy::act(Simulation &simulation){
    SelectionPolicy *encoded = encodePolicy(newPolicy);
    if (encoded->toString() == simulation.getPlan(planId).getSelectionPolicy()->toString()){
        error("Cannot change selection policy");
    }
    else{
        Plan plan = simulation.getPlan(planId);
        plan.setSelectionPolicy(encoded);
        complete();    
    }

}
ChangePlanPolicy *ChangePlanPolicy::clone() const{
    ChangePlanPolicy *clone = new ChangePlanPolicy(planId, newPolicy);
    return clone;
}
const string ChangePlanPolicy::toString() const{
    const string output = "changePolicy " + std::to_string(planId) + " " + newPolicy + " " + statusToString(getStatus());
    return output;
}

// PrintActionsLog
PrintActionsLog::PrintActionsLog() : BaseAction(){}
void PrintActionsLog::act(Simulation &simulation){
    const vector<BaseAction *> &actions = simulation.getActionsLog();
    for (BaseAction * action : actions){
        cout << action->toString() << endl;
    }
    complete();
}
PrintActionsLog *PrintActionsLog::clone() const{
    PrintActionsLog *clone = new PrintActionsLog();
    return clone;
}
const string PrintActionsLog::toString() const{
    const string output = "log " + statusToString(getStatus());
    return output;
}

// Close
Close::Close(): BaseAction(){}
void Close::act(Simulation &simulation){
    simulation.close();
    complete();
}
Close *Close::clone() const{
    Close *clone = new Close();
    return clone;
}
const string Close::toString() const{
    const string output = "close " + statusToString(getStatus());
    return output;
}

// BackupSimulation
BackupSimulation::BackupSimulation() : BaseAction(){}
void BackupSimulation::act(Simulation &simulation) {
    backup = &simulation;
    complete();
}
BackupSimulation *BackupSimulation::clone() const{
    BackupSimulation *clone = new BackupSimulation();
    return clone;
}
const string BackupSimulation::toString() const{
    const string output = "backup " + statusToString(getStatus());
    return output;
}

// RestoreSimulation
RestoreSimulation::RestoreSimulation() : BaseAction(){}
void RestoreSimulation::act(Simulation &simulation){
    if (backup = nullptr){
        error("No backup available");
    }
    else{
        simulation = *backup;
        complete();
    }
}
RestoreSimulation *RestoreSimulation::clone() const{
    RestoreSimulation *clone = new RestoreSimulation();
    return clone;
}
const string RestoreSimulation::toString() const{
    const string output = "restore " + statusToString(getStatus());
    return output;
}

// Encode a string to a SelectionPolicy.
SelectionPolicy *encodePolicy(string selectionPolicy){
    SelectionPolicy *encoded = nullptr;
    if (selectionPolicy == "nve"){
        encoded = new NaiveSelection();
    }   
    else if (selectionPolicy == "bal"){
        encoded = new BalancedSelection(0,0,0);
    }
    else if (selectionPolicy == "eco"){
        encoded = new EconomySelection();
    }
    else if (selectionPolicy == "env"){
        encoded = new SustainabilitySelection();
    }
    return encoded;
}

// Converts ActionStatus to proper string representation
string statusToString(const ActionStatus &status){
    if (status == ActionStatus::COMPLETED){
        return "COMPLETED";
    }
    else if (status == ActionStatus::ERROR){
        return "ERROR";
    }
    else{
        return "";
    }
}