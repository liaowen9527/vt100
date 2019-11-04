#pragma once
#include <string>
#include <vector>


class cp_table
{
public:
	cp_table();
	cp_table(std::initializer_list<wchar_t> _Ilist);
	~cp_table();

public:
	int size();
	wchar_t get_cp(int index);

protected:
	std::vector<wchar_t> m_cp_list;
};


class cp_list_item
{
public:
	cp_list_item();
	cp_list_item(const char* name, int cp, const cp_table* table = nullptr);
	~cp_list_item();

public:
	std::string get_name();
	void set_name(const std::string& name);

	int get_codepage();
	void set_codepage(int cp);

	int get_size();

	cp_table* get_cp_table();

protected:
	std::string m_name;
	int m_codepage;
	cp_table* m_table;
};

extern const std::vector<cp_list_item> g_cp_list;

