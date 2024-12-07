#include "Auxiliary.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
/*
This is a 'static' method that receives a string(line) and returns a vector of the string's arguments.

For example:
parseArguments("settlement KfarSPL 0") will return vector with ["settlement", "KfarSPL", "0"]

To execute this method, use Auxiliary::parseArguments(line)
*/

vector<string> Auxiliary::parseArguments(const string &line)
{
    vector<string> arguments;
    istringstream stream(line);
    string argument;

    while (stream >> argument)
    {
        arguments.push_back(argument);
    }

    return arguments;
}