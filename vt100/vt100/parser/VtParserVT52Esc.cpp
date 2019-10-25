/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserOSC_P.h"


VtParserVT52Esc::VtParserVT52Esc()
{
	
}

VtParserVT52Esc::~VtParserVT52Esc()
{

}

void VtParserVT52Esc::Parse()
{
	term->termstate = TOPLEVEL;
	
	switch (c) {
	case 'A':
		Up();
		break;
	case 'B':
		Down();
		break;
	case 'C':
		Right();
		break;
	case 'D':
		Left();
		break;
		/*
		 * From the VT100 Manual
		 * NOTE: The special graphics characters in the VT100
		 *       are different from those in the VT52
		 *
		 * From VT102 manual:
		 *       137 _  Blank             - Same
		 *       140 `  Reserved          - Humm.
		 *       141 a  Solid rectangle   - Similar
		 *       142 b  1/                - Top half of fraction for the
		 *       143 c  3/                - subscript numbers below.
		 *       144 d  5/
		 *       145 e  7/
		 *       146 f  Degrees           - Same
		 *       147 g  Plus or minus     - Same
		 *       150 h  Right arrow
		 *       151 i  Ellipsis (dots)
		 *       152 j  Divide by
		 *       153 k  Down arrow
		 *       154 l  Bar at scan 0
		 *       155 m  Bar at scan 1
		 *       156 n  Bar at scan 2
		 *       157 o  Bar at scan 3     - Similar
		 *       160 p  Bar at scan 4     - Similar
		 *       161 q  Bar at scan 5     - Similar
		 *       162 r  Bar at scan 6     - Same
		 *       163 s  Bar at scan 7     - Similar
		 *       164 t  Subscript 0
		 *       165 u  Subscript 1
		 *       166 v  Subscript 2
		 *       167 w  Subscript 3
		 *       170 x  Subscript 4
		 *       171 y  Subscript 5
		 *       172 z  Subscript 6
		 *       173 {  Subscript 7
		 *       174 |  Subscript 8
		 *       175 }  Subscript 9
		 *       176 ~  Paragraph
		 *
		 */
	case 'F':
		Charset(CSET_LINEDRW);
		break;
	case 'G':
		Charset(CSET_ASCII);
		break;
	case 'H':
		Home();
		break;
	case 'I':
		ReverseLine();
		break;
	case 'J':
		EraseEos();
		break;
	case 'K':
		EraseEol();
		break;
#if 0
	case 'V':
		/* XXX Print cursor line */
		break;
	case 'W':
		/* XXX Start controller mode */
		break;
	case 'X':
		/* XXX Stop controller mode */
		break;
#endif
	case 'Y':
		Into_VT52_Y1();
		break;
	case 'Z':
		Identify();
		break;
	case '=':
		KeypadApp(true);
		break;
	case '>':
		KeypadApp(false);
		break;
	case '<':
		ExitVT52();
		break;
#if 0
	case '^':
		/* XXX Enter auto print mode */
		break;
	case '_':
		/* XXX Exit auto print mode */
		break;
	case ']':
		/* XXX Print screen */
		break;
#endif

#ifdef VT52_PLUS
	case 'E':
		HomeAndEraseEol();
		break;
	case 'L':
		ScrollUp();
		break;
	case 'M':
		ScrollDown();
		break;
	case 'b':
		Into_VT52_FG();
		break;
	case 'c':
		Into_VT52_BG();
		break;
	case 'd':
		EraseSos();
		break;
	case 'e':
		CursorOn(true);
		break;
	case 'f':
		CursorOn(false);
		break;
		/* case 'j': Save cursor position - broken on ST */
		/* case 'k': Restore cursor position */
	case 'l':
		EraseLine();
		break;
	case 'o':
		EraseSol();
		break;
	case 'p':
		SetAttr(ATTR_REVERSE);
		break;
	case 'q':
		RemoveAttr(ATTR_REVERSE);
		break;
	case 'v':	       /* wrap Autowrap on - Wyse style */
		Wrap(true);
		break;
	case 'w':	       /* Autowrap off */
		Wrap(false);
		break;
	case 'R':
		ResetEraseChar();
		break;
	case 'S':
		SetAttr(ATTR_UNDER);
		break;
	case 'W':
		RemoveAttr(ATTR_UNDER);
		break;
	case 'U':
		Bold(true);
	case 'T':
		Bold(false);
		break;
#endif
	}
}

void VtParserVT52Esc::Up()
{

}

void VtParserVT52Esc::Down()
{

}

void VtParserVT52Esc::Right()
{

}

void VtParserVT52Esc::Left()
{

}

void VtParserVT52Esc::Home()
{

}

void VtParserVT52Esc::Charset(int attr)
{

}

void VtParserVT52Esc::ReverseLine()
{

}

void VtParserVT52Esc::ResetEraseChar()
{

}

void VtParserVT52Esc::EraseEol()
{

}

void VtParserVT52Esc::EraseEos()
{

}

void VtParserVT52Esc::EraseSol()
{

}

void VtParserVT52Esc::EraseSos()
{

}

void VtParserVT52Esc::EraseLine()
{

}

void VtParserVT52Esc::Into_VT52_Y1()
{

}

void VtParserVT52Esc::Into_VT52_FG()
{

}

void VtParserVT52Esc::Into_VT52_BG()
{

}

void VtParserVT52Esc::Identify()
{

}

void VtParserVT52Esc::KeypadApp(bool open)
{

}

void VtParserVT52Esc::ExitVT52()
{

}

void VtParserVT52Esc::HomeAndEraseEol()
{

}

void VtParserVT52Esc::ScrollUp()
{

}

void VtParserVT52Esc::ScrollDown()
{

}

void VtParserVT52Esc::CursorOn(bool open)
{

}

void VtParserVT52Esc::SetAttr(int attr)
{

}

void VtParserVT52Esc::RemoveAttr(int attr)
{

}

void VtParserVT52Esc::Bold(bool value)
{

}

void VtParserVT52Esc::Wrap(bool value)
{

}
