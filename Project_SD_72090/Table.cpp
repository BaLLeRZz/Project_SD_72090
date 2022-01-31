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
	stack<char> operators;
	string output;
	string number{};
	size_t size = expr.size();
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

		// 123 -> 351^2*2*+52^2*2*1*+0+
		if (count > 1)
		{
			output.pop_back();
			output.push_back(number[count - 1]);
			for (int j = count - 2; j >= 0; j--)
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
				output.push_back('0');
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

const double calculate(const double& lvalue, const double& rvalue, const char& operation)
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
	/*case '>':
		return lvalue > rvalue;
	case '<':
		return lvalue < rvalue;
	case '=':
		return lvalue == rvalue;
	case '!':
		return lvalue != rvalue;*/
	default:
		std::cout << "Invalid input!" << std::endl;
	}
}

const double evaluateRpn(string input)
{
	size_t size = input.size();
	stack<double> values;
	double lvalue{}, rvalue{}, result{};
	for (size_t i = 0; i < size; i++)
	{
		if (isDigit(input[i]))
		{
			values.push(input[i] - '0');
			continue;
		}

		if (values.size() == 1 && input[i] == '-')
		{
			if (input[i + 1] == '-')
				return values.top();

			else
				return -values.top();
		}

		if (values.size() < 2)
			break;

		else
		{
			rvalue = values.top();
			values.pop();
			lvalue = values.top();
			values.pop();
		}

		if (input[i + 1] == '-')
		{
			if (input[i] == '-' || (input[i] == '+' && rvalue < 0))
			{
				input[i + 1] = '+';
				result = -calculate(lvalue, abs(rvalue), '+');
				values.push(result);
				continue;
			}

			if (input[i] == '+' && rvalue >= 0)
			{
				input[i + 1] = '+';
				result = calculate(-lvalue, rvalue, '+');
				values.push(result);
				continue;
			}
		}

		result = calculate(lvalue, rvalue, input[i]);
		values.push(result);
	}

	return values.top();
}

Node Table::get_node(const long int row, const long int column)
{
	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
		if (this->table[i].row == row && this->table[i].column == column)
			return this->table[i];

	return Node();
}

const double Table::find_value(const long int row, const long int column) const
{
	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
		if (this->table[i].row == row && this->table[i].column == column)
			return this->table[i].value;

	return 0;
}

const double Table::calculate_value(const string& expr, const string& current_node)
{
	// (R23C31 - 5) * 12 + R1C0 -> (3-5)*12+6
	if (expr[0] == '"')
		return 0;

	vector<string> nodes;
	string node_string{};
	size_t size = expr.size();
	bool flagR = false, flagC = false, flagHelp = true, flagRelative = false;
	for (size_t i = 0; i < size; i++)
	{
		if (expr[i] == 'R' || flagR)
		{
			flagR = true;
			if (expr[i] == '[')
				flagRelative = true;

			if (expr[i] == ']')
				flagRelative = false;

			if ((isDigit(expr[i]) || expr[i] == 'R' || expr[i] == '[' || expr[i] == ']' || expr[i] == '-') && !flagC)
				node_string += expr[i];

			if (expr[i] == 'C' || flagC)
			{
				flagC = true;
				if (flagHelp)
				{
					node_string += expr[i];
					flagHelp = false;
					continue;
				}

				if (isDigit(expr[i]) || expr[i] == '[' || expr[i] == ']' || (expr[i] == '-' && flagRelative))
				{
					node_string += expr[i];
					if (i + 1 < size)
						continue;
				}
			}

			if (!isDigit(expr[i]) && expr[i] != ']' && flagR && flagC || ((isDigit(expr[i]) || expr[i] == ']') && i + 1 == size))
			{
				flagR = false;
				flagC = false;
				flagHelp = true;
				flagRelative = false;
				nodes.push_back(node_string);
				node_string = "";
			}
		}
	}

	vector<int> nodes_values{};
	size_t nodes_size = nodes.size();
	long int row{}, column{};
	long int row_current{}, column_current{};
	for (size_t i = 0; i < nodes_size; i++)
	{
		if (this->is_absolute(nodes[i]))
		{
			row = this->get_row(nodes[i]);
			column = this->get_column(nodes[i]);
			if (row > this->max_rows)
				this->max_rows = row;

			if (column > this->max_columns)
				this->max_columns = column;

			if (this->exists(row, column))
				nodes_values.push_back(this->get_node(row, column).value);

			else
				nodes_values.push_back(0);
			
			continue;
		}
		
		if (this->is_relative(nodes[i]))
		{
			row = this->get_row_relative(nodes[i]);
			column = this->get_column_relative(nodes[i]);
			row_current = this->get_row(current_node);
			column_current = this->get_column(current_node);
			if (row_current + row <= 0 || column_current + column <= 0)
			{
				std::cout << "Incorrect row or column input in a relative adress!" << std::endl;
				std::cout << "The value is set to 0" << std::endl;
				return 0;
			}

			if (row_current + row > this->max_rows)
				this->max_rows = row_current + row;

			if (column_current + column > this->max_columns)
				this->max_columns = column_current + column;

			if (this->exists(row_current + row, column_current + column))
				nodes_values.push_back(this->get_node(row_current + row, column_current + column).value);

			else
				nodes_values.push_back(0);

			continue;
		}

		std::cout << "Incorrect adress input in the expression!" << std::endl;
		std::cout << "The value is set to 0" << std::endl;
		return 0;
	}

	string fixed_expr{};
	size_t count{};
	for (size_t i = 0; i < size; i++)
	{
		if (expr[i] == ' ')
			continue;

		if (expr[i] == 'R' || flagR)
		{
			flagR = true;
			if (flagHelp)
			{
				fixed_expr += std::to_string(nodes_values[count++]);
				flagHelp = false;
			}

			if (expr[i] == 'C' || flagC)
			{
				flagC = true;
				if (expr[i] == 'C' && expr[i + 1] == '[')
				{
					if (expr[i + 2] == '-')
					{
						i += 2;
						continue;
					}
					else
					{
						i += 1;
						continue;
					}
				}

				if (!isDigit(expr[i]) && expr[i] != 'C' && expr[i] != ']')
				{
					flagR = false;
					flagC = false;
					flagHelp = true;
				}
			}
		}

		if (!flagR && !flagC)
			fixed_expr += expr[i];
	}

	fixed_expr = fix_expr(fixed_expr);
	if (fixed_expr == "Incorrect expression input!")
	{
		std::cout << fixed_expr << std::endl;
		std::cout << "The value is set to 0" << std::endl;
		return 0;
	}

	nodes.clear();
	nodes_values.clear();
	node_string.clear();
	string polish_notation = polishNotation(fixed_expr);
	return evaluateRpn(polish_notation);
}

const double Table::SUM(const string& str1, const string& str2)
{
	if (!this->is_absolute(str1))
	{
		std::cout << "The first adress must be absolute!" << std::endl;
		return 0;
	}

	if (this->is_absolute(str2))
	{
		double result{};
		long int row1{}, row2{}, column1{}, column2{};
		row1 = this->get_row(str1);
		column1 = this->get_column(str1);
		row2 = this->get_row(str2);
		column2 = this->get_column(str2);

		if (row1 > this->max_rows)
			this->max_rows = row1;

		if (column1 > this->max_columns)
			this->max_columns = column1;

		if (row2 > this->max_rows)
			this->max_rows = row2;

		if (column2 > this->max_columns)
			this->max_columns = column2;

		if (row1 <= row2)
		{
			if (column1 <= column2)
			{
				for (size_t i = row1; i <= row2; i++)
					for (size_t j = column1; j <= column2; j++)
						if (this->exists(i, j))
							result += this->get_node(i, j).value;

				return result;
			}
			else
			{
				for (size_t i = row1; i <= row2; i++)
					for (size_t j = column2; j <= column1; j++)
						if (this->exists(i, j))
							result += this->get_node(i, j).value;

				return result;
			}
		}
		else
		{
			if (column1 <= column2)
			{
				for (size_t i = row2; i <= row1; i++)
					for (size_t j = column1; j <= column2; j++)
						if (this->exists(i, j))
							result += this->get_node(i, j).value;

				return result;
			}
			else
			{
				for (size_t i = row2; i <= row1; i++)
					for (size_t j = column2; j <= column1; j++)
						if (this->exists(i, j))
							result += this->get_node(i, j).value;

				return result;
			}
		}

		return result;
	}

	if (this->is_relative(str2))
	{
		double result{};
		long int row1{}, row2{}, column1{}, column2{};
		row1 = this->get_row(str1);
		column1 = this->get_column(str1);
		row2 = this->get_row_relative(str2);
		column2 = this->get_column_relative(str2);
		if (row1 + row2 <= 0 || column1 + column2 <= 0)
		{
			std::cout << "Invalid row or column input in a relative adress!" << std::endl;
			return 0;
		}

		if (row1 > this->max_rows)
			this->max_rows = row1;

		if (column1 > this->max_columns)
			this->max_columns = column1;

		if (row1 + row2 > this->max_rows)
			this->max_rows = row1 + row2;

		if (column1 + column2 > this->max_columns)
			this->max_columns = column1 + column2;

		if (row1 <= row2)
		{
			if (column1 <= column2)
			{
				for (size_t i = row1; i <= row2; i++)
					for(size_t j = column1; j <= column2; j++)
						if (this->exists(i, j))
							result += this->get_node(i, j).value;

				return result;
			}
			else
			{
				for (size_t i = row1; i <= row2; i++)
					for (size_t j = column2; j <= column1; j++)
						if (this->exists(i, j))
							result += this->get_node(i, j).value;

				return result;
			}
		}
		else
		{
			if (column1 <= column2)
			{
				for (size_t i = row2; i <= row1; i++)
					for (size_t j = column1; j <= column2; j++)
						if (this->exists(i, j))
							result += this->get_node(i, j).value;

				return result;
			}
			else
			{
				for (size_t i = row2; i <= row1; i++)
					for (size_t j = column2; j <= column1; j++)
						if (this->exists(i, j))
							result += this->get_node(i, j).value;

				return result;
			}
		}
	}

	std::cout << "Invalid adresses input!" << std::endl;
	return 0;
}

const size_t Table::COUNT(const string& str1, const string& str2)
{
	if (!this->is_absolute(str1))
	{
		std::cout << "The first adress must be absolute!" << std::endl;
		return 0;
	}

	if (this->is_absolute(str2))
	{
		size_t count{};
		long int row1{}, row2{}, column1{}, column2{};
		row1 = this->get_row(str1);
		column1 = this->get_column(str1);
		row2 = this->get_row(str2);
		column2 = this->get_column(str2);

		if (row1 > this->max_rows)
			this->max_rows = row1;

		if (column1 > this->max_columns)
			this->max_columns = column1;

		if (row2 > this->max_rows)
			this->max_rows = row2;

		if (column2 > this->max_columns)
			this->max_columns = column2;

		if (row1 <= row2)
		{
			if (column1 <= column2)
			{
				for (size_t i = row1; i <= row2; i++)
					for (size_t j = column1; j <= column2; j++)
						if (this->exists(i, j))
							if (this->get_node(i, j).expression != "0")
								count++;

				return count;
			}
			else
			{
				for (size_t i = row1; i <= row2; i++)
					for (size_t j = column2; j <= column1; j++)
						if (this->exists(i, j))
							if (this->get_node(i, j).expression != "0")
								count++;

				return count;
			}
		}
		else
		{
			if (column1 <= column2)
			{
				for (size_t i = row2; i <= row1; i++)
					for (size_t j = column1; j <= column2; j++)
						if (this->exists(i, j))
							if (this->get_node(i, j).expression != "0")
								count++;

				return count;
			}
			else
			{
				for (size_t i = row2; i <= row1; i++)
					for (size_t j = column2; j <= column1; j++)
						if (this->exists(i, j))
							if (this->get_node(i, j).expression != "0")
								count++;

				return count;
			}
		}

		return count;
	}

	if (this->is_relative(str2))
	{
		size_t count{};
		long int row1{}, row2{}, column1{}, column2{};
		row1 = this->get_row(str1);
		column1 = this->get_column(str1);
		row2 = this->get_row_relative(str2);
		column2 = this->get_column_relative(str2);
		if (row1 + row2 <= 0 || column1 + column2 <= 0)
		{
			std::cout << "Invalid row or column input in a relative adress!" << std::endl;
			return 0;
		}

		if (row1 > this->max_rows)
			this->max_rows = row1;

		if (column1 > this->max_columns)
			this->max_columns = column1;

		if (row1 + row2 > this->max_rows)
			this->max_rows = row1 + row2;

		if (column1 + column2 > this->max_columns)
			this->max_columns = column1 + column2;

		if (row1 <= row2)
		{
			if (column1 <= column2)
			{
				for (size_t i = row1; i <= row2; i++)
					for (size_t j = column1; j <= column2; j++)
						if (this->exists(i, j))
							if (this->get_node(i, j).expression != "0")
								count++;

				return count;
			}
			else
			{
				for (size_t i = row1; i <= row2; i++)
					for (size_t j = column2; j <= column1; j++)
						if (this->exists(i, j))
							if (this->get_node(i, j).expression != "0")
								count++;

				return count;
			}
		}
		else
		{
			if (column1 <= column2)
			{
				for (size_t i = row2; i <= row1; i++)
					for (size_t j = column1; j <= column2; j++)
						if (this->exists(i, j))
							if (this->get_node(i, j).expression != "0")
								count++;

				return count;
			}
			else
			{
				for (size_t i = row2; i <= row1; i++)
					for (size_t j = column2; j <= column1; j++)
						if (this->exists(i, j))
							if (this->get_node(i, j).expression != "0")
								count++;

				return count;
			}
		}
	}

	std::cout << "Invalid adresses input!" << std::endl;
	return 0;
}

void Table::SET(const long int row, const long int column, const string& expr, const string& current_node)
{
	if (row <= 0 || column <= 0)
	{
		std::cout << "Invalid row or column input!" << std::endl;
		return;
	}

	if (row > this->max_rows)
		this->max_rows = row;

	if (column > this->max_columns)
		this->max_columns = column;

	size_t size = this->table.size();
	bool changed_value = false;
	for (size_t i = 0; i < size; i++)
	{
		if (this->table[i].row == row && this->table[i].column == column)
		{
			changed_value = true;
			if (expr[0] == '"')
			{
				this->table[i].value = 0;
				this->table[i].expression = expr;
				break;
			}

			this->table[i].value = this->calculate_value(expr, current_node);
			this->table[i].expression = expr;
			break;
		}
	}

	for (size_t i = 0; i < size; i++)
		this->table[i].value = this->calculate_value(this->table[i].expression, "R" + std::to_string(this->table[i].row) + "C" + std::to_string(this->table[i].column));

	Node node;
	node.row = row;
	node.column = column;
	node.expression = expr;
	if (expr[0] == '"')
	{
		node.value = 0;
		this->table.push_back(node);
		return;
	}

	node.value = this->calculate_value(expr, current_node);
	this->table.push_back(node);
}

void Table::PRINT_VAL(const long int row, const long int column)
{
	if (row <= 0 || column <= 0)
	{
		std::cout << "Invalid row or column input!" << std::endl;
		return;
	}

	if (row > this->max_rows)
		this->max_rows = row;

	if (column > this->max_columns)
		this->max_columns = column;

	if (this->exists(row, column))
	{
		if (this->is_absolute(this->get_node(row, column).expression))
		{
			if (this->get_node(this->get_row(this->get_node(row, column).expression), this->get_column(this->get_node(row, column).expression)).expression[0] == '"')
			{
				std::cout << this->get_node(this->get_row(this->get_node(row, column).expression), this->get_column(this->get_node(row, column).expression)).expression;
				return;
			}
		}

		if (this->is_relative(this->get_node(row, column).expression))
		{
			if (this->get_node(this->get_row_relative(this->get_node(row, column).expression), this->get_column_relative(this->get_node(row, column).expression)).expression[0] == '"')
			{
				std::cout << this->get_node(this->get_row_relative(this->get_node(row, column).expression), this->get_column_relative(this->get_node(row, column).expression)).expression;
				return;
			}
		}

		if (this->get_node(row, column).expression[0] == '"')
			std::cout << this->get_node(row, column).expression;
		else
			std::cout << this->get_node(row, column).value;
		return;
	}

	std::cout << "0";
}

void Table::PRINT_EXPR(const long int row, const long int column)
{
	if (row <= 0 || column <= 0)
	{
		std::cout << "Invalid row or column input!" << std::endl;
		return;
	}

	if (row > this->max_rows)
		this->max_rows = row;

	if (column > this->max_columns)
		this->max_columns = column;

	if (this->exists(row, column))
	{
		std::cout << this->get_node(row, column).expression;
		return;
	}

	std::cout << "0";
}

void Table::PRINT_VAL_ALL()
{
	if (this->max_rows == 0 && this->max_columns == 0)
	{
		this->PRINT_VAL(1, 1);
		std::cout << " ; " << std::endl;
		return;
	}

	for (size_t i = 1; i <= this->max_rows; i++)
	{
		for (size_t j = 1; j <= this->max_columns; j++)
		{
			this->PRINT_VAL(i, j);
			std::cout << " ; ";
		}
		std::cout << std::endl;
	}
}

void Table::PRINT_EXPR_ALL()
{
	if (this->max_rows == 0 && this->max_columns == 0)
	{
		this->PRINT_EXPR(1, 1);
		std::cout << " ; " << std::endl;
		return;
	}

	for (size_t i = 1; i <= this->max_rows; i++)
	{
		for (size_t j = 1; j <= this->max_columns; j++)
		{
			this->PRINT_EXPR(i, j);
			std::cout << " ; ";
		}
		std::cout << std::endl;
	}
}

void Table::SAVE(const string& str)
{
	std::ofstream saveData(str, std::ios::beg);
	if (saveData.is_open())
	{
		for (size_t i = 1; i <= this->max_rows; i++)
		{
			for (size_t j = 1; j <= this->max_columns; j++)
			{
				if (this->exists(i, j))
					saveData << this->get_node(i, j).expression << " ; ";

				else
					saveData << "0 ; ";
			}

			saveData << std::endl;
		}

		std::cout << "Data Saved successfully!" << std::endl;
	}
	else
		std::cout << "File did not open!" << std::endl;
}

void Table::LOAD(const string& str)
{
	std::ifstream loadData(str, std::ios::beg);
	if (loadData.is_open())
	{
		string data{};
		char symbol{};
		while (!loadData.eof())
		{
			loadData.get(symbol);
			data += symbol;
		}
		size_t size = data.size();
		string node_expr{};
		Node node;
		long int row = 1, column = 1;
		data[size - 1] = '\0';
		for (size_t i = 0; i < size - 1; i++)
		{
			if (data[i] == '\n')
			{
				row++;
				if (data[i + 1] != '\0')
					column = 1;

				continue;
			}

			if (data[i] == ';')
			{
				column++;
				i += 1;
				continue;
			}

			node_expr += data[i];
			if (data[i + 2] == ';')
			{
				if (node_expr == "0")
				{
					node_expr = "";
					i += 1;
					continue;
				}

				node.row = row;
				node.column = column;
				node.expression = node_expr;
				this->table.push_back(node);
				node_expr = "";
				i += 1;
			}
		}

		this->max_rows = row - 1;
		this->max_columns = column - 1;
		size = this->table.size();
		for (size_t i = 0; i < size; i++)
			this->table[i].value = this->calculate_value(this->table[i].expression, "R" + std::to_string(this->table[i].row) + "C" + std::to_string(this->table[i].column));
		
		std::cout << "Data loaded successfully!" << std::endl;
	}
	else
		std::cout << "File did not open!" << std::endl;
}

void Table::increase_by_one(const long int row, const long int column)
{
	if (row <= 0 || column <= 0)
	{
		std::cout << "Invalid row or column input!" << std::endl;
		return;
	}

	if (row > this->max_rows)
		this->max_rows = row;

	if (column > this->max_columns)
		this->max_columns = column;

	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
	{
		if (this->table[i].row == row && this->table[i].column == column)
		{
			if (this->table[i].expression[0] == '"')
			{
				std::cout << "Cannot increase the value of a text box!" << std::endl;
				return;
			}

			if (this->is_absolute(this->get_node(row, column).expression) || this->is_relative(this->get_node(row, column).expression))
			{
				if (this->get_node(this->get_row(this->get_node(row, column).expression), this->get_column(this->get_node(row, column).expression)).expression[0] == '"')
				{
					std::cout << "Cannot increase the value of a text box!" << std::endl;
					return;
				}
			}

			if (this->table[i].value == (int)this->table[i].value)
			{
				this->table[i].value++;
				std::cout << "Value increased successfully!" << std::endl;
				return;
			}
			else
			{
				std::cout << "The value of that box is not a whole number, therefore cannot be increased!" << std::endl;
				return;
			}
		}
	}

	Node node;
	node.row = row;
	node.column = column;
	node.value = 1;
	node.expression = std::to_string(node.value);
	this->table.push_back(node);
	std::cout << "Value increased successfully!" << std::endl;
}

void Table::decrease_by_one(const long int row, const long int column)
{
	if (row <= 0 || column <= 0)
	{
		std::cout << "Invalid row or column input!" << std::endl;
		return;
	}

	if (row > this->max_rows)
		this->max_rows = row;

	if (column > this->max_columns)
		this->max_columns = column;

	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
	{
		if (this->table[i].row == row && this->table[i].column == column)
		{
			if (this->table[i].expression[0] == '"')
			{
				std::cout << "Cannot decrease the value of a text box!" << std::endl;
				return;
			}

			if (this->is_absolute(this->get_node(row, column).expression) || this->is_relative(this->get_node(row, column).expression))
			{
				if (this->get_node(this->get_row(this->get_node(row, column).expression), this->get_column(this->get_node(row, column).expression)).expression[0] == '"')
				{
					std::cout << "Cannot decrease the value of a text box!" << std::endl;
					return;
				}
			}

			if (this->table[i].value == (int)this->table[i].value)
			{
				this->table[i].value--;
				std::cout << "Value decreased successfully!" << std::endl;
				return;
			}
			else
			{
				std::cout << "The value of that box is not a whole number, therefore cannot be decreased!" << std::endl;
				return;
			}
		}
	}

	Node node;
	node.row = row;
	node.column = column;
	node.value = -1;
	node.expression = std::to_string(node.value);
	this->table.push_back(node);
	std::cout << "Value decreased successfully!" << std::endl;
}

const string Table::fix_expr(string expr)
{
	size_t size = expr.size();
	string new_expr{};
	for (size_t i = 0; i < size; i++)
	{
		if (expr[i] == '-' && expr[i + 1] == '-')
		{
			expr[i + 1] = '+';
			for (int j = i; j > 0; j--)
				expr[j] = expr[j - 1];

			for (size_t k = 1; k < size; k++)
				new_expr += expr[k];

			continue;
		}

		if (expr[i] == '+' && expr[i + 1] == '-')
		{
			for (int j = i; j > 0; j--)
				expr[j] = expr[j - 1];

			for (size_t k = 1; k < size; k++)
				new_expr += expr[k];

			continue;
		}

		if (!isDigit(expr[i]) && expr[i] == expr[i + 1] && expr[i] != '(' && expr[i] != ')')
			return "Incorrect expression input!";
	}

	if (new_expr != "")
		return new_expr;

	return expr;
}

const string Table::get_string1(const string& str) const
{
	size_t size = str.size();
	string command{};
	for (size_t i = 0; i < size; i++)
	{
		if (str[i] != ' ')
		{
			for (size_t j = i; j < size; j++)
			{
				if (str[j] == ' ')
					break;

				command += str[j];
			}
			break;
		}
	}

	return command;
}

const string Table::get_string2(const string& str) const
{
	size_t size = str.size();
	string adress{};
	size_t start_point = this->get_string1(str).size();
	for (size_t i = start_point; i < size; i++)
	{
		if (str[i] != ' ')
		{
			for (size_t j = i; j < size; j++)
			{
				if (str[j] == ' ')
					break;

				adress += str[j];
			}
			break;
		}
	}

	return adress;
}

const string Table::get_string3(const string& str) const
{
	string expression{};
	size_t size = str.size();
	bool flagS1 = false, flagS2 = false, text_box = false;
	for (size_t i = 0; i < size; i++)
	{
		if (str[i] == '"')
			text_box = true;

		if (flagS1 && flagS2 && text_box)
		{
			expression += str[i];
			continue;
		}

		if (str[i] == ' ')
			continue;

		if (str[i] != ' ' && !flagS1)
		{
			flagS1 = true;
			i += this->get_string1(str).size() - 1;
			continue;
		}

		if (str[i] != ' ' && !flagS2)
		{
			flagS2 = true;
			i += this->get_string2(str).size() - 1;
			continue;
		}

		if (flagS1 && flagS2)
			expression += str[i];
	}

	return expression;
}

const string Table::get_file_name(const string& str) const
{
	string file_name{};
	size_t size = str.size();
	bool flagS1 = false, flagS2 = false;;
	for (size_t i = 0; i < size; i++)
	{
		if (flagS2)
		{
			file_name += str[i];
			continue;
		}

		if (str[i] != ' ' && !flagS1)
		{
			flagS1 = true;
			i += this->get_string1(str).size() - 1;
			continue;
		}

		if (str[i] != ' ' && flagS1)
		{
			flagS2 = true;
			file_name += str[i];
		}
	}

	return file_name;
}

const long int Table::get_row(const string& str) const
{
	string number{};
	size_t size = str.size();
	for (size_t i = 1; i < size; i++)
	{
		if (str[i] == 'C')
			return stoi(number);

		number += str[i];
	}

	return stoi(number);
}

const long int Table::get_column(const string& str) const
{
	string number{};
	size_t size = str.size();
	bool flag = false;
	for (size_t i = 0; i < size; i++)
	{
		if (str[i] == 'C')
		{
			flag = true;
			continue;
		}

		if (flag)
			number += str[i];
	}

	return stoi(number);
}

const long int Table::get_row_relative(const string& str) const
{
	string number{};
	size_t size = str.size();
	for (size_t i = 2; i < size; i++)
	{
		if (str[i] == ']')
			return stoi(number);

		number += str[i];
	}

	return stoi(number);
}

const long int Table::get_column_relative(const string& str) const
{
	string number{};
	size_t size = str.size();
	bool flag = false;
	for (size_t i = 0; i < size; i++)
	{
		if (str[i] == 'C')
		{
			flag = true;
			i += 1;
			continue;
		}

		if (flag && str[i] != ']')
			number += str[i];
	}

	return stoi(number);
}

const bool Table::exists(const long int row, const long int column) const
{
	size_t size = this->table.size();
	for (size_t i = 0; i < size; i++)
		if (this->table[i].row == row && this->table[i].column == column)
			return true;

	return false;
}

const bool Table::check_expression(const string& str) const
{

	size_t size = str.size();
	if (size == 0)
		return false;

	if (size == 1 && str[0] == '"')
		return false;

	if (size == 2 && str[0] == '"' && str[1] == '"')
		return false;

	if (size > 2 && str[0] == '"' && str[size - 1] != '"')
		return false;

	if (size > 2 && str[0] != '"' && str[size - 1] == '"')
		return false;

	for (size_t i = 0; i < size; i++)
		if (!isDigit(str[i]) && str[i] != ' ' && str[0] != '"' && str[i] != '[' && str[i] != ']')
			if (str[i] != 'R' && str[i] != 'C' && str[i] != '(' && str[i] != ')' && str[i] != '>' && str[i] != '<')
				if (str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '^')
					return false;

	return true;
}

const bool Table::is_absolute(const string& str) const
{
	size_t size = str.size();
	if (size < 4)
		return false;

	if (str[0] != 'R' || !isDigit(str[1]) || !isDigit(str[size - 1]))
		return false;

	bool flagC = false;
	for (size_t i = 1; i < size; i++)
	{
		if (str[i] == ' ')
			return false;

		if (!isDigit(str[i]) && str[i] != 'C')
			return false;

		if (!flagC && i == size - 1)
			return false;

		if (str[i] == 'C')
			flagC = true;
	}

	if (!flagC)
		return false;

	return true;
}

const bool Table::is_relative(const string& str) const
{
	size_t size = str.size();
	if (size < 8)
		return false;

	if (str[0] != 'R' || str[1] != '[' || str[size - 1] != ']')
		return false;

	bool flag = true;
	bool flagC = false;
	size_t count = 1; // counting the '[' and ']'
	for (size_t i = 2; i < size; i++)
	{
		if (str[i] == '-' && i == 2)
			continue;

		if (str[i] == 'C' && flagC)
			return false;

		if (str[i] == 'C' && str[i + 1] != '[')
			return false;

		if (str[i] == 'C')
		{
			if (str[i + 2] == '-')
				i += 2;

			else
				i += 1;

			flagC = true;
			flag = true;
			count++;
			continue;
		}

		if (str[i] == ']' && i + 1 < size && str[i + 1] != 'C')
			return false;

		if (!isDigit(str[i]) && str[i] != '[' && str[i] != ']' && str[i] != 'C')
			return false;

		if (str[i] == '[' && flag)
			return false;

		if (str[i] == ']' && !flag)
			return false;

		if (str[i] == '[')
			flag = true;

		if (str[i] == ']')
			flag = false;

		if (str[i] == '[' || str[i] == ']')
			count++;
	}

	if (count != 4 || !flagC)
		return false;

	return true;
}

//const bool Table::bool_function(const string& str) const
//{
//	size_t size = str.size();
//	if (size < 5)
//		return false;
//
//	if (str[0] == 'I' && str[1] == 'F' && str[2] == '(' && str[size - 1] == ')')
//		return true;
//
//	if (str[0] == 'A' && str[1] == 'N' && str[2] == 'D' && str[3] == '(' && str[size - 1] == ')')
//		return true;
//
//	if (str[0] == 'N' && str[1] == 'O' && str[2] == 'T' && str[3] == '(' && str[size - 1] == ')')
//		return true;
//
//	if (str[0] == 'O' && str[1] == 'R' && str[2] == '(' && str[size - 1] == ')')
//		return true;
//
//	return false;
//}

//const bool Table::if_function(const string& condition, const int value_true, const int value_false)
//{
//	// if (a < b; 2; 3)
//	if (this->calculate_value())
//}

void Table::execute_proccess()
{
	string command{};
	string command2{};
	string command3{};
	string adress{};
	string expression{};
	string file_name{};
	string command_str{};
	long int row, column;
	std::cout << "Enter a command or type HELP to see the command list." << std::endl;
	while (command != "EXIT")
	{
		std::cout << "-------------------------------------------------------------------\n";
		std::cout << "Command: ";
		std::getline(std::cin, command_str);
		command = this->get_string1(command_str);
		command2 = this->get_string2(command_str);
		command3 = this->get_string3(command_str);
		
		if (command == "HELP")
		{
			std::cout << "> SET <adress> <expression>\n";
			std::cout << "> PRINT VAL <adress>\n";
			std::cout << "> PRINT EXPR <adress>\n";
			std::cout << "> PRINT VAL ALL\n";
			std::cout << "> PRINT EXPR ALL\n";
			std::cout << "> SAVE <file name>\n";
			std::cout << "> LOAD <file name>\n";
			std::cout << "> ++ <adress>\n";
			std::cout << "> -- <adress>\n";
			std::cout << "> SUM <adress1> <adress2>\n";
			continue;
		}

		if (command == "SET")
		{
			adress = command2;
			if (!this->is_absolute(adress))
			{
				std::cout << "Incorrect adress input!" << std::endl;
				continue;
			}
			expression = command3;
			if (!this->check_expression(expression))
			{
				std::cout << "Incorrect expression!" << std::endl;
				continue;
			}
			row = this->get_row(adress);
			column = this->get_column(adress);
			this->SET(row, column, expression, adress);
			continue;
		}

		if (command == "PRINT" && command2 == "VAL" && command3 != "ALL")
		{
			adress = command3;
			if (!this->is_absolute(adress))
			{
				std::cout << "Incorrect adress input!" << std::endl;
				continue;
			}
			row = this->get_row(adress);
			column = this->get_column(adress);
			this->PRINT_VAL(row, column);
			std::cout << std::endl;
			continue;
		}

		if (command == "PRINT" && command2 == "EXPR" && command3 != "ALL")
		{
			adress = command3;
			if (!this->is_absolute(adress))
			{
				std::cout << "Incorrect adress input!" << std::endl;
				continue;
			}
			row = this->get_row(adress);
			column = this->get_column(adress);
			this->PRINT_EXPR(row, column);
			std::cout << std::endl;
			continue;
		}

		if (command == "PRINT" && command2 == "VAL" && command3 == "ALL")
		{
			this->PRINT_VAL_ALL();
			continue;
		}

		if (command == "PRINT" && command2 == "EXPR" && command3 == "ALL")
		{
			this->PRINT_EXPR_ALL();
			continue;
		}

		if (command == "SAVE")
		{
			file_name = this->get_file_name(command_str);
			this->SAVE(file_name);
			continue;
		}

		if (command == "LOAD")
		{
			file_name = this->get_file_name(command_str);
			this->LOAD(file_name);
			continue;
		}

		if (command == "++")
		{
			adress = command2;
			if (!this->is_absolute(adress))
			{
				std::cout << "Incorrect adress input!" << std::endl;
				continue;
			}
			row = this->get_row(adress);
			column = this->get_column(adress);
			this->increase_by_one(row, column);
			continue;
		}

		if (command == "--")
		{
			adress = command2;
			if (!this->is_absolute(adress))
			{
				std::cout << "Incorrect adress input!" << std::endl;
				continue;
			}
			row = this->get_row(adress);
			column = this->get_column(adress);
			this->decrease_by_one(row, column);
			continue;
		}

		if (command == "SUM")
		{
			std::cout << this->SUM(command2, command3) << std::endl;
			continue;
		}

		if (command == "COUNT")
		{
			std::cout << this->COUNT(command2, command3) << std::endl;
			continue;
		}

		if (command == "EXIT")
			break;

		std::cout << "Invalid command input!" << std::endl;
	}
}

int main()
{
	Table table;
	table.execute_proccess();
	return 0;
}