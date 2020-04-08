#include "StringUtils.h"

namespace Utils
{
	std::string toString(int i)
	{
		return std::to_string(i);
	}

	std::string toString(float f)
	{
		char tmp0[16];
		sprintf_s(tmp0, "%.4f", f);
		return std::string(tmp0);
	}
}