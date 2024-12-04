#pragma once
#include "Action.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Auxiliary
{
public:
    static vector<string> parseArguments(const string &line);
    static BaseAction *parseToAction(vector<string> &parsedArguments);
    static void initialParsing(const string &configFilePath, Simulation &simulation);
};
