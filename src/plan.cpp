#include "Plan.h"
#include <iostream>
#include <algorithm>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions), status(PlanStatus::AVALIABLE), facilities(), 
        underConstruction(), life_quality_score(0), economy_score(0), environment_score(0){}
        
Plan::Plan(const Plan &other, const vector<FacilityType> &facilityOptions)
    : plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy),
      facilityOptions(facilityOptions), status(other.status), life_quality_score(other.life_quality_score),
      economy_score(other.economy_score), environment_score(other.environment_score), facilities(), underConstruction()
{
    for (Facility *facility : other.facilities)
    {
        facilities.push_back(new Facility(*facility));
    }
    for (Facility *facility : other.underConstruction)
    {
        underConstruction.push_back(new Facility(*facility));
    }
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *newSelectionPolicy)
{
    delete this->selectionPolicy;
    this->selectionPolicy = newSelectionPolicy;
}

const int Plan::getPlanId() const
{
    return plan_id;
}

void Plan::step()
{
    // technically we can do it every time but it's less efficient? idk we won't use PlanStatus otherwise

    while (underConstruction.size() < settlement.getConstructionLimit())
    {
        FacilityType facilityType = selectionPolicy->selectFacility(facilityOptions);
        underConstruction.push_back(new Facility(facilityType, settlement.getName()));
    }

    for (Facility *facility : facilities)
    {
        FacilityStatus facilityStatus = facility->step();
        if (facilityStatus == FacilityStatus::OPERATIONAL)
        {
            underConstruction.erase(std::find(facilities.begin(),facilities.end(), *facility));
            facilities.push_back(facility);
            life_quality_score += facility->getLifeQualityScore();
            economy_score += facility->getEconomyScore();
            environment_score += facility->getEnvironmentScore();
        }
    }
    status = underConstruction.size() == settlement.getConstructionLimit() ? PlanStatus::BUSY : PlanStatus::AVALIABLE;
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

// not sure about this code lol
Plan::~Plan()
{

    for (Facility *facility : facilities)
    {
        delete facility;
    }
    delete &facilities;
    for (Facility *facility : underConstruction)
    {
        delete facility;
    }
    delete &underConstruction;

    delete selectionPolicy;
    delete &status;
}
