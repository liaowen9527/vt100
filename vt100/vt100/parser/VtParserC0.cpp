/*
https://www2.ccs.neu.edu/research/gpc/VonaUtils/vona/terminal/vtansi.htm
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/

#include "VtParserC0.h"

VtParserC0::VtParserC0()
{
	
}

VtParserC0::~VtParserC0()
{

}

void VtParserC0::Parse()
{
	if ((c & ~0x1F) != 0 || term->termstate >= DO_CTRLS)
	{
		return;
	}

	switch (c)
	{
	case '\005':             /* ENQ: terminal type query */
		m_ldisc->QueryType2();
		break;
	case '\007':            /* BEL: Bell */
		m_ctrlTerm->Bell();
		break;
	case '\b':              /* BS: Back space */
		m_cursor->Backspace();
		break;
	case '\016':            /* LS1: Locking-shift one */
		m_ctrlTerm->LockShift(1);
		break;
	case '\017':            /* LS0: Locking-shift zero */
		m_ctrlTerm->LockShift(0);
		break;
	case '\033':            /* ESC: Escape */
		m_ctrlTerm->EnterEsc();
		break;
	case '\015':            /* CR: Carriage return */
		m_ctrlTerm->KeyCR();
		break;
	case '\014':            /* FF: Form feed */
		m_ctrlTerm->FormFeed();
		break;
	case '\013':            /* VT: Line tabulation */
		m_ctrlTerm->LineTab();
		break;
	case '\012':            /* LF: Line feed */
		m_ctrlTerm->KeyLF();
		break;
	case '\t':              /* HT: Character tabulation */
		m_ctrlTerm->KeyTab();
		break;
	default:
		break;
	}
}
