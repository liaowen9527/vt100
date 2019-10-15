#pragma once
#include "VtControl.h"

class VtCtrlColor : public VtControl
{
public:
	VtCtrlColor();
	~VtCtrlColor();

	void Init();

public:
	void Reset();

	/*
	Set Attribute Mode	<ESC>[{attr1};...;{attrn}m
	Sets multiple display attribute settings. The following lists standard attributes:
	0	Reset all attributes
	1	Bright
	2	Dim
	4	Underscore
	5	Blink
	7	Reverse
	8	Hidden

		Foreground Colors
	30	Black
	31	Red
	32	Green
	33	Yellow
	34	Blue
	35	Magenta
	36	Cyan
	37	White

		Background Colors
	40	Black
	41	Red
	42	Green
	43	Yellow
	44	Blue
	45	Magenta
	46	Cyan
	47	White
	*/
	void SetAttribute();
	void SetAttribute(char attr);
};