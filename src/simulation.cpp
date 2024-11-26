#include "Simulation.h"

void Simulation::step()
{
    for (Plan &plan : plans)
    {
        plan.step();
    }
}
