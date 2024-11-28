#include <Facility.h>


// FacilityType
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score){
    FacilityType::name = &name;
    FacilityType::category = category;
    FacilityType::price = price;
    FacilityType::lifeQuality_score = lifeQuality_score;
    FacilityType::economy_score = economy_score;
    Facility::environment_score = environment_score;
}
const string& FacilityType::getName(){
    const string& output = &name;
    return output;
}
int FacilityType::getCost(){
    return FacilityType::name;
}
int FacilityType::getLifeQualityScore(){
    return FacilityType::lifeQuality_score;
}
int FacilityType::getEconomyScore(){
    return FacilityType::economy_score;
}
int FacilityType::getEnvironmentScore(){
    return FacilityType::environment_score;
}
FacilityCategory FacilityType::getCategory(){
    return FacilityType::category;
}
// add rule of 5/3

// Facility
    Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score){
        FacilityType::FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score);
        Facility::settlementName = &settlementName;
        Facility::status = FacilityStatus::UNDER_CONSTRUCTIONS;
    }
    Facility::Facility(const FacilityType &type, const string &settlementName){
        // Copy constructor??

        Facility::settlementName = &settlementName;
    }
    const string& Facility::getSettlementName(){
        const string& output = &Facility::settlementName;
        return output;
    }
    const int getTimeLeft(){
        return Facility::timeLeft;
    }
    FacilityStatus Facility::step(){
        // tf is this
    }
    void Facility::setStatus(FacilityStatus status){
        Facility::status = status;
    }
    const FacilityStatus& Facility::getStatus(){
        FacilityStatus &output = Facility::status;
        return output;
    }
    const string Facility::toString(){
        // unknown info
    }

