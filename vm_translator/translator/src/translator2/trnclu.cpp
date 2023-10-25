
#include "trnclu.h"

namespace vmtr
{

	static int32_t gInstanceCounter{ 0 };

	vmins Equal()
	{
		sform cmd{};

		const std::string lTrue{ "eqTrue_" + std::to_string(gInstanceCounter) };
		const std::string lSkip{ "eqSkip_" + std::to_string(gInstanceCounter++) };

		cmd.Add("D=D-M");
		cmd.Add("@" + lTrue);
		cmd.Add("D;JEQ");
		cmd.Add("D=0");
		cmd.Add("@" + lSkip);
		cmd.Add("0;JMP");
		cmd.Add("(" + lTrue + ")");
		cmd.Add("D=-1");
		cmd.Add("(" + lSkip + ")");

		return cmd.GetContent();
	}

	vmins GreaterThan()
	{
		sform cmd{};

		const std::string lTrue{ "gtTrue_" + std::to_string(gInstanceCounter) };
		const std::string lSkip{ "gtSkip_" + std::to_string(gInstanceCounter++) };

		cmd.Add("D=D-M");
		cmd.Add("@" + lTrue);
		cmd.Add("D;JGT");
		cmd.Add("D=0");
		cmd.Add("@" + lSkip);
		cmd.Add("0;JMP");
		cmd.Add("(" + lTrue + ")");
		cmd.Add("D=-1");
		cmd.Add("(" + lSkip + ")");

		return cmd.GetContent();
	}

	vmins LessThan()
	{
		sform cmd{};

		const std::string lTrue{ "ltTrue_" + std::to_string(gInstanceCounter) };
		const std::string lSkip{ "ltSkip_" + std::to_string(gInstanceCounter++) };

		cmd.Add("D=D-M");
		cmd.Add("@" + lTrue);
		cmd.Add("D;JLT");
		cmd.Add("D=0");
		cmd.Add("@" + lSkip);
		cmd.Add("0;JMP");
		cmd.Add("(" + lTrue + ")");
		cmd.Add("D=-1");
		cmd.Add("(" + lSkip + ")");

		return cmd.GetContent();
	}

}
