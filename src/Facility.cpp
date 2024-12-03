#include <Facility.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

// FacilityType
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score)
{
}
const string &FacilityType::getName() const
{
    return name;
}
int FacilityType::getCost() const
{
    return price;
}
int FacilityType::getLifeQualityScore() const
{
    return FacilityType::lifeQuality_score;
}
int FacilityType::getEconomyScore() const
{
    return FacilityType::economy_score;
}
int FacilityType::getEnvironmentScore() const
{
    return FacilityType::environment_score;
}
FacilityCategory FacilityType::getCategory() const
{
    return FacilityType::category;
}


// Facility
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price,
                   const int lifeQuality_score, const int economy_score, const int environment_score)
    : settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS),
      FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score) { timeLeft = getCost(); }

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS) { timeLeft = getCost(); }
const string &Facility::getSettlementName() const
{
    return settlementName;
}
const int Facility::getTimeLeft() const
{
    return timeLeft;
}
FacilityStatus Facility::step()
{
    timeLeft--;
    if (timeLeft == 0)
    {
        Facility::status = FacilityStatus::OPERATIONAL;
    }
}
void Facility::setStatus(FacilityStatus status)
{
    Facility::status = status;
}
const FacilityStatus &Facility::getStatus() const
{
    return status;
}
const string Facility::toString() const
{
    std::stringstream ss;
    ss << "facility ";
    ss << name << " ";
    ss << static_cast<int>(category) << " ";
    ss << price << " ";
    ss << lifeQuality_score << " ";
    ss << economy_score << " ";
    ss << environment_score;
    return ss.str();
}
