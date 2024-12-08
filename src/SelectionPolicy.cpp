#include <SelectionPolicy.h>
#include <vector>

// Naive
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}
const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex++;
    lastSelectedIndex = lastSelectedIndex % facilitiesOptions.size();
    const FacilityType &selected = facilitiesOptions.at(lastSelectedIndex);
    return selected;
}

const string NaiveSelection::toString() const
{
    return "nve";
}
const string NaiveSelection::toLongString() const
{
    return "Naive";
}

NaiveSelection *NaiveSelection::clone() const
{
    NaiveSelection *output = new NaiveSelection();
    output->lastSelectedIndex = lastSelectedIndex;
    return output;
}

// Balenced
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore),
                                                                                                     EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int minDist = distance(facilitiesOptions[0]);
    int minIndex = 0;
    // Find minIndex
    for (int i = 1; i < (int)facilitiesOptions.size(); i++)
    {
        if (distance(facilitiesOptions[i]) < minDist)
        {
            minDist = distance(facilitiesOptions[i]);
            minIndex = i;
        }
    }
    const FacilityType &minFacility = facilitiesOptions[minIndex];
    return minFacility;
}
const string BalancedSelection::toString() const
{
    return "bal";
}
const string BalancedSelection::toLongString() const
{
    return "Balanced";
}
BalancedSelection *BalancedSelection::clone() const
{
    BalancedSelection *output = new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore);
    return output;
}
int BalancedSelection::distance(const FacilityType &facility)
{
    int newLifeQ = LifeQualityScore + facility.getLifeQualityScore();
    int newEco = EconomyScore + facility.getEconomyScore();
    int newEnv = EnvironmentScore + facility.getEnvironmentScore();

    int dist = abs(newLifeQ - newEco);
    if (abs(newEco - newEnv) > dist)
    {
        dist = abs(newEco - newEnv);
    }
    if (abs(newEnv - newLifeQ) > dist)
    {
        dist = abs(newEnv - newLifeQ);
    }
    return dist;
}

void BalancedSelection::updateScores(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
{
    this->LifeQualityScore += LifeQualityScore;
    this->EconomyScore += EconomyScore;
    this->EnvironmentScore += EnvironmentScore;
}

int abs(int num)
{
    if (num < 0)
    {
        return -num;
    }
    else
    {
        return num;
    }
}

// Economic
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}
const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    for (int i = (lastSelectedIndex + 1); i < (int)facilitiesOptions.size(); i = (i + 1) % ((int)facilitiesOptions.size()))
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ECONOMY)
        {
            lastSelectedIndex = i;
            return facilitiesOptions[i];
        }
    }
    throw "No economy facilities found";
}

const string EconomySelection::toString() const
{
    return "eco";
}
const string EconomySelection::toLongString() const
{
    return "Economy";
}

EconomySelection *EconomySelection::clone() const
{
    EconomySelection *output = new EconomySelection();
    output->lastSelectedIndex = lastSelectedIndex;
    return output;
}

// Sustainability
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1)
{
}
const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    for (int i = (lastSelectedIndex + 1); i < (int)facilitiesOptions.size(); i = (i + 1) % ((int)facilitiesOptions.size()))
    {
        if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT)
        {
            lastSelectedIndex = i;
            return facilitiesOptions[i];
        }
    }
    throw "No environment facilities found";
}

const string SustainabilitySelection::toString() const
{
    return "env";
}
const string SustainabilitySelection::toLongString() const
{
    return "Sustainability";
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    SustainabilitySelection *output = new SustainabilitySelection();
    output->lastSelectedIndex = lastSelectedIndex;
    return output;
}

SelectionPolicy *SelectionPolicyFactory::createPolicy(const string &selectionPolicy, int LifeQualityScore, int EconomyScore, int EnvironmentScore)
{
    if (selectionPolicy == "nve")
    {
        return new NaiveSelection();
    }
    else if (selectionPolicy == "bal")
    {
        return new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore);
    }
    else if (selectionPolicy == "eco")
    {
        return new EconomySelection();
    }
    else if (selectionPolicy == "env")
    {
        return new SustainabilitySelection();
    }
    else
    {
        return nullptr;
    }
}