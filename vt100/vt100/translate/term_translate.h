#pragma once
#include "term_utf8.h"

class term_translate
{
public:
	term_translate();
	~term_translate();

public:
	unsigned long translate(unsigned char c);

protected:
	unsigned long to_sco_acs(unsigned char c);
	unsigned long to_charset(unsigned char c);
	
	unsigned long to_line_draw(unsigned char c);
	unsigned long to_uk_ascii(unsigned char c);
	unsigned long to_ascii(unsigned char c);
	unsigned long to_charset_sco_acs(unsigned char c);

protected:
	bool in_utf();
	bool in__sco_acs(unsigned char c);
	
protected:
	term_utf8 m_utf8;
};

