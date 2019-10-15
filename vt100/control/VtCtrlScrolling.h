#pragma once
#include "VtControl.h"

class VtCtrlScrolling : public VtControl
{
public:
	VtCtrlScrolling();
	~VtCtrlScrolling();

	void Init();

public:
	/*
	Scroll Screen		<ESC>[r
	Enable scrolling for entire display.
	*/
	void Enable();

	/*
	Scroll Screen		<ESC>[{start};{end}r
	Enable scrolling from row {start} to row {end}.
	*/
	void VScroll();

	/* <Esc>D	Move / scroll window up one line	IND */
	void Down();

	/* <Esc>M	Move / scroll window down one line	RI */
	void Up();

	/* 
	<Esc>[{top};{bottom}r	Set top and bottom lines of a window	DECSTBM 
	Pt is the number of the top line of the scrolling region; Pb is the number of the bottom line of the scrolling region and must be greater than Pt.
	*/
	void Region();
};