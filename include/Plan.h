#pragma once
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <vector>
using namespace std;

enum class PlanStatus
{
    AVALIABLE,
    BUSY,
};

class Plan
{
public:
    Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
    const int getlifeQualityScore() const;
    const int getEconomyScore() const;
    const int getEnvironmentScore() const;
    void setSelectionPolicy(SelectionPolicy *selectionPolicy);
    void step();
    void printStatus();
    const vector<Facility *> &getFacilities() const;
    void addFacility(Facility *facility);
    const string toString() const;

    // getters
    PlanStatus getPlanStatus() const;
    const Settlement &getSettlement() const;
    vector<Facility *> &getUnderConstruction();
    SelectionPolicy *getSelectionPolicy();
    const vector<FacilityType> &getFacilityOptions() const;
    const int getPlanId() const;

    // rule of 5
    Plan(const Plan &other, const vector<FacilityType> &facilityOptions);
    ~Plan();
    Plan &operator=(const Plan &other) = delete;

private:
    int plan_id;
    const Settlement &settlement;
    SelectionPolicy *selectionPolicy; // What happens if we change this to a reference? You cant change it if its a reference
    PlanStatus status;
    vector<Facility *> facilities;
    vector<Facility *> underConstruction;
    const vector<FacilityType> &facilityOptions;
    int life_quality_score, economy_score, environment_score;
};