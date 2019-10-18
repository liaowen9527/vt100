#include "VtFmtAction.h"
#include "../VtContext.h"
#include <assert.h>


VtFormat::VtFormat()
{

}

VtFormat::~VtFormat()
{

}

VtFormatPtr VtFormat::Create(EmVtFormat type)
{
	if (emString == type)
	{
		return std::make_shared<VtStringFmt>();
	}
	else if (emAttr == type)
	{
		return std::make_shared<VtAttrFmt>();
	}
	else if (emRegex == type)
	{
		return std::make_shared<VtAttrFmt>();
	}
	else
	{
		assert(false);
		abort();
	}

	return nullptr;
}

void VtFormat::SetFormat(const std::vector<std::string>& arr)
{
	std::string strFmt;
	for (int i = 0; i < arr.size(); ++i)
	{
		strFmt.append(arr[i]);
	}

	SetFormat(strFmt);
}

void VtFormat::SetFormat(const std::string& fmt)
{
	m_strFmt = fmt;
}

void VtFormat::GetAttrs(VtContext* context)
{

}

bool VtStringFmt::Match(VtContext* context)
{
	const std::string& fmt = context->buffer.m_buffer;

	int len = fmt.size();
	return m_strFmt.compare(0, len, fmt, 0, len) == 0;
}

bool VtAttrFmt::Match(VtContext* context)
{
	const std::string& fmt = context->buffer.m_fmt;

	int len = fmt.size();
	return m_strFmt.compare(0, len, fmt, 0, len) == 0;
}

void VtAttrFmt::GetAttrs(VtContext* context)
{
	std::string& buffer = context->buffer.m_buffer;
	std::vector<std::string>& attrs = context->buffer.m_attrs;
	attrs.clear();
	
	std::string temp;
	for (int i = 0; i < buffer.length(); ++i)
	{
		char ch = buffer.at(i);
		if (isdigit(ch))
		{
			temp.append(1, ch);
		}
		else
		{
			if (!temp.empty())
			{
				attrs.push_back(temp);
				temp.clear();
			}
		}
	}

	if (!temp.empty())
	{
		attrs.push_back(temp);
	}
}

bool VtLoopFmt::Match(VtContext* context)
{
	const std::string& fmt = context->buffer.m_buffer;
	int len = 0;

	//1, compare before loop
	if (fmt.length() <= m_strFmt.length())
	{
		len = fmt.length();
		return m_strFmt.compare(0, len, fmt, 0, len) == 0;
	}

	len = m_strFmt.length();
	if (m_strFmt.compare(0, len, fmt, 0, len) != 0)
	{
		return false;
	}
	
	//compare loop
	std::string strLoop = fmt.substr(len);
	std::vector<std::string> attrs = SplitAttrs(strLoop, ";");
	
	for (int i = 0; i < attrs.size(); ++i)
	{
		bool bLast = (i == attrs.size() - 1);
		if (!m_func(attrs[i], bLast))
		{
			return false;
		}
	}

	return true;
}

void VtLoopFmt::GetAttrs(VtContext* context)
{

}

void VtLoopFmt::SetFunc(std::function<bool(const std::string&, bool)> func)
{
	m_func = func;
}

std::vector<std::string> VtLoopFmt::SplitAttrs(const std::string& str, const std::string& sep)
{
	std::vector<std::string> ret;

	size_t wBegin = str.find_first_not_of(sep, 0);
	size_t wEnd = wBegin;
	while (true)
	{
		if (wBegin >= str.length())
			break;
		wEnd = str.find_first_of(sep, wBegin);
		ret.push_back(str.substr(wBegin, wEnd - wBegin));
		wBegin = str.find_first_not_of(sep, wEnd);
	}

	return std::move(ret);
}
