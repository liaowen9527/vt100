/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserVT52_Y2.h"


VtParserVT52_Y2::VtParserVT52_Y2()
{
	
}

VtParserVT52_Y2::~VtParserVT52_Y2()
{

}

void VtParserVT52_Y2::Parse()
{
	term->termstate = VT52_Y2;
	//move(term, term->curs.x, c - ' ', 0);
}
