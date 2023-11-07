#pragma once

#include <jackc.h>
#include "token/jline.h"

#include <iostream>
#include <string>
#include <fstream>

namespace jcom
{

	void AnalyzeFile(std::ifstream& inFile, std::ofstream& outFile);

}
