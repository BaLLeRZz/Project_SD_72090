#pragma once
#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

struct Node
{
	double value{};
	string expression{};
	long int row{};
	long int column{};
	Node() = default;
};

class Table
{
private:
	vector<Node> table{};
	long int max_rows{};
	long int max_columns{};
public:
	Table() = default;
	Node get_node(const long int row, const long int column);
	const double find_value(const long int row, const long int column) const;
	const double calculate_value(const string& expr);
	void SET(const long int row, const long int column, const string& expr);
	void PRINT_VAL(const long int row, const long int column);
	void PRINT_EXPR(const long int row, const long int column);
	void PRINT_VAL_ALL();
	void PRINT_EXPR_ALL();
	//save
	//load
	void increase_by_one(const long int row, const long int column); // ++
	void decrease_by_one(const long int row, const long int column); // --
	const string fix_expr(string expr);
	const string get_string1(const string& str) const;
	const string get_string2(const string& str) const;
	const string get_string3(const string& str) const;
	const long int get_row(const string& str) const;
	const long int get_column(const string& str) const;
	const bool exists(const long int row, const long int column) const;
	const bool check_adress(const string& str) const;
	const bool check_expression(const string& str) const;
	const bool is_absolute(const string& str) const;
	const bool is_relative(const string& str) const;
	void execute_proccess();
};