#include "char_attr.h"


attr_line::attr_line()
{

}

attr_line::~attr_line()
{

}

void attr_line::enable(int attr, bool _enable)
{
	if (_enable)
	{
		m_attr |= attr;
	}
	else
	{
		m_attr &= ~attr;
	}
}

bool attr_line::is_enable(int attr)
{
	return m_attr & attr != LATTR_NORM;
}

bool attr_line::is_normal()
{
	return m_attr == LATTR_NORM;
}

bool attr_line::is_wide()
{
	return is_enable(LATTR_WIDE);
}

bool attr_line::is_top()
{
	return is_enable(LATTR_TOP);
}

bool attr_line::is_bottom()
{
	return is_enable(LATTR_BOT);
}

bool attr_line::is_mode()
{
	return is_enable(LATTR_MODE);
}

bool attr_line::is_wrapped()
{
	return is_enable(LATTR_WRAPPED);
}

bool attr_line::is_wrapped2()
{
	return is_enable(LATTR_WRAPPED2);
}
