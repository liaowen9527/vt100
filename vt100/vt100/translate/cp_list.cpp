#include "cp_list.h"

cp_table::cp_table()
{

}

cp_table::cp_table(std::initializer_list<wchar_t> _Ilist)
{
	m_cp_list = _Ilist;
}

cp_table::~cp_table()
{

}

int cp_table::size()
{
	return m_cp_list.size();
}

wchar_t cp_table::get_cp(int index)
{
	return m_cp_list[index];
}

cp_list_item::cp_list_item()
	: m_table(nullptr)
{

}

cp_list_item::cp_list_item(const char* name, int cp, const cp_table* table /*= nullptr*/)
	: m_name(name)
	, m_codepage(cp)
	, m_table((cp_table*)table)
{

}

cp_list_item::~cp_list_item()
{

}

std::string cp_list_item::get_name()
{
	return m_name;
}

void cp_list_item::set_name(const std::string& name)
{
	m_name = name;
}

int cp_list_item::get_codepage()
{
	return m_codepage;
}

void cp_list_item::set_codepage(int cp)
{
	m_codepage = cp;
}

int cp_list_item::get_size()
{
	return m_table == nullptr ? 0 : m_table->size();
}

cp_table* cp_list_item::get_cp_table()
{
	return m_table;
}

