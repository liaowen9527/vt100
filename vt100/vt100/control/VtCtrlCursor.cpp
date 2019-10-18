#include "VtCtrlCursor.h"
#include "../VtContext.h"

VtCtrlCursor::VtCtrlCursor()
{
 	
}

VtCtrlCursor::~VtCtrlCursor()
{

}

void VtCtrlCursor::Init()
{
	VtFmtManager& fmtManager = m_context->fmtManager;

	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, "A" }, std::bind(&VtCtrlCursor::Up, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, "B" }, std::bind(&VtCtrlCursor::Down, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, "C" }, std::bind(&VtCtrlCursor::Forward, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, "D" }, std::bind(&VtCtrlCursor::Backward, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[H" }, std::bind(&VtCtrlCursor::Home, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[;H" }, std::bind(&VtCtrlCursor::Home, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[f" }, std::bind(&VtCtrlCursor::Home, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[;f" }, std::bind(&VtCtrlCursor::Home, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, ";", VT_STR_NUM, "H" }, std::bind(&VtCtrlCursor::SetPosition, this));
	fmtManager.AddAttrFmt({ VT_STR_ESC, "[", VT_STR_NUM, ";", VT_STR_NUM, "f" }, std::bind(&VtCtrlCursor::SetPosition, this));

	fmtManager.AddFmt({ VT_STR_ESC, "[E" }, std::bind(&VtCtrlCursor::NextLine, this));

	fmtManager.AddFmt({ VT_STR_ESC, "[s" }, std::bind(&VtCtrlCursor::Save, this));
	fmtManager.AddFmt({ VT_STR_ESC, "[u" }, std::bind(&VtCtrlCursor::Restore, this));
	fmtManager.AddFmt({ VT_STR_ESC, "7" }, std::bind(&VtCtrlCursor::Save, this));
	fmtManager.AddFmt({ VT_STR_ESC, "8" }, std::bind(&VtCtrlCursor::Restore, this));

}

void VtCtrlCursor::Up()
{
	int count = m_context->buffer.GetIntAttr(0);
	count = (count == 0 ? 1 : count);

	VtCursor& cursor = m_context->terminal.Cursor();
	cursor.Move(-count, 0);
}

void VtCtrlCursor::Down()
{
	int count = m_context->buffer.GetIntAttr(0);
	count = (count == 0 ? 1 : count);

	VtCursor& cursor = m_context->terminal.Cursor();

	cursor.Move(count, 0);
}

void VtCtrlCursor::Forward()
{
	int count = m_context->buffer.GetIntAttr(0);
	count = (count == 0 ? 1 : count);

	VtCursor& cursor = m_context->terminal.Cursor();

	cursor.Move(0, count);
}

void VtCtrlCursor::Backward()
{
	int count = m_context->buffer.GetIntAttr(0);
	count = (count == 0 ? 1 : count);

	VtCursor& cursor = m_context->terminal.Cursor();

	cursor.Move(0, -count);
}

void VtCtrlCursor::Home()
{
	VtCursor& cursor = m_context->terminal.Cursor();
	terminal_tag& tag = m_context->terminal.tag();
	if (tag.dec_om)
	{
		cursor.MoveTo(0, 0, true);
	}
	else
	{
		cursor.Set(0, 0);
	}
}

void VtCtrlCursor::SetPosition()
{
	int row = m_context->buffer.GetIntAttr(0) - 1;
	int col = m_context->buffer.GetIntAttr(1) - 1;
	VtCursor& cursor = m_context->terminal.Cursor();
	terminal_tag& tag = m_context->terminal.tag();

	row = cursor.ToInsideRow(row);
	col = cursor.ToInsideRow(col);

	if (tag.dec_om)
	{
		cursor.MoveTo(row, col, true);
	}
	else
	{
		cursor.Set(row, col);
	}
}

void VtCtrlCursor::NextLine()
{
	VtCursor& cursor = m_context->terminal.Cursor();
	cursor.MoveTo(cursor.Row() + 1, 0, false);
}

void VtCtrlCursor::Save()
{
	VtCursor& cursor = m_context->terminal.Cursor();
	cursor.Save();
}

void VtCtrlCursor::Restore()
{
	VtCursor& cursor = m_context->terminal.Cursor();
	cursor.Restore();
}
