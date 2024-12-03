#include <Action.h>

// SimulateStep
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps)
{
}

void SimulateStep::act(Simulation &simulation)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        for (Plan &plan : simulation.getPlans())
        {
            plan.step();
        }
    }
}
const string SimulateStep::toString() const
{
    // Do stuff
}
SimulateStep *SimulateStep::clone() const
{
}