#pragma once

// Translation clusters
#include "sform.h"
#include <string>


#define D_REG ("D")
#define A_REG ("A")
#define M_REG ("M")

#define H_CONST ("constant")
#define H_LOCAL ("local")
#define H_ARGS ("argument")
#define H_THIS ("this")
#define H_THAT ("that")
#define H_STATIC ("static")
#define H_TEMP ("temp")
#define H_POINTER ("pointer")

#define H_PUSH ("push")
#define H_POP ("pop")
#define H_LABEL ("label")

#define H_GOTO ("goto")
#define H_IFGO ("if-goto")

#define H_CALL ("call")
#define H_FUNC ("function")
#define H_RETURN ("return")

using vmins = std::string;

namespace vmtr
{
	vmins LessThan();
	vmins GreaterThan();
	vmins Equal();
}
