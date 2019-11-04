#pragma once

class VtScreen;

class VtCursor
{
public:
	VtCursor();
	~VtCursor();

public:
	void Get(int& row, int& col);
	void Set(int row, int col);
	
	int Row();
	void SetRow(int row);

	int Col();
	void SetCol(int col);

public:
	void Move(int row, int col);
	void MoveTo(int row, int col, bool visible);
	void NextLine();

	void ReverseIndex();

	void Save();
	void Restore();

public:
	bool IsVisible();
	bool AtBottom();

public:
	int ToInsideRow(int row);
	int ToInsideCol(int col);

	bool IsVisibleRow(int row);
	int ToVisibleRow(int row);
	
protected:
	VtScreen* m_screen;
};

