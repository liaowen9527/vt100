#include "VtCtrlRender.h"
#include "../VtContext.h"

VtCtrlRender::VtCtrlRender()
{
	
}

VtCtrlRender::~VtCtrlRender()
{

}

void VtCtrlRender::Render()
{
	/*
	* A VT100 without the AVO only had one
	* attribute, either underline or
	* reverse video depending on the
	* cursor type, this was selected by
	* CSI 7m.
	*
	* case 2:
	*  This is sometimes DIM, eg on the
	*  GIGI and Linux
	* case 8:
	*  This is sometimes INVIS various ANSI.
	* case 21:
	*  This like 22 disables BOLD, DIM and INVIS
	*
	* The ANSI colours appear on any
	* terminal that has colour (obviously)
	* but the interaction between sgr0 and
	* the colours varies but is usually
	* related to the background colour
	* erase item. The interaction between
	* colour attributes and the mono ones
	* is also very implementation
	* dependent.
	*
	* The 39 and 49 attributes are likely
	* to be unimplemented.
	*/
	
	int nArgs = m_args->Count();
	for (int i = 0; i < nArgs; i++)
	{
		int arg = m_args->GetArg(i, 0);
		switch (arg) 
		{
		case 0:       /* restore defaults */
			Restore();
			break;
		case 1:       /* enable bold */
			if (CheckCompat(VT100AVO)) EnableAttr(ATTR_BOLD, true);
			break;
		case 2:       /* enable dim */
			if (CheckCompat(OTHER)) EnableAttr(ATTR_DIM, true);
			break;
		case 21:      /* (enable double underline) */
			if (CheckCompat(OTHER) && CheckCompat(VT100AVO)) EnableAttr(ATTR_UNDER, true);
		case 4:       /* enable underline */
			if (CheckCompat(VT100AVO)) EnableAttr(ATTR_UNDER, true);
			break;
		case 5:       /* enable blink */
			if (CheckCompat(VT100AVO)) EnableAttr(ATTR_BLINK, true);
			break;
		case 6:       /* SCO light bkgrd */
			Blink();
			break;
		case 7:       /* enable reverse video */
			EnableAttr(ATTR_REVERSE, true);
			break;
		case 10:      /* SCO acs off */
			SetAcsStatus(0);
			break;
		case 11:      /* SCO acs on */
			SetAcsStatus(1);
			break;
		case 12:      /* SCO acs on, |0x80 */
			SetAcsStatus(2);
			break;
		case 22:      /* disable bold and dim */
			if (CheckCompat_2(OTHER, VT220)) EnableAttr(ATTR_BOLD | ATTR_DIM, false);
			break;
		case 24:      /* disable underline */
			if (CheckCompat_2(OTHER, VT220)) EnableAttr(ATTR_UNDER, false);
			break;
		case 25:      /* disable blink */
			if (CheckCompat_2(OTHER, VT220)) EnableAttr(ATTR_BLINK, false);
			break;
		case 27:      /* disable reverse video */
			if (CheckCompat_2(OTHER, VT220)) EnableAttr(ATTR_REVERSE, false);
			break;
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
			/* foreground */
			Foreground(i);
			break;
		case 90:
		case 91:
		case 92:
		case 93:
		case 94:
		case 95:
		case 96:
		case 97:
			AixtermForeground(i);
			break;
		case 39:      /* default-foreground */
			DefaultForeground();
			break;
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
			/* background */
			Background(i);
			break;
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 107:
			AixtermBackground(i);
			break;
		case 49:      /* default-background */
			DefaultBackground();
			break;

			/*
			 * 256-colour and true-colour
			 * sequences. A 256-colour
			 * foreground is selected by a
			 * sequence of 3 arguments in the
			 * form 38;5;n, where n is in the
			 * range 0-255. A true-colour RGB
			 * triple is selected by 5 args of
			 * the form 38;2;r;g;b. Replacing
			 * the initial 38 with 48 in both
			 * cases selects the same colour
			 * as the background.
			 */
		case 38:
			i = Attr_38(i);

			break;
		case 48:
			Attr_48(i);
			break;
		}
	}

	m_ctrl->erase.SetEraseChar();
}

void VtCtrlRender::Restore()
{
	term->curr_attr = term->default_attr;
	term->curr_truecolour = term->basic_erase_char.truecolour;
}

void VtCtrlRender::EnableAttr(int attr, bool enable)
{
	if (enable)
	{
		term->curr_attr |= attr;
	}
	else
	{
		term->curr_attr &= ~attr;
	}
	
}

void VtCtrlRender::Blink()
{
	if (!CheckCompat(SCOANSI))
	{
		return;
	}

	term->blink_is_real = false;
	EnableAttr(ATTR_BLINK, true);
	term_schedule_tblink(term);
}

void VtCtrlRender::SetBlink()
{
	if (!CheckCompat(SCOANSI))
	{
		return;
	}

	term->blink_is_real = false;
	term_schedule_tblink(term);

	bool enable = m_args->GetArg(0) >= 1;
	EnableAttr(ATTR_BLINK, enable);
}

