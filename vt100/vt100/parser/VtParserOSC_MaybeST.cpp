/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserOSC_MaybeST.h"


VtParserOSC_MaybeST::VtParserOSC_MaybeST()
{
	
}

VtParserOSC_MaybeST::~VtParserOSC_MaybeST()
{

}

void VtParserOSC_MaybeST::Parse()
{
	term->termstate = VT52_Y2;
	//move(term, term->curs.x, c - ' ', 0);
}
