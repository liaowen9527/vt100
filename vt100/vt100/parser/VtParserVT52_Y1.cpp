/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserVT52_Y1.h"
#include "../define/VtType.h"

VtParserVT52_Y1::VtParserVT52_Y1()
{
	
}

VtParserVT52_Y1::~VtParserVT52_Y1()
{

}

void VtParserVT52_Y1::Parse()
{
	term->termstate = VT52_Y1;
	//move(term, term->curs.x, c - ' ', 0);
}
