#pragma once

class CodePage
{
public:
	static int Decode(char *cp_name);
	static std::string Name(int codepage);
};

