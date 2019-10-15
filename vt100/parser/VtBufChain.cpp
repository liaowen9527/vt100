#include "VtBufChain.h"
/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/


VtBufChain::VtBufChain()
{

}

VtBufChain::~VtBufChain()
{

}

void VtBufChain::Input(char ch)
{
	std::string temp(1, ch);

	m_buffer.append(temp);
	if ('0' <= ch && ch <= '9')
	{
		if (m_fmt.empty() || m_fmt.back() != VT_NUM)
		{
			m_fmt.append(VT_STR_NUM);
			m_attrs.push_back(temp);
		}
		else
		{
			m_attrs.back().append(temp);
		}
	}
	else
	{
		m_fmt.append(temp);
	}
}

void VtBufChain::Reset()
{
	m_buffer = "";
	m_fmt = "";
}

void VtBufChain::Recal()
{
	m_buffer = std::string(1, m_buffer.back());
	m_fmt = std::string(1, m_fmt.back());
}

int VtBufChain::GetAttrsCount()
{
	return m_attrs.size();
}

int VtBufChain::GetIntAttr(int nIndex)
{
	return atoi(m_attrs[nIndex].c_str());
}
