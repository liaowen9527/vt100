/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParser.h"
#include "../define/VtType.h"

VtParser::VtParser()
{
	
}

VtParser::~VtParser()
{

}

void VtParser::Parse(const std::string& str)
{
	Parse(str.c_str(), str.length());
}

void VtParser::Parse(const char* str, int len)
{
	const char* str_old = str;
	int len_old = len;
	while (len)
	{
		InputChar(*str);

		str = str + 1;
		--len;
	}

	DoAction();
	m_context.buffer.Reset();
}

void VtParser::InputChar(char ch)
{
	if (m_bPrinting)
	{
		DoPainting();
	}
	if (m_term.termstate == TOPLEVEL)
	{
		m_vtTrans.Translate(ch);
		if (m_vtTrans.IsBreak())
		{
			return;
		}
	}
	DoSpecialCase();

	if (IsC0Control(ch))
	{
		C0Control(ch);
	}
	else
	{
		NormalControl(ch);
	}
}

void VtParser::DoPainting()
{

}

void VtParser::DoSpecialCase(char& c)
{
	/*
		 * How about C1 controls?
		 * Explicitly ignore SCI (0x9a), which we don't translate to DECID.
		 */
	if ((c & -32) == 0x80 && m_term.termstate < DO_CTRLS && !m_term.vt52_mode && vt_type::IsCompat(CL_VT220))
	{
		if (c == 0x9a)
		{
			c = 0;
		}	
		else 
		{
			m_term.termstate = SEEN_ESC;
			m_term.esc_query = FALSE;
			c = '@' + (c & 0x1F);
		}
	}
}

void VtParser::GLControl(char& c)
{
	//if (term->curs.x && !term->wrapnext)
	//{
	//	term->curs.x--;
	//}
	//	
	//term->wrapnext = FALSE;
	///* destructive backspace might be disabled */
	//if (!term->no_dbackspace) 
	//{
	//	check_boundary(term, term->curs.x, term->curs.y);
	//	check_boundary(term, term->curs.x + 1, term->curs.y);
	//	copy_termchar(scrlineptr(term->curs.y), term->curs.x, &term->erase_char);
	//}
}

void VtParser::C0Control(char& c)
{

}

void VtParser::NormalControl(char& c)
{
	if (IsC0Control(c))
	{
		C0Control(c);
	}

	switch (m_term.termstate)
	{
	case TOPLEVEL:
		m_vtTopLevel.Parse(c);
		break;
	case SEEN_ESC:
		m_vtEsc.Parse(c);
		break;
	case SEEN_CSI:
		m_vtCsi.Parse(c);
		break;
	case SEEN_OSC:
		m_vtOsc.Parse(c);
		break;
	case SEEN_OSC_W:
		m_vtOscW.Parse(c);
		break;
	case SEEN_OSC_P:
		m_vtOscP.Parse(c);
		break;
	case OSC_STRING:
		m_vtOscStr.Parse(c);
		break;
	case OSC_MAYBE_ST:
		m_vtMaybeST.Parse(c);
		break;
	case VT52_ESC:
		m_vt52Esc.Parse(c);
		break;
	case VT52_Y1:
		m_vt52Y1.Parse(c);
		break;
	case VT52_Y2:
		m_vt52Y1.Parse(c);
		break;
	case VT52_FG:
		m_vt52FG.Parse(c);
		break;
	case VT52_BG:
		m_vt52BG.Parse(c);
		break;
	default:
		break;
	}
}

bool VtParser::IsGLControl(char c)
{
	return c == '\177' && m_term.termstate < DO_CTRLS && vt_type::IsCompat(CL_OTHER);
}

bool VtParser::IsC0Control(char c)
{
	return (c & ~0x1F) == 0 && m_term.termstate < DO_CTRLS;
}