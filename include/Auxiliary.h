#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "Action.h"

class Auxiliary
{
public:
    static std::vector<std::string> parseArguments(const std::string &line);
    static BaseAction parseToAction(std::vector<std::string> &parsedArguments);
};
