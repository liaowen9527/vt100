/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParser2.h"

VtParser2::VtParser2()
{
	
}

VtParser2::~VtParser2()
{

}

void VtParser2::Parser(const std::string& str)
{
	Parser(str.c_str(), str.length());
}

void VtParser2::Parser(const char* str, int len)
{
	const char* str_old = str;
	int len_old = len;
	while (len)
	{
		InputChar(*str);

		str = str + 1;
		--len;
	}

	DoAction();
	m_context.buffer.Reset();
}

void VtParser2::InputChar(char ch)
{
	m_context.buffer.Input(ch);

	std::vector<VtFmtAction> actions = m_context.fmtManager.Filter(m_actions.empty());
	if (actions.empty())
	{
		DoAction();
	}

	m_actions.swap(actions);
}

void VtParser2::DoAction()
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

	action.Active();
}

