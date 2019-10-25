#pragma once
#include "attr_char.h"
#include "truecolour.h"

/* These are internal use overlapping with the UTF-16 surrogates */
#define CSET_ASCII   0x0000D800UL      /* normal ASCII charset ESC ( B */
#define CSET_LINEDRW 0x0000D900UL      /* line drawing charset ESC ( 0 */
#define CSET_SCOACS  0x0000DA00UL      /* SCO Alternate charset */
#define CSET_GBCHR   0x0000DB00UL      /* UK variant   charset ESC ( A */
#define CSET_MASK    0xFFFFFF00UL      /* Character set mask */

class term_char
{
public:
	term_char();
	~term_char();

public:
	int cc_next();

public:
	bool is_wide();
	wchar_t get_wchar();

public:
	unsigned long get_charset();
	int get_ucsdata_index();

	bool is_ascii();
	bool is_linedraw();
	bool is_scoacs();
	bool is_gbchr();
	bool is_mask();

protected:
	unsigned long m_tchar;
	attr_char m_attr;
	truecolour m_colour;

	int m_cc_next;
};