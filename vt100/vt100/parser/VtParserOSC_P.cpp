/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserOSC_P.h"


VtParserOSC_P::VtParserOSC_P()
{
	
}

VtParserOSC_P::~VtParserOSC_P()
{

}

void VtParserOSC_P::Parse()
{
	int max = (term->osc_strlen == 0 ? 21 : 15);
	int val;
	if ((int)c >= '0' && (int)c <= '9')
	{
		val = c - '0';
	}
	else if ((int)c >= 'A' && (int)c <= 'A' + max - 10)
	{
		val = c - 'A' + 10;
	}
	else if ((int)c >= 'a' && (int)c <= 'a' + max - 10)
	{
		val = c - 'a' + 10;
	}
	else 
	{
		term->termstate = TOPLEVEL;
		return;
	}

	term->osc_string[term->osc_strlen++] = val;
	if (term->osc_strlen >= 7) 
	{
		palette_set(term->frontend, term->osc_string[0],
			term->osc_string[1] * 16 + term->osc_string[2],
			term->osc_string[3] * 16 + term->osc_string[4],
			term->osc_string[5] * 16 + term->osc_string[6]);
		term_invalidate(term);
		term->termstate = TOPLEVEL;
	}
}
