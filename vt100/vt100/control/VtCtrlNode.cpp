#include "VtCtxNode.h"

VtCtrlNode::VtCtrlNode()
{

}

VtCtrlNode::~VtCtrlNode()
{

}

void VtCtrlNode::SetContext(VtContext* context)
{
	m_context = context;

	m_term = &(m_context->term);
	m_args = &(m_term->m_args);
	m_cursor = &(m_term->m_cursor);
}

bool VtCtrlNode::HasCompat(int vttype)
{
	return vt_type::IsCompat(m_term->termstate, vttype);
}

bool VtCtrlNode::CheckCompat(int vttype)
{
	if (!HasCompat(vttype))
	{
		m_term->termstate = TOPLEVEL;
		return false;
	}

	return true;
}

bool VtCtrlNode::CheckCompat_2(int vttype, int vttype2)
{
	if (HasCompat(vttype) && HasCompat(vttype2))
	{
		return true;
	}

	m_term->termstate = TOPLEVEL;
	return false;
}
