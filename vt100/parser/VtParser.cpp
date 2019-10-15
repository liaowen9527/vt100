/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParser.h"

VtParser::VtParser()
{
	
}

VtParser::~VtParser()
{

}

void VtParser::Parser(const std::string& str)
{
	const char* temp = str.c_str();
	int len = str.length();

	while (len)
	{
		InputChar(*temp);

		temp = temp + 1;
		--len;
	}

	DoAction();
}

void VtParser::InputChar(char ch)
{
	m_context.buffer.Input(ch);

	std::vector<VtFmtAction> actions = m_context.fmtManager.Filter(false);
	if (actions.empty())
	{
		DoAction();
	}

	m_actions.swap(actions);
}

void VtParser::DoAction()
{
	//write char
	if (m_actions.empty())
	{
		m_context.buffer.Reset();
		return;
	}

	//do action
	VtFmtAction action = m_actions.front();
	m_actions2.push_back(m_context.buffer.m_buffer);

	m_context.buffer.Recal();
}

