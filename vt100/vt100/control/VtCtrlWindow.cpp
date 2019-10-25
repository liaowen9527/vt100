#include "VtCtrlWindow.h"
#include "../VtContext.h"

VtCtrlWindow::VtCtrlWindow()
{
	
}

VtCtrlWindow::~VtCtrlWindow()
{

}

void VtCtrlWindow::SetLines()
{
	/*
	* Set number of lines on screen
	* VT420 uses VGA like hardware and can
	* support any size in reasonable range
	* (24..49 AIUI) with no default specified.
	*/
	if (!CheckCompat(VT420))
	{
		return;
	}
	
	if (term->esc_nargs == 1 && term->esc_args[0] > 0) 
	{
		if (!term->no_remote_resize)
		{
			win_request_resize(term->win, term->cols, def(term->esc_args[0], term->conf_height));
		}
		
		deselect(term);
	}
}

void VtCtrlWindow::SetColumns()
{
	/*
	* Set number of columns per page
	* Docs imply range is only 80 or 132, but
	* I'll allow any.
	*/
	if (!CheckCompat(VT340TEXT))
	{
		return;
	}

	if (term->esc_nargs <= 1) 
	{
		if (!term->no_remote_resize)
		{
			win_request_resize(term->win, def(term->esc_args[0], term->conf_width), term->rows);
		}
			
		deselect(term);
	}
}

bool VtCtrlWindow::win_setup_draw_ctx()
{
	return win->vt->setup_draw_ctx(win);
}

void VtCtrlWindow::win_draw_text(int x, int y, wchar_t *text, int len, unsigned long attrs, int line_attrs, truecolour tc)
{
	win->vt->draw_text(win, x, y, text, len, attrs, line_attrs, tc);
}

void VtCtrlWindow::win_draw_cursor(int x, int y, wchar_t *text, int len, unsigned long attrs, int line_attrs, truecolour tc)
{
	win->vt->draw_cursor(win, x, y, text, len, attrs, line_attrs, tc);
}

void VtCtrlWindow::win_draw_trust_sigil(int x, int y)
{
	win->vt->draw_trust_sigil(win, x, y);
}

int VtCtrlWindow::win_char_width(int uc)
{
	return win->vt->char_width(win, uc);
}

void VtCtrlWindow::win_free_draw_ctx()
{
	win->vt->free_draw_ctx(win);
}

void VtCtrlWindow::win_set_cursor_pos(int x, int y)
{
	win->vt->set_cursor_pos(win, x, y);
}

void VtCtrlWindow::win_set_raw_mouse_mode(bool enable)
{
	win->vt->set_raw_mouse_mode(win, enable);
}

void VtCtrlWindow::win_set_scrollbar(int t, int s, int p)
{
	win->vt->set_scrollbar(win, t, s, p);
}

void VtCtrlWindow::win_bell(int mode)
{
	win->vt->bell(win, mode);
}

void VtCtrlWindow::win_clip_write(int clipboard, wchar_t *text, int *attrs, truecolour *colours, int len, bool deselect)
{
	win->vt->clip_write(win, clipboard, text, attrs, colours, len, deselect);
}

void VtCtrlWindow::win_clip_request_paste(int clipboard)
{
	win->vt->clip_request_paste(win, clipboard);
}

void VtCtrlWindow::win_refresh()
{
	win->vt->refresh(win);
}

void VtCtrlWindow::win_request_resize(int w, int h)
{
	win->vt->request_resize(win, w, h);
}

void VtCtrlWindow::win_set_title(const char *title)
{
	win->vt->set_title(win, title);
}

void VtCtrlWindow::win_set_icon_title(const char *icontitle)
{
	win->vt->set_icon_title(win, icontitle);
}

void VtCtrlWindow::win_set_minimised(bool minimised)
{
	win->vt->set_minimised(win, minimised);
}

bool VtCtrlWindow::win_is_minimised()
{
	return win->vt->is_minimised(win);
}

void VtCtrlWindow::win_set_maximised(bool maximised)
{
	win->vt->set_maximised(win, maximised);
}

void VtCtrlWindow::win_move(int x, int y)
{
	win->vt->move(win, x, y);
}

void VtCtrlWindow::win_set_zorder(bool top)
{
	win->vt->set_zorder(win, top);
}

bool VtCtrlWindow::win_palette_get(int n, int *r, int *g, int *b)
{
	return win->vt->palette_get(win, n, r, g, b);
}

void VtCtrlWindow::win_palette_set(int n, int r, int g, int b)
{
	win->vt->palette_set(win, n, r, g, b);
}

void VtCtrlWindow::win_palette_reset()
{
	win->vt->palette_reset(win);
}

void VtCtrlWindow::win_get_pos(int *x, int *y)
{
	win->vt->get_pos(win, x, y);
}

void VtCtrlWindow::win_get_pixels(int *x, int *y)
{
	win->vt->get_pixels(win, x, y);
}

const char * VtCtrlWindow::win_get_title(bool icon)
{
	return win->vt->get_title(win, icon);
}

bool VtCtrlWindow::win_is_utf8()
{
	return win->vt->is_utf8(win);
}
