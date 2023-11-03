#pragma once

#include <jackc.h>
#include "token/jline.h"

#include <iostream>
#include <string>
#include <fstream>

namespace jcom
{

	void AnalyzeFile(std::ifstream& inFile)
	{
		std::string str{ };
		while (std::getline(inFile, str))
		{
			jline line{ str };
			token t{};
			while (line.Next(t))
			{
				std::cout << "Rec: " << t << "\n";
			}
		}
	}

}
