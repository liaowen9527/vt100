#pragma once
#include <string>
#include "parser/VtContext.h"

class VtParser
{
public:
	VtParser();
	~VtParser();

public:
	void Parser(const std::string& str);

protected:
	void InputChar(char ch);
	void DoAction();

protected:
	VtContext m_context;

	std::vector<VtFmtAction> m_actions;
	std::vector<std::string> m_actions2;
};

