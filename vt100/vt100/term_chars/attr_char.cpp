#include "attr_char.h"



attr_char::attr_char()
{

}

attr_char::~attr_char()
{

}

void attr_char::enable(int attr, bool _enable)
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

bool attr_char::is_enable(int attr)
{
	return m_attr & attr;
}

bool attr_char::is_narrow()
{
	return is_enable(ATTR_NARROW);
}

bool attr_char::is_wide()
{
	return is_enable(ATTR_WIDE);
}

bool attr_char::is_bold()
{
	return is_enable(ATTR_BOLD);
}

bool attr_char::is_under()
{
	return is_enable(ATTR_UNDER);
}

bool attr_char::is_reverse()
{
	return is_enable(LATTR_NORM);
}

bool attr_char::is_blink()
{
	return is_enable(LATTR_NORM);
}

bool attr_char::is_fgmask()
{
	return is_enable(LATTR_NORM);
}

bool attr_char::is_bgmask()
{
	return is_enable(LATTR_NORM);
}

bool attr_char::is_colours()
{
	return is_enable(LATTR_NORM);
}

bool attr_char::is_dim()
{
	return is_enable(LATTR_NORM);
}
