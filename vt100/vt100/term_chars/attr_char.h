#pragma once

#define ATTR_NARROW  0x0800000U
#define ATTR_WIDE    0x0400000U
#define ATTR_BOLD    0x0040000U
#define ATTR_UNDER   0x0080000U
#define ATTR_REVERSE 0x0100000U
#define ATTR_BLINK   0x0200000U
#define ATTR_FGMASK  0x00001FFU
#define ATTR_BGMASK  0x003FE00U
#define ATTR_COLOURS 0x003FFFFU
#define ATTR_DIM     0x1000000U
#define ATTR_FGSHIFT 0
#define ATTR_BGSHIFT 9

class attr_char
{
public:
	attr_char();
	~attr_char();

public:
	void enable(int attr, bool _enable);
	bool is_enable(int attr);

public:
	bool is_narrow();
	bool is_wide();
	bool is_bold();
	bool is_under();
	bool is_reverse();
	bool is_blink();
	bool is_fgmask();
	bool is_bgmask();
	bool is_colours();
	bool is_dim();

protected:
	unsigned long m_attr;
};