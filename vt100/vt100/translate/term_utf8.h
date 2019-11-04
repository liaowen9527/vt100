#pragma once


#define UCS_INCOMPLETE 0x8000003FU    /* '?' */
#define UCS_TRUNCATED  0x80000021U    /* '!' */
#define UCS_INVALID    0x8000002AU    /* '*' */

/* These are internal use overlapping with the UTF-16 surrogates */
#define CSET_ASCII   0x0000D800UL      /* normal ASCII charset ESC ( B */
#define CSET_LINEDRW 0x0000D900UL      /* line drawing charset ESC ( 0 */
#define CSET_SCOACS  0x0000DA00UL      /* SCO Alternate charset */
#define CSET_GBCHR   0x0000DB00UL      /* UK variant   charset ESC ( A */
#define CSET_MASK    0xFFFFFF00UL      /* Character set mask */

class term_translate;

class term_utf8
{
public:
	term_utf8();
	~term_utf8();

public:
	void set_translate(term_translate* trans);
	unsigned long append(unsigned char c);

protected:
	unsigned long append_first_char(unsigned char c);
	unsigned long append_other_char(unsigned char c);

protected:
	bool is_finish_by_state();
	bool is_other_char(unsigned char c);

	int get_len_by_first_char(unsigned char c);
	unsigned long trans_first_char(unsigned char c);
	unsigned long trans_first_char_ascii(unsigned char c);
	int trans_other_char(unsigned char c);

protected:
	unsigned long check();
	bool is_bad_chr(int chr, int size);
	bool is_crlf(int chr);
	bool is_high_control(int chr);
	bool is_utf16(int chr);
	bool is_iso_10646(int chr);
	bool is_tag_phobic_app(int chr);
	bool is_0xfeff(int chr);
	bool is_0xfffe(int chr);

protected:
	unsigned char get_unitab_ctrl(unsigned char c);
	bool is_utf8linedraw();

protected:
	int m_state;                         /* Is there a pending UTF-8 character */
	int m_chr;                           /* and what is it so far? */
	int m_size;                          /* The size of the UTF character. */

	term_translate* m_trans;
};