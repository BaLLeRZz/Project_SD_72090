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
	int get_number() const;
};

class Table
{
private:
	vector<Node> table{};
	long int max_rows{};
	long int max_columns{};
public:
	Table() = default;
	int find_value(const int row, const int column) const;
	int calculate_value(const string& expr) const;
	void SET(const int row, const int column, const string& expr);
	void PRINT_VAL(const int row, const int column) const;
	void PRINT_EXPR(const int row, const int column) const;
	void PRINT_VAL_ALL() const;
	void PRINT_EXPR_ALL() const;
	//save
	//load
	void increase_by_one(const int row, const int column); // ++
	void decrease_by_one(const int row, const int column); // --
};