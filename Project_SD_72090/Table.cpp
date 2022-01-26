#include "Table.h"
#include <stack>

using std::stack;

const bool isDigit(const char& symbol)
{
	return symbol >= '0' && symbol <= '9';
}

const bool isLeftAssociative(const char& operation)
{
	return operation != '^';
}

const string polishNotation(const string& expr)
{
	size_t size = expr.size();
	stack<char> operators;
	string output;
	string number{};
	size_t count{};
	size_t power{};
	for (size_t i = 0; i < size; i++)
	{
		if (isDigit(expr[i]))
		{
			number += expr[i];
			count++;
			if (count <= 1)
			{
				output.push_back(expr[i]);
				continue;
			}

			if (i + 1 < size)
				continue;
		}

		if (count > 1)
		{
			output.pop_back();
			output.push_back(number[count - 1]);
			for (int j = count - 2; j >= 0; j--) // 123 -> 351^2*2*+52^2*2*1*+
			{
				output.push_back('5');
				output.push_back(++power + '0');
				output.push_back('^');
				for (int k = 1; k < count - j; k++)
				{
					output.push_back('2');
					output.push_back('*');
				}
				output.push_back(number[j]);
				output.push_back('*');
				output.push_back('+');
			}

			if (i + 1 >= size)
				break;
		}
		count = 0;
		number = "";
		power = 0;

		if (expr[i] == ')')
		{
			while (!operators.empty() && operators.top() != '(')
			{
				output.push_back(operators.top());
				operators.pop();
			}
			operators.pop();
			continue;
		}

		if (isLeftAssociative(expr[i]))
		{
			if (expr[i] == '+' || expr[i] == '-')
			{
				while (!operators.empty() && operators.top() != '+' && operators.top() != '-' && operators.top() != '(')
				{
					output.push_back(operators.top());
					operators.pop();
				}
			}
		}

		operators.push(expr[i]);
	}
	while (!operators.empty())
	{
		output.push_back(operators.top());
		operators.pop();
	}
	return output;
}

const int calculate(const int& lvalue, const int& rvalue, char& operation)
{
	switch (operation)
	{
	case '+':
		return lvalue + rvalue;
	case '-':
		return lvalue - rvalue;
	case '*':
		return lvalue * rvalue;
	case '/':
		if (rvalue != 0)
			return lvalue / rvalue;
		return lvalue;
	case '^':
		return pow(lvalue, rvalue);
	default:
		std::cout << "Invalid input!" << std::endl;
	}
}

const int evaluateRpn(const string input)
{
	stack<int> values;
	int lvalue{}, rvalue{}, result{};
	for (char symbol : input)
	{
		if (isDigit(symbol))
		{
			values.push(symbol - '0');
			continue;
		}

		rvalue = values.top();
		values.pop();
		lvalue = values.top();
		values.pop();
		result = calculate(lvalue, rvalue, symbol);
		values.push(result);
	}
	return values.top();
}


bool Node::is_number() const
{
	size_t size = this->expression.size();
	for (size_t i = 0; i < size; i++)
	{
		if (this->expression[i] == '-')
			continue;

		if (this->expression[i] >= '0' && this->expression[i] <= '9')
			continue;

		return false;
	}

	return true;
}

int Node::get_number() const
{
	return stoi(this->expression);
}

int Table::find_value(const int row, const int column) const
{
	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
		if (this->table[i].row == row && this->table[i].column == column)
			return this->table[i].value;

	return 0;
}

int Table::calculate_value(const string& expr) const
{
	int value = 0;
	size_t size = expr.size(); // (R2C3 - 5) * 12 + R1C0
	vector<int> nodes_values;
	bool flagR = false, flagC = false;
	long int row, column;
	string number{};
	for (size_t i = 0; i < size; i++)
	{
		if (expr[i] == 'R' || flagR)
		{
			flagR = true;
			if (expr[i] >= '0' && expr[i] <= '9' && !flagC)
				number += expr[i];

			if (expr[i] == 'C' || flagC)
			{
				flagC = true;
				row = stoi(number);
				number = "";
				if (expr[i] >= '0' && expr[i] <= '9')
					number += expr[i];
			}

			if (!(expr[i] >= '0' && expr[i] <= '9') && flagR && flagC)
			{
				column = stoi(number);
				number.clear();
				flagR = false;
				flagC = false;
				nodes_values.push_back(this->find_value(row, column));
			}
		}
	}

	return 0;
}

void Table::SET(const int row, const int column, const string& expr)
{
	if (row < 0 || column < 0)
	{
		std::cout << "Invalid row or column input!" << std::endl;
		return;
	}

	if (row > this->max_rows)
		this->max_rows = row;

	if (column > this->max_columns)
		this->max_columns = column;

	/*size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
	{
		if (this->table[i].row == row && this->table[i].column == column)
		{

		}
	}*/
}

void Table::PRINT_VAL(const int row, const int column) const
{
	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
		if (this->table[i].row == row && this->table[i].column == column)
			std::cout << this->table[i].value << std::endl;
}

void Table::PRINT_EXPR(const int row, const int column) const
{
	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
		if (this->table[i].row == row && this->table[i].column == column)
			std::cout << this->table[i].expression << std::endl;
}

void Table::PRINT_VAL_ALL() const
{
	for (size_t i = 0; i < this->max_rows; i++)
	{
		for (size_t j = 0; j < this->max_columns; j++)
		{
			if (this->table[j].row == i && this->table[j].column == j)
			{
				if (this->table[j].expression[0] == '"')
					std::cout << this->table[j].expression << " ; ";
				else
					std::cout << this->table[j].value << " ; ";
			}
			else
				std::cout << "0 ; ";
		}
		std::cout << std::endl;
	}
}

void Table::PRINT_EXPR_ALL() const
{
	for (size_t i = 0; i < this->max_rows; i++)
	{
		for (size_t j = 0; j < this->max_columns; j++)
		{
			if (this->table[j].row == i && this->table[j].column == j)
				std::cout << this->table[j].expression << " ; ";

			else
				std::cout << "0 ; ";
		}
		std::cout << std::endl;
	}
}

void Table::increase_by_one(const int row, const int column)
{
	if (row > this->max_rows)
		this->max_rows = row;

	if (column > this->max_columns)
		this->max_columns = column;

	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
	{
		if (this->table[i].row == row && this->table[i].column == column)
		{
			this->table[i].value++;
			return;
		}
	}

	Node node;
	node.row = row;
	node.column = column;
	node.value = 1;
	node.expression = std::to_string(node.value);
	this->table.push_back(node);
}

void Table::decrease_by_one(const int row, const int column)
{
	if (row > this->max_rows)
		this->max_rows = row;

	if (column > this->max_columns)
		this->max_columns = column;

	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
	{
		if (this->table[i].row == row && this->table[i].column == column)
		{
			this->table[i].value--;
			return;
		}
	}

	Node node;
	node.row = row;
	node.column = column;
	node.value = -1;
	node.expression = std::to_string(node.value);
	this->table.push_back(node);
}

int main()
{
	//string input = "(2^3+4*(7-2))*2^2^3";
	string input = "(8*12-101*15)-21";
	string output = polishNotation(input);
	std::cout << "Input: " << input << std::endl;
	std::cout << "Polish Notation: " << output << std::endl;
	std::cout << "Evaluation of PN: " << evaluateRpn(output) << std::endl;
	return 0;
}