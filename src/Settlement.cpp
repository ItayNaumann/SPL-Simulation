#include <Settlement.h>
#include <string>

Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type) {}

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
    const string output = name + std::to_string(static_cast<int>(type));
    return output;
}

const int Settlement::getConstructionLimit() const
{
    return static_cast<int>(type) + 1;
}
