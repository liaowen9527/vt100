/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserOSC_String.h"
#include "../define/VtType.h"

VtParserOSC_String::VtParserOSC_String()
{
	
}

VtParserOSC_String::~VtParserOSC_String()
{

}

void VtParserOSC_String::Parse()
{
	/*
	* This OSC stuff is EVIL. It takes just one character to get into
	* sysline mode and it's not initially obvious how to get out.
	* So I've added CR and LF as string aborts.
	* This shouldn't effect compatibility as I believe embedded
	* control characters are supposed to be interpreted (maybe?)
	* and they don't display anything useful anyway.
	*
	* -- RDB
	*/
	if (c == '\012' || c == '\015') 
	{
		term->termstate = TOPLEVEL;
	}
	else if (c == 0234 || c == '\007') 
	{
		/*
		 * These characters terminate the string; ST and BEL
		 * terminate the sequence and trigger instant
		 * processing of it, whereas ESC goes back to SEEN_ESC
		 * mode unless it is followed by \, in which case it is
		 * synonymous with ST in the first place.
		 */
		do_osc(term);
		term->termstate = TOPLEVEL;
	}
	else if (c == '\033')
	{
		term->termstate = OSC_MAYBE_ST;
	}
	else if (term->osc_strlen < OSC_STR_MAX)
	{
		term->osc_string[term->osc_strlen++] = (char)c;
	}
}
