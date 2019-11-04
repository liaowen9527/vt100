#pragma once
#include "term_utf8.h"
#include "unicode_data.h"

class VtTerm;

class term_translate
{
public:
	term_translate();
	~term_translate();

public:
	VtTerm* get_term();
	void set_term(VtTerm* term);

	term_utf8* get_utf8();
	unicode_data* get_ucsdata();

public:
	unsigned long translate(unsigned char c);

public:
	int get_sco_acs();
	unsigned char get_unitab_ctrl(unsigned char c);

	bool is_utf8linedraw();

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
	VtTerm* m_term;
	term_utf8 m_utf8;
	unicode_data m_ucsdata;
};

