#include "VtParser.h"
#include "../data/VtTerm.h"

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

	m_context.buffer.Reset();
}

void VtParser::InputChar(char ch)
{
	unsigned long c = ch;

	if (m_term->GetTermState() == TOPLEVEL)
	{
		unsigned long t = m_vtTrans.translate(ch);
		switch (t)
		{
		case UCS_INCOMPLETE:
			return;       /* didn't complete a multibyte char */
		case UCS_TRUNCATED:
			m_unget = c;
			/* fall through */
		case UCS_INVALID:
			c = UCSERR;
			break;
		default:
			c = t;
			break;
		}
	}

	C1_Control(c);

	if (IsGL_Control(c))
	{
		GL_Control(c);
	}
	else if(IsC0_Control(c))
	{
		m_vtC0.Parse(c);
	}
	else
	{
		NormalControl(c);
	}
	
}

void VtParser::C1_Control(unsigned long& c)
{
	/*
	* How about C1 controls?
	* Explicitly ignore SCI (0x9a), which we don't translate to DECID.
	*/
	if (m_term->GetTermState() >= DO_CTRLS)
	{
		return;
	}
	if (m_term->IsVt52Mode() || !vt_type::IsCompat(CL_VT220))
	{
		return;
	}

	if ((c & -32) == 0x80)
	{
		if (c == 0x9a)
		{
			c = 0;
		}	
		else 
		{
			m_term->SetTermState(SEEN_ESC);
			m_term->SetEscQuery(0);
			c = '@' + (c & 0x1F);
		}
	}
}

void VtParser::GL_Control(unsigned long& c)
{
	//if (term->curs.x && !term->wrapnext)
	//{
	//	term->curs.x--;
	//}
	//	
	//term->wrapnext = false;
	///* destructive backspace might be disabled */
	//if (!term->no_dbackspace) 
	//{
	//	check_boundary(term, term->curs.x, term->curs.y);
	//	check_boundary(term, term->curs.x + 1, term->curs.y);
	//	copy_termchar(scrlineptr(term->curs.y), term->curs.x, &term->erase_char);
	//}
}

void VtParser::NormalControl(unsigned long& c)
{
	switch (m_term->GetTermState())
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

bool VtParser::IsGL_Control(unsigned long c)
{
	return c == '\177' && m_term->GetTermState() < DO_CTRLS && vt_type::IsCompat(CL_OTHER);
}

bool VtParser::IsC0_Control(unsigned long c)
{
	return (c & ~0x1F) == 0 && m_term->GetTermState() < DO_CTRLS;
}