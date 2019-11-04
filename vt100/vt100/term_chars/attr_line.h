#pragma once

#define LATTR_NORM   0x00000000UL
#define LATTR_WIDE   0x00000001UL
#define LATTR_TOP    0x00000002UL
#define LATTR_BOT    0x00000003UL
#define LATTR_MODE   0x00000003UL		//top and wide
#define LATTR_WRAPPED 0x00000010UL     /* this line wraps to next */
#define LATTR_WRAPPED2 0x00000020UL    /* with WRAPPED: CJK wide character
										  wrapped to next line, so last
										  single-width cell is empty */

class attr_line
{
public:
	attr_line();
	~attr_line();

public:
	void enable(int attr, bool _enable);
	bool is_enable(int attr);

public:
	bool is_normal();
	bool is_wide();
	bool is_top();
	bool is_bottom();
	bool is_mode();
	bool is_wrapped();
	bool is_wrapped2();

protected:
	unsigned long m_attr;
};