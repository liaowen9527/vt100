#pragma once
#include <string>
#include "../VtContext.h"

class VtParser
{
public:
	VtParser();
	~VtParser();

public:
	void Parser(const std::string& str);
	void Parser(const char* str, int len);

protected:
	void InputChar(char ch);
	void DoAction();

protected:
	VtContext m_context;

	std::vector<VtFmtAction> m_actions;
	std::vector<std::string> m_actions2;
};