void VtCtrlRender::SetBlinkIsReal()
{
	term->blink_is_real = (term->esc_args[0] >= 1);
}

void VtCtrlRender::SetAcsStatus(int status)
{
	if (!CheckCompat(SCOANSI))
	{
		return;
	}

	if (term->no_remote_charset)
	{
		return;
	}

	term->sco_acs = status;
}

void VtCtrlRender::Foreground(int i)
{
	term->curr_truecolour.fg.enabled = false;
	term->curr_attr &= ~ATTR_FGMASK;
	term->curr_attr |= (term->esc_args[i] - 30) << ATTR_FGSHIFT;
}

void VtCtrlRender::AixtermForeground(int i)
{
	/* aixterm-style bright foreground */
	term->curr_truecolour.fg.enabled = false;
	term->curr_attr &= ~ATTR_FGMASK;
	term->curr_attr |= ((term->esc_args[i] - 90 + 8) << ATTR_FGSHIFT);
}

void VtCtrlRender::DefaultForeground()
{
	term->curr_truecolour.fg.enabled = false;
	term->curr_attr &= ~ATTR_FGMASK;
	term->curr_attr |= ATTR_DEFFG;
}

void VtCtrlRender::NormalForeground()
{
	compatibility(SCOANSI);
	if (term->esc_args[0] < 16) 
	{
		long colour = (sco2ansicolour[term->esc_args[0] & 0x7] | (term->esc_args[0] & 0x8)) << ATTR_FGSHIFT;
		term->curr_attr &= ~ATTR_FGMASK;
		term->curr_attr |= colour;
		term->curr_truecolour.fg = optionalrgb_none;
		term->default_attr &= ~ATTR_FGMASK;
		term->default_attr |= colour;
		set_erase_char(term);
	}
}

void VtCtrlRender::Background(int i)
{
	term->curr_truecolour.bg.enabled = false;
	term->curr_attr &= ~ATTR_BGMASK;
	term->curr_attr |= (term->esc_args[i] - 40) << ATTR_BGSHIFT;
}

void VtCtrlRender::AixtermBackground(int i)
{
	/* aixterm-style bright background */
	term->curr_truecolour.bg.enabled = false;
	term->curr_attr &= ~ATTR_BGMASK;
	term->curr_attr |= ((term->esc_args[i] - 100 + 8) << ATTR_BGSHIFT);
}

void VtCtrlRender::DefaultBackground()
{
	term->curr_truecolour.bg.enabled = false;
	term->curr_attr &= ~ATTR_BGMASK;
	term->curr_attr |= ATTR_DEFBG;
}

void VtCtrlRender::NomalBackground()
{
	compatibility(SCOANSI);
	if (term->esc_args[0] < 16) 
	{
		long colour = (sco2ansicolour[term->esc_args[0] & 0x7] | (term->esc_args[0] & 0x8)) << ATTR_BGSHIFT;
		term->curr_attr &= ~ATTR_BGMASK;
		term->curr_attr |= colour;
		term->curr_truecolour.bg = optionalrgb_none;
		term->default_attr &= ~ATTR_BGMASK;
		term->default_attr |= colour;
		set_erase_char(term);
	}
}

int VtCtrlRender::Attr_38(int i)
{
	int nArgs = m_args->Count();

	do 
	{
		if (nArgs <= i + 2)
		{
			break;
		}

		int arg_1 = m_args->GetArg(i + 1);
		if (arg_1 == 5)
		{
			term->curr_attr &= ~ATTR_FGMASK;
			term->curr_attr |= ((term->esc_args[i + 2] & 0xFF) << ATTR_FGSHIFT);
			term->curr_truecolour.fg = optionalrgb_none;
			i += 2;
		}
	} while (0);
	
	do 
	{
		if (nArgs <= i + 4)
		{
			break;
		}

		int arg_1 = m_args->GetArg(i + 1);
		if (arg_1 == 2)
		{
			term->curr_attr &= ~ATTR_FGMASK;
			term->curr_attr |= ((term->esc_args[i + 2] & 0xFF) << ATTR_FGSHIFT);
			term->curr_truecolour.fg = optionalrgb_none;
			i += 2;
		}
	} while (0);

	return i;
}

int VtCtrlRender::Attr_48(int i)
{
	int nArgs = m_args->Count();

	do
	{
		if (nArgs <= i + 2)
		{
			break;
		}

		int arg_1 = m_args->GetArg(i + 1);
		if (arg_1 == 5)
		{
			term->curr_attr &= ~ATTR_BGMASK;
			term->curr_attr |= ((term->esc_args[i + 2] & 0xFF) << ATTR_BGSHIFT);
			term->curr_truecolour.bg = optionalrgb_none;
			i += 2;
		}
	} while (0);

	do
	{
		if (nArgs <= i + 4)
		{
			break;
		}

		int arg_1 = m_args->GetArg(i + 1);
		if (arg_1 == 2)
		{
			parse_optionalrgb(&term->curr_truecolour.bg, term->esc_args + (i + 2));
			i += 4;
		}
	} while (0);

	return i;
}