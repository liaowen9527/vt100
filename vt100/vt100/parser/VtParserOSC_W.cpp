/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserOSC_W.h"
#include "../define/VtType.h"

VtParserOSC_W::VtParserOSC_W()
{
	
}

VtParserOSC_W::~VtParserOSC_W()
{

}

void VtParserOSC_W::Parse()
{
	switch (c) 
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		Args();
		break;
	default:
		OscString();
		break;
	}
}

void VtParserOSC_W::Args()
{
	if (term->esc_args[0] <= UINT_MAX / 10 &&
		term->esc_args[0] * 10 <= UINT_MAX - c - '0')
	{
		term->esc_args[0] = 10 * term->esc_args[0] + c - '0';
	}
	else
	{
		term->esc_args[0] = UINT_MAX;
	}
}

void VtParserOSC_W::OscString()
{
	term->termstate = OSC_STRING;
	term->osc_strlen = 0;
}
