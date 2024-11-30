#include "Plan.h"
#include <iostream>
#include <algorithm>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions), status(PlanStatus::AVALIABLE)
{
    life_quality_score = 0;
    economy_score = 0;
    environment_score = 0;
    facilities = new vector<Facility *>();
    underConstruction = new vector<Facility *>();
}
Plan::Plan(const Plan &other)
    : plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy), facilityOptions(other.facilityOptions), status(other.status), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score)
{
    facilities = new vector<Facility *>();
    underConstruction = new vector<Facility *>();
    for (Facility *facility : other.facilities)
    {
        facilities.push_back(new Facility(*facility));
    }
    for (Facility *facility : other.underConstruction)
    {
        underConstruction.push_back(new Facility(*facility));
    }
}

void Plan::addFacility(Facility *facility)
{
    underConstruction.erase(facility);
    facilities.push_back(facility);
    life_quality_score += facility->getLifeQualityScore();
    economy_score += facility->getEconomyScore();
    environment_score += facility->getEnvironmentScore();
}

void Plan::step()
{
    // technically we can do it every time but it's less efficient? idk we won't use PlanStatus otherwise

    if (status != PlanStatus::BUSY)
    {
        while (underConstruction.size() < settlement.getConstructionLimit())
        {
            FacilityType facilityType = selectionPolicy->selectFacility(facilityOptions);
            underConstruction.push_back(new Facility(facilityType, settlement.getName()));
        }
    }
    for (Facility *facility : facilities)
    {
        FacilityStatus facilityStatus = facility->step();
        if (facilityStatus == FacilityStatus::OPERATIONAL)
        {
            addFaicility(facility);
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

    delete &facilityOptions;
    delete &settlement;
    delete &selectionPolicy;
    delete &status;
    ;
}