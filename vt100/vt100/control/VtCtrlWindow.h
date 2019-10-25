#pragma once
#include "VtCtrlNode.h"

//Some terminals support local printing :
class VtCtrlWindow : public VtCtrlNode
{
public:
	VtCtrlWindow();
	~VtCtrlWindow();

public:
	void SetLines();
	void SetColumns();

public:
	bool win_setup_draw_ctx();
	void win_draw_text(int x, int y, wchar_t *text, int len, unsigned long attrs, int line_attrs, truecolour tc);
	void win_draw_cursor(int x, int y, wchar_t *text, int len, unsigned long attrs, int line_attrs, truecolour tc);
	void win_draw_trust_sigil(int x, int y);
	int win_char_width(int uc);
	void win_free_draw_ctx();
	void win_set_cursor_pos(int x, int y);
	void win_set_raw_mouse_mode(bool enable);
	void win_set_scrollbar(int t, int s, int p);
	void win_bell(int mode);
	void win_clip_write(int clipboard, wchar_t *text, int *attrs, truecolour *colours, int len, bool deselect);
	void win_clip_request_paste(int clipboard);
	void win_refresh();
	void win_request_resize(int w, int h);
	void win_set_title(const char *title);
	void win_set_icon_title(const char *icontitle);
	void win_set_minimised(bool minimised);
	bool win_is_minimised();
	void win_set_maximised(bool maximised);
	void win_move(int x, int y);
	void win_set_zorder(bool top);
	bool win_palette_get(int n, int *r, int *g, int *b);
	void win_palette_set(int n, int r, int g, int b);
	void win_palette_reset();
	void win_get_pos(int *x, int *y);
	void win_get_pixels(int *x, int *y);
	const char *win_get_title(bool icon);
	bool win_is_utf8();
};