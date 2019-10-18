#pragma once
#include "../VtCtxNode.h"

class VtCtrlCursor : public VtCtxNode
{
public:
	VtCtrlCursor();
	~VtCtrlCursor();
	
	void Init();

public:
	/* <Esc>[{Count}A	Move cursor up n lines	CUU */
	void Up();

	/* <Esc>[{Count}B	Move cursor down n lines	CUD */
	void Down();

	/* <Esc>[{Count}C	Move cursor right n lines	CUF */
	void Forward();	

	/* <Esc>[{Count}D	Move cursor left n lines	CUB */
	void Backward();

	/* <Esc>H	Move cursor to upper left corner	cursorhome */
	/* <Esc>[H	Move cursor to upper left corner	cursorhome */
	/* <Esc>[;H	Move cursor to upper left corner	cursorhome */
	/* <Esc>[f	Move cursor to upper left corner	hvhome */
	/* <Esc>[;f	Move cursor to upper left corner	hvhome */
	void Home();
	
	/* <Esc>[{Line};{Column}H	Move cursor to screen location v, h	CUP */
	/* <Esc>[{Line};{Column}f	Move cursor to screen location v, h	CUP */
	void SetPosition();

	/* <Esc>E	Move to next line	NEL */
	void NextLine();

	/* <ESC>[s Save current cursor position. */
	/* <Esc>7	Save cursor position and attributes	DECSC */
	void Save();

	/* <ESC>[u Restores cursor position after a Save Cursor.*/
	/* <Esc>8	Restore cursor position and attributes	DECSC */
	void Restore();
};