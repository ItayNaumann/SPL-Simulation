#include "Plan.h"
#include <iostream>

void Plan::step()
{
    if (status != PlanStatus::BUSY)
    {
        for (Facility *facility : facilities)
        {
            FacilityStatus facilityStatus = facility->step();
            if (facilityStatus == FacilityStatus::OPERATIONAL)
            {
                // facilities.erase(
                //     std::remove_if(
                //         facilities.begin(),
                //         facilities.end(),
                //         [&](Facility const &node)
                //         {
                //             return node. == candidateNode.id && node.position == candidateNode.position;
                //         }),
                //     candidateNodes.end());
            }
        }
        status = underConstruction.size() == this->settlement.getConstructionLimit() ? PlanStatus::BUSY : PlanStatus::AVALIABLE;
    }
}