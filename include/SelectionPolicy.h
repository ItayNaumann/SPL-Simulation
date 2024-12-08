#pragma once
#include "Facility.h"
#include <vector>
#include <string>
using namespace std;

class SelectionPolicy
{
public:
    virtual const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) = 0;
    virtual const string toString() const = 0;
    virtual const string toLongString() const = 0;
    virtual SelectionPolicy *clone() const = 0;
    virtual ~SelectionPolicy() = default;
};

class NaiveSelection : public SelectionPolicy
{
public:
    NaiveSelection();
    const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
    const string toString() const override;
    const string toLongString() const override;
    NaiveSelection *clone() const override;
    ~NaiveSelection() override = default;

private:
    int lastSelectedIndex;
};

class BalancedSelection : public SelectionPolicy
{
public:
    BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
    const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
    const string toString() const override;
    const string toLongString() const override;
    BalancedSelection *clone() const override;
    ~BalancedSelection() override = default;
    void updateScores(int LifeQualityScore, int EconomyScore, int EnvironmentScore);

private:
    int LifeQualityScore;
    int EconomyScore;
    int EnvironmentScore;

    // Auxiliary methods
    int distance(const FacilityType &Facility);
};

class EconomySelection : public SelectionPolicy
{
public:
    EconomySelection();
    const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
    const string toString() const override;
    const string toLongString() const override;
    EconomySelection *clone() const override;
    ~EconomySelection() override = default;

private:
    int lastSelectedIndex;
};

class SustainabilitySelection : public SelectionPolicy
{
public:
    SustainabilitySelection();
    const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
    const string toString() const override;
    const string toLongString() const override;
    SustainabilitySelection *clone() const override;
    ~SustainabilitySelection() override = default;

private:
    int lastSelectedIndex;
};

class SelectionPolicyFactory
{
public:
    static SelectionPolicy *createPolicy(const string &selectionPolicy, int LifeQualityScore, int EconomyScore, int EnvironmentScore);
};