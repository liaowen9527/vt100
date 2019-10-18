#pragma once
#include "../VtCtxNode.h"

class VtCtrlClear : public VtCtxNode
{
public:
	VtCtrlClear();
	~VtCtrlClear();

	void Init();

public:
	/* <Esc>[K	Clear line from cursor right	EL0 */
	/* <Esc>[0K	Clear line from cursor right	EL0 */
	void Right();

	/* <Esc>[1K	Clear line from cursor left	EL1 */
	void Left();

	/* <Esc>[2K	Clear entire line	EL2 */
	void Line();

	/* <Esc>[J	Clear screen from cursor down	ED0 */
	/* <Esc>[0J	Clear screen from cursor down	ED0 */
	void Down();

	/* <Esc>[1J	Clear screen from cursor up	ED1 */
	void Up();

	/* <Esc>[2J	Clear entire screen	ED2 */
	void Screen();

	/* <Esc>K	Erase to end of current line	cleareol */
	void Eol();

	/* <Esc>J	Erase to end of screen	cleareos */
	void Eos();
};