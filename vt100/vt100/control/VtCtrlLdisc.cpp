#include "VtCtrlLdisc.h"
#include "../VtContext.h"

VtCtrlLdisc::VtCtrlLdisc()
{
	
}

VtCtrlLdisc::~VtCtrlLdisc()
{

}

void VtCtrlLdisc::QueryType()
{
	compatibility(VT100);
	/* This is the response for a VT102 */
	if (term->ldisc && term->id_string[0])
	{
		ldisc_send(term->ldisc, term->id_string, strlen(term->id_string), false);
	}
}

void VtCtrlLdisc::QueryType2()
{
	/*
	* Strictly speaking this is VT100 but a VT100 defaults to
	* no response. Other terminals respond at their option.
	*
	* Don't put a CR in the default string as this tends to
	* upset some weird software.
	*/
	if (!CheckCompat(ANSIMIN))
	{
		return;
	}
	
	VtLdisc* ldisc = m_term->Ldisc();
	if (ldisc) 
	{
		std::string buf;
		//strbuf *buf = term_input_data_from_charset(term, DEFAULT_CODEPAGE, term->answerback, term->answerbacklen);
		ldisc->Send(buf, false);
	}
}

void VtCtrlLdisc::QueryCursorPos()
{
	if (term->ldisc) 
	{
		int arg = m_args->GetArg(0);
		if (arg == 6)
		{
			char buf[32] = '\0';
			sprintf(buf, "\033[%d;%dR", m_cursor->Row() + 1, m_cursor->Col() + 1);
			ldisc_send(term->ldisc, buf, strlen(buf), false);
		}
		else if (arg == 5)
		{
			ldisc_send(term->ldisc, "\033[0n", 4, false);
		}
	}
}

void VtCtrlLdisc::ReportCharacter()
{
	compatibility(VT100);
	if (term->ldisc) 
	{
		char buf[32];
		int i = def(term->esc_args[0], 0);
		if (i == 0 || i == 1) 
		{
			strcpy(buf, "\033[2;1;1;112;112;1;0x");
			buf[2] += i;
			ldisc_send(term->ldisc, buf, 20, false);
		}
	}
}

void VtCtrlLdisc::Send()
{
	if (term->ldisc && term->id_string[0])
	{
		ldisc_send(term->ldisc, term->id_string, strlen(term->id_string), false);
	}
}
