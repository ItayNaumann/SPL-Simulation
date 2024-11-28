#include "Plan.h"
#include <iostream>
#include <algorithm>

void Plan::step()
{
    if (status != PlanStatus::BUSY)
    {
        for (Facility *facility : facilities)
        {
            FacilityStatus facilityStatus = facility->step();
            if (facilityStatus == FacilityStatus::OPERATIONAL)
            {
                underConstruction.erase(
                    std::remove_if(
                        underConstruction.begin(),
                        underConstruction.end(),
                        [&](Facility *node)
                        {
                            return node == facility;
                        }),
                    underConstruction.end());
            }
        }
        status = underConstruction.size() == this->settlement.getConstructionLimit() ? PlanStatus::BUSY : PlanStatus::AVALIABLE;
    }
}