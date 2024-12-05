#include <Settlement.h>
#include <string>

Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type)
{
    // might work
}

const string &Settlement::getName() const
{
    return Settlement::name;
}

SettlementType Settlement::getType() const
{
    return Settlement::type;
}

const string Settlement::toString() const
{
    // order unknown, may need to add toString for SettlementType.
}

const int Settlement::getConstructionLimit() const
{
    return static_cast<int>(type) + 1;
    // I think this works.
}
