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
    for (int i = 1; i < facilitiesOptions.size(); i++)
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
    int maxEco = 0;
    int maxIndex = 0;
    for (int i = 0; i < facilitiesOptions.size(); i++)
    {
        if (facilitiesOptions[i].getEconomyScore() > maxEco)
        {
            maxEco = facilitiesOptions[i].getEconomyScore();
            maxIndex = i;
        }
    }
    lastSelectedIndex = maxIndex;
    const FacilityType &output = facilitiesOptions[maxIndex];
    return output;
}

const string EconomySelection::toString() const
{
    return "eco";
}

EconomySelection *EconomySelection::clone() const
{
    EconomySelection *output = new EconomySelection();
    output->lastSelectedIndex = lastSelectedIndex;
    return output;
}

// Sustainability
SustainabilitySelection::SustainabilitySelection()
{
    lastSelectedIndex = -1;
}
const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int maxEnv = 0;
    int maxIndex = 0;
    for (int i = 0; i < facilitiesOptions.size(); i++)
    {
        if (facilitiesOptions[i].getEnvironmentScore() > maxEnv)
        {
            maxEnv = facilitiesOptions[i].getEnvironmentScore();
            maxIndex = i;
        }
    }
    lastSelectedIndex = maxIndex;
    const FacilityType &output = facilitiesOptions[maxIndex];
    return output;
}

const string SustainabilitySelection::toString() const
{
    return "env";
}
SustainabilitySelection *SustainabilitySelection::clone() const
{
    SustainabilitySelection *output = new SustainabilitySelection();
    output->lastSelectedIndex = lastSelectedIndex;
    return output;
}