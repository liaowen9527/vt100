#include "VtLdisc.h"

VtLdisc::VtLdisc()
{

}

VtLdisc::~VtLdisc()
{

}

void VtLdisc::Send(const char* buf, int len, bool interactive)
{
	int keyflag = 0;

	if (interactive)
	{
		term->NoPaste();
	}

	/*
	 * Less than zero means null terminated special string.
	 */
	if (len < 0) 
	{
#define CTRL(x) (x^'@')
#define KCTRL(x) ((x^'@') | 0x100)
		len = strlen(buf);
		keyflag = KCTRL('@');
	}
	/*
	 * Either perform local editing, or just send characters.
	 */
#define EDITING (ldisc->localedit == FORCE_ON || (ldisc->localedit == AUTO && (backend_ldisc_option_state(ldisc->backend, LD_EDIT))))
	if (EDITING) 
	{
		while (len--) {
			int c;
			c = (unsigned char)(*buf++) + keyflag;
			if (!interactive && c == '\r')
				c += KCTRL('@');
			switch (ldisc->quotenext ? ' ' : c) {
				/*
				 * ^h/^?: delete, and output BSBs, to return to
				 * last character boundary (in UTF-8 mode this may
				 * be more than one byte)
				 * ^w: delete, and output BSBs, to return to last
				 * space/nonspace boundary
				 * ^u: delete, and output BSBs, to return to BOL
				 * ^c: Do a ^u then send a telnet IP
				 * ^z: Do a ^u then send a telnet SUSP
				 * ^\: Do a ^u then send a telnet ABORT
				 * ^r: echo "^R\n" and redraw line
				 * ^v: quote next char
				 * ^d: if at BOL, end of file and close connection,
				 * else send line and reset to BOL
				 * ^m: send line-plus-\r\n and reset to BOL
				 */
			case KCTRL('H'):
			case KCTRL('?'):         /* backspace/delete */
				if (ldisc->buflen > 0) {
					do {
						if (ECHOING)
							bsb(ldisc, plen(ldisc, ldisc->buf[ldisc->buflen - 1]));
						ldisc->buflen--;
					} while (!char_start(ldisc, ldisc->buf[ldisc->buflen]));
				}
				break;
			case CTRL('W'):          /* delete word */
				while (ldisc->buflen > 0) {
					if (ECHOING)
						bsb(ldisc, plen(ldisc, ldisc->buf[ldisc->buflen - 1]));
					ldisc->buflen--;
					if (ldisc->buflen > 0 &&
						isspace((unsigned char)ldisc->buf[ldisc->buflen - 1]) &&
						!isspace((unsigned char)ldisc->buf[ldisc->buflen]))
						break;
				}
				break;
			case CTRL('U'):          /* delete line */
			case CTRL('C'):          /* Send IP */
			case CTRL('\\'):         /* Quit */
			case CTRL('Z'):          /* Suspend */
				while (ldisc->buflen > 0) {
					if (ECHOING)
						bsb(ldisc, plen(ldisc, ldisc->buf[ldisc->buflen - 1]));
					ldisc->buflen--;
				}
				backend_special(ldisc->backend, SS_EL, 0);
				/*
				 * We don't send IP, SUSP or ABORT if the user has
				 * configured telnet specials off! This breaks
				 * talkers otherwise.
				 */
				if (!ldisc->telnet_keyboard)
					goto default_case;
				if (c == CTRL('C'))
					backend_special(ldisc->backend, SS_IP, 0);
				if (c == CTRL('Z'))
					backend_special(ldisc->backend, SS_SUSP, 0);
				if (c == CTRL('\\'))
					backend_special(ldisc->backend, SS_ABORT, 0);
				break;
			case CTRL('R'):          /* redraw line */
				if (ECHOING) {
					int i;
					c_write(ldisc, "^R\r\n", 4);
					for (i = 0; i < ldisc->buflen; i++)
						pwrite(ldisc, ldisc->buf[i]);
				}
				break;
			case CTRL('V'):          /* quote next char */
				ldisc->quotenext = true;
				break;
			case CTRL('D'):          /* logout or send */
				if (ldisc->buflen == 0) {
					backend_special(ldisc->backend, SS_EOF, 0);
				}
				else {
					backend_send(ldisc->backend, ldisc->buf, ldisc->buflen);
					ldisc->buflen = 0;
				}
				break;
				/*
				 * This particularly hideous bit of code from RDB
				 * allows ordinary ^M^J to do the same thing as
				 * magic-^M when in Raw protocol. The line `case
				 * KCTRL('M'):' is _inside_ the if block. Thus:
				 *
				 *  - receiving regular ^M goes straight to the
				 *    default clause and inserts as a literal ^M.
				 *  - receiving regular ^J _not_ directly after a
				 *    literal ^M (or not in Raw protocol) fails the
				 *    if condition, leaps to the bottom of the if,
				 *    and falls through into the default clause
				 *    again.
				 *  - receiving regular ^J just after a literal ^M
				 *    in Raw protocol passes the if condition,
				 *    deletes the literal ^M, and falls through
				 *    into the magic-^M code
				 *  - receiving a magic-^M empties the line buffer,
				 *    signals end-of-line in one of the various
				 *    entertaining ways, and _doesn't_ fall out of
				 *    the bottom of the if and through to the
				 *    default clause because of the break.
				 */
			case CTRL('J'):
				if (ldisc->protocol == PROT_RAW &&
					ldisc->buflen > 0 && ldisc->buf[ldisc->buflen - 1] == '\r') {
					if (ECHOING)
						bsb(ldisc, plen(ldisc, ldisc->buf[ldisc->buflen - 1]));
					ldisc->buflen--;
					/* FALLTHROUGH */
			case KCTRL('M'):         /* send with newline */
				if (ldisc->buflen > 0)
					backend_send(ldisc->backend,
						ldisc->buf, ldisc->buflen);
				if (ldisc->protocol == PROT_RAW)
					backend_send(ldisc->backend, "\r\n", 2);
				else if (ldisc->protocol == PROT_TELNET && ldisc->telnet_newline)
					backend_special(ldisc->backend, SS_EOL, 0);
				else
					backend_send(ldisc->backend, "\r", 1);
				if (ECHOING)
					c_write(ldisc, "\r\n", 2);
				ldisc->buflen = 0;
				break;
				}
				/* FALLTHROUGH */
			default:                 /* get to this label from ^V handler */
			default_case:
				sgrowarray(ldisc->buf, ldisc->bufsiz, ldisc->buflen);
				ldisc->buf[ldisc->buflen++] = c;
				if (ECHOING)
					pwrite(ldisc, (unsigned char)c);
				ldisc->quotenext = false;
				break;
			}
		}
	}
	else {
		if (ldisc->buflen != 0) {
			backend_send(ldisc->backend, ldisc->buf, ldisc->buflen);
			while (ldisc->buflen > 0) {
				bsb(ldisc, plen(ldisc, ldisc->buf[ldisc->buflen - 1]));
				ldisc->buflen--;
			}
		}
		if (len > 0) {
			if (ECHOING)
				c_write(ldisc, buf, len);
			if (keyflag && ldisc->protocol == PROT_TELNET && len == 1) {
				switch (buf[0]) {
				case CTRL('M'):
					if (ldisc->protocol == PROT_TELNET && ldisc->telnet_newline)
						backend_special(ldisc->backend, SS_EOL, 0);
					else
						backend_send(ldisc->backend, "\r", 1);
					break;
				case CTRL('?'):
				case CTRL('H'):
					if (ldisc->telnet_keyboard) {
						backend_special(ldisc->backend, SS_EC, 0);
						break;
					}
				case CTRL('C'):
					if (ldisc->telnet_keyboard) {
						backend_special(ldisc->backend, SS_IP, 0);
						break;
					}
				case CTRL('Z'):
					if (ldisc->telnet_keyboard) {
						backend_special(ldisc->backend, SS_SUSP, 0);
						break;
					}

				default:
					backend_send(ldisc->backend, buf, len);
					break;
				}
			}
			else
				backend_send(ldisc->backend, buf, len);
		}
	}
}

void VtLdisc::Send(const std::string& str, bool interactive)
{

}

void VtLdisc::UpdateEchoEdit()
{

}

bool VtLdisc::IsEdit()
{
	if (FORCE_ON == m_tag.localedit)
	{
		return true;
	}

	bool aa = backend_ldisc_option_state(ldisc->backend, LD_EDIT);
	
	return m_tag.localedit == AUTO && aa;
}

bool VtLdisc::IsEcho()
{
	if (FORCE_ON == m_tag.localecho)
	{
		return true;
	}

	bool aa = backend_ldisc_option_state(ldisc->backend, LD_ECHO);

	return m_tag.localecho == AUTO && aa;
}
