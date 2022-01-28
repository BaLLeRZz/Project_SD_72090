#pragma once
#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

struct Node
{
	int value{};
	string expression{};
	long int row{};
	long int column{};
	Node() = default;
	bool is_number() const;
};

class Table
{
private:
	vector<Node> table{};
	long int max_rows{};
	long int max_columns{};
public:
	Table() = default;
	bool exists(const int row, const int column) const; // raboti
	Node get_node(const int row, const int column); // raboti
	int find_value(const int row, const int column) const; // raboti
	string fix_expr(string expr); // raboti
	int calculate_value(const string& expr); // raboti
	void SET(const int row, const int column, const string& expr); // raboti
	void PRINT_VAL(const int row, const int column); // raboti
	void PRINT_EXPR(const int row, const int column); // raboti
	void PRINT_VAL_ALL(); // raboti
	void PRINT_EXPR_ALL(); // raboti
	//save
	//load
	void increase_by_one(const int row, const int column); // ++
	void decrease_by_one(const int row, const int column); // --
	string get_string1(const string str); // raboti
	string get_string2(const string str); // raboti
	string get_string3(const string str); // raboti
	long int get_row(const string str); // raboti
	long int get_column(const string str); // raboti
	void execute_proccess();
};