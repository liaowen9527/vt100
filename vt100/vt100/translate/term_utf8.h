#pragma once


#define UCS_INCOMPLETE 0x8000003FU    /* '?' */
#define UCS_TRUNCATED  0x80000021U    /* '!' */
#define UCS_INVALID    0x8000002AU    /* '*' */

class term_utf8
{
public:
	term_utf8();
	~term_utf8();

public:
	int append(unsigned char c);

protected:
	int append_first_char(unsigned char c);
	int append_other_char(unsigned char c);

protected:
	bool is_finish_by_state();
	bool is_other_char(unsigned char c);

	int get_len_by_first_char(unsigned char c);
	int trans_first_char(int size, unsigned char c);
	int trans_other_char(unsigned char c);

protected:
	int check();
	bool is_bad_chr(int chr, int size);
	bool is_crlf(int chr);
	bool is_high_control(int chr);
	bool is_utf16(int chr);
	bool is_iso_10646(int chr);
	bool is_tag_phobic_app(int chr);
	bool is_0xfeff(int chr);
	bool is_0xfffe(int chr);

protected:
	int m_state;                         /* Is there a pending UTF-8 character */
	int m_chr;                           /* and what is it so far? */
	int m_size;                          /* The size of the UTF character. */
};