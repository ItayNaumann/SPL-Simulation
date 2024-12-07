#include "Plan.h"
#include <iostream>
#include <algorithm>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVAILABLE),
      facilities(vector<Facility *>()), underConstruction(vector<Facility *>()), facilityOptions(facilityOptions),
      life_quality_score(0), economy_score(0), environment_score(0) {}
Plan::Plan(const Plan &other)
    : plan_id(other.getPlanId()), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()),
      status(other.status), facilities(vector<Facility *>()), underConstruction(vector<Facility *>()), facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score),
      economy_score(other.economy_score), environment_score(other.environment_score)
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
Plan::Plan(const Plan &other, const vector<FacilityType> &facilityOptions)
    : plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy),
      status(other.status), facilities(vector<Facility *>()), underConstruction(vector<Facility *>()), facilityOptions(facilityOptions),
      life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score)
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

Plan::Plan(const Plan &&other)
    : plan_id(other.plan_id), settlement(std::move(other.settlement)), selectionPolicy(other.selectionPolicy),
      status(other.status), facilities(std::move(other.facilities)), underConstruction(std::move(other.underConstruction)),
      facilityOptions(std::move(other.facilityOptions)), life_quality_score(other.life_quality_score),
      economy_score(other.economy_score), environment_score(other.environment_score)
{
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

const Settlement &Plan::getSettlement() const
{
    return settlement;
}

const vector<FacilityType> &Plan::getFacilityOptions() const
{
    return facilityOptions;
}

void Plan::printStatus()
{
}

SelectionPolicy *Plan::getSelectionPolicy()
{
    return selectionPolicy;
}

void Plan::step()
{

    for (int i = underConstruction.size(); i < settlement.getConstructionLimit(); i++)
    {
        FacilityType facilityType = selectionPolicy->selectFacility(facilityOptions);
        underConstruction.push_back(new Facility(facilityType, settlement.getName()));
    }

    for (auto facility = underConstruction.begin(); facility != underConstruction.end();)
    {
        FacilityStatus facilityStatus = (*facility)->step();
        if (facilityStatus == FacilityStatus::OPERATIONAL)
        {

            facilities.push_back(*facility);
            underConstruction.erase(facility);
            life_quality_score += (*facility)->getLifeQualityScore();
            economy_score += (*facility)->getEconomyScore();
            environment_score += (*facility)->getEnvironmentScore();
        }
        else
        {
            facility++;
        }
    }
    status = (int)underConstruction.size() == settlement.getConstructionLimit() ? PlanStatus::BUSY : PlanStatus::AVAILABLE;
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
