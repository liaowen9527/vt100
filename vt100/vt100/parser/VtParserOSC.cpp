/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserOSC.h"


VtParserOSC::VtParserOSC()
{
	
}

VtParserOSC::~VtParserOSC()
{

}

void VtParserOSC::Parse()
{
	term->osc_w = false;

	switch (c) {
	case 'P':	       /* Linux palette sequence */
		Palette();
		break;
	case 'R':	       /* Linux palette reset */
		ResetPalette();
		break;
	case 'W':	       /* word-set */
		WordSet();
		break;
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
	case 'L':
		Title();
		break;
		/* else fall through */
	default:
		OscString();
		break;
	}
}

void VtParserOSC::Palette()
{

}

void VtParserOSC::ResetPalette()
{

}

void VtParserOSC::WordSet()
{

}

void VtParserOSC::Args()
{

}

void VtParserOSC::Title()
{
	/*
	* Grotty hack to support xterm and DECterm title
	* sequences concurrently.
	*/
	if (term->esc_args[0] == 2) 
	{
		term->esc_args[0] = 1;
	}
	else
	{
		OscString();
	}
}

void VtParserOSC::OscString()
{
	term->termstate = OSC_STRING;
	term->osc_strlen = 0;
}
