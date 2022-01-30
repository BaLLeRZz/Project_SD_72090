#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

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
	const double calculate_value(const string& expr, const string& current_node);
	void SET(const long int row, const long int column, const string& expr, const string& current_node);
	void PRINT_VAL(const long int row, const long int column);
	void PRINT_EXPR(const long int row, const long int column);
	void PRINT_VAL_ALL();
	void PRINT_EXPR_ALL();
	void SAVE(const string& str);
	//load
	void increase_by_one(const long int row, const long int column); // ++
	void decrease_by_one(const long int row, const long int column); // --
	const string fix_expr(string expr);
	const string get_string1(const string& str) const;
	const string get_string2(const string& str) const;
	const string get_string3(const string& str) const;
	const string get_file_name(const string& str) const;
	const long int get_row(const string& str) const;
	const long int get_column(const string& str) const;
	const long int get_row_relative(const string& str) const;
	const long int get_column_relative(const string& str) const;
	const bool exists(const long int row, const long int column) const;
	const bool check_expression(const string& str) const;
	const bool is_absolute(const string& str) const;
	const bool is_relative(const string& str) const;
	const bool if_function(const string& condition, const int value_true, const int value_false) const;
	void execute_proccess();
};