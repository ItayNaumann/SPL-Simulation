#include "Simulation.h"
#include <iostream>

using namespace std;

Simulation *backup = nullptr;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "usage: simulation <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Simulation simulation(configurationFile);
    simulation.start();
    if (backup != nullptr)
    {
        delete backup;
        backup = nullptr;
    }
    return 0;
}