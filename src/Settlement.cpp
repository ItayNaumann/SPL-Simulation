#include <Settlement.h>
#include <string>

Settlement::Settlement(const string &name, SettlementType type):name(name),type(type){
    // might work
}

const string& Settlement::getName(){
    return Settlement::name;
}

SettlementType Settlement::getType(){
    return Settlement::type;
}

const string Settlement::toString(){
    // order unknown, may need to add toString for SettlementType.
}

const int Settlement::getConstructionLimit(){
    enum SettlementType t = Settlement::type;
    return t+1;
    // I think this works.
}

// r5
Settlement::~Settlement(){
    //add stuff
    delete name;
    delete type;
}
