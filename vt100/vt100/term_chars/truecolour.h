#pragma once

struct optionalrgb
{
	bool enabled;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};
extern const optionalrgb g_optionalrgb_none;

struct truecolour
{
	optionalrgb fg;
	optionalrgb bg;
};