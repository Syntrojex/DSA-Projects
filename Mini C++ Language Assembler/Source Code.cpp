#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool isSpace(char c)
{
	return c == ' ' || c == '\t' || c == '\r';
}

bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool isIdentStart(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isIdentChar(char c)
{
	return isIdentStart(c) || isDigit(c);
}

string charToString(char c)
{
	string s = "";
	s += c;
	return s;
}

string subFrom(const string& s, size_t from)
{
	string result = "";
	for (size_t i = from; i < s.size(); i++)
	{
		result += s[i];
	}
	return result;
}

string trim(const string& s)
{
	size_t start = 0;
	while (start < s.size() && isSpace(s[start]))
	{
		start++;
	}

	size_t end = s.size();
	while (end > start && isSpace(s[end - 1]))
	{
		end--;
	}

	string result = "";
	for (size_t i = start; i < end; i++)
	{
		result += s[i];
	}
	return result;
}

string removeComment(const string& line)
{
	string result = "";
	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] == '/' && i + 1 < line.size() && line[i + 1] == '/')
		{
			break;
		}
		result += line[i];
	}
	return result;
}

template <typename T>
struct StackNode
{
	T data;
	StackNode* next;
};

template <typename T>
class Stack
{
	StackNode<T>* top;
	int count;

public:
	Stack()
	{
		top = nullptr;
		count = 0;
	}

	~Stack()
	{
		while (!isEmpty())
		{
			pop();
		}
	}

	void push(T val)
	{
		StackNode<T>* newNode = new StackNode<T>{ val, top };
		top = newNode;
		count++;
	}

	T pop()
	{
		if (isEmpty())
		{
			return T();
		}
		StackNode<T>* toPop = top;
		T popped = toPop->data;
		top = toPop->next;
		delete toPop;
		count--;
		return popped;
	}

	T peek() const
	{
		if (isEmpty())
		{
			return T();
		}
		return top->data;
	}

	bool isEmpty() const
	{
		return top == nullptr;
	}

	int size() const
	{
		return count;
	}
};

template <typename T>
struct QueueNode
{
	T data;
	QueueNode* next;
};

template <typename T>
class Queue
{
	QueueNode<T>* front;
	QueueNode<T>* rear;
	int count;

public:
	Queue()
	{
		front = rear = nullptr;
		count = 0;
	}

	~Queue()
	{
		while (front != nullptr)
		{
			QueueNode<T>* n = front;
			front = front->next;
			delete n;
		}
	}

	void enqueue(const T& val)
	{
		QueueNode<T>* newNode = new QueueNode<T>{ val, nullptr };
		if (isEmpty())
		{
			front = rear = newNode;
		}
		else
		{
			rear->next = newNode;
			rear = newNode;
		}
		count++;
	}

	bool dequeue(T& out)
	{
		if (isEmpty())
		{
			return false;
		}
		QueueNode<T>* n = front;
		out = n->data;
		front = front->next;
		if (front == nullptr)
		{
			rear = nullptr;
		}
		delete n;
		count--;
		return true;
	}

	bool isEmpty() const
	{
		return front == nullptr;
	}

	int size() const
	{
		return count;
	}
};

struct Statement
{
	int line = 0;
	string text;
	bool hasSemicolon = false;
};

struct SymbolNode
{
	string name;
	bool initialized;
	int value;
	int declLine;
	SymbolNode* next;
	SymbolNode* prev;
};

class SymbolTableDLL
{
	SymbolNode* head;
	SymbolNode* tail;

	static void printNode(const SymbolNode* n)
	{
		cout << n->name << " = ";
		if (n->initialized)
		{
			cout << n->value;
		}
		else
		{
			cout << "<uninitialized>";
		}
		cout << " (declared at line " << n->declLine << ")" << endl;
	}

public:
	SymbolTableDLL()
	{
		head = tail = nullptr;
	}

	~SymbolTableDLL()
	{
		while (head != nullptr)
		{
			SymbolNode* n = head;
			head = head->next;
			delete n;
		}
	}

	void append(const string& name, bool initialized, int value, int declLine)
	{
		SymbolNode* newNode = new SymbolNode{ name, initialized, value, declLine, nullptr, tail };
		if (tail == nullptr)
		{
			head = tail = newNode;
		}
		else
		{
			tail->next = newNode;
			tail = newNode;
		}
	}

	SymbolNode* find(const string& name) const
	{
		for (SymbolNode* p = head; p != nullptr; p = p->next)
		{
			if (p->name == name)
			{
				return p;
			}
		}
		return nullptr;
	}

	void printForward() const
	{
		if (head == nullptr)
		{
			cout << "(no variables declared)" << endl;
			return;
		}
		for (SymbolNode* p = head; p != nullptr; p = p->next)
		{
			printNode(p);
		}
	}

	void printBackward() const
	{
		if (tail == nullptr)
		{
			cout << "(no variables declared)" << endl;
			return;
		}
		for (SymbolNode* p = tail; p != nullptr; p = p->prev)
		{
			printNode(p);
		}
	}
};

char closerOf(char open)
{
	if (open == '(')
	{
		return ')';
	}
	if (open == '[')
	{
		return ']';
	}
	return '}';
}

class DelimiterChecker
{
	Stack<char> openers;
	Stack<int> openLines;
	Stack<int> openCols;

public:
	bool scanLine(const string& line, int lineNo, string& err)
	{
		for (size_t i = 0; i < line.size(); i++)
		{
			char c = line[i];
			int col = (int)i + 1;

			if (c == '(' || c == '[' || c == '{')
			{
				openers.push(c);
				openLines.push(lineNo);
				openCols.push(col);
			}
			else if (c == ')' || c == ']' || c == '}')
			{
				if (openers.isEmpty())
				{
					err = "Line " + to_string(lineNo) + ", Column " + to_string(col) +
						": unexpected closing '" + charToString(c) + "' (no matching opener)";
					return false;
				}

				char open = openers.pop();
				int openLine = openLines.pop();
				int openCol = openCols.pop();

				if (closerOf(open) != c)
				{
					err = "Line " + to_string(lineNo) + ", Column " + to_string(col) +
						": found '" + charToString(c) + "' but expected '" + charToString(closerOf(open)) +
						"' to close '" + charToString(open) + "' opened at line " + to_string(openLine) +
						", column " + to_string(openCol);
					return false;
				}
			}
		}
		return true;
	}

	bool finish(string& err)
	{
		if (openers.isEmpty())
		{
			return true;
		}

		char open = openers.pop();
		int openLine = openLines.pop();
		int openCol = openCols.pop();
		err = "Line " + to_string(openLine) + ", Column " + to_string(openCol) +
			": '" + charToString(open) + "' is never closed (expected '" + charToString(closerOf(open)) +
			"' before end of file)";
		return false;
	}
};

const int ERR_NONE = 0;
const int ERR_SYNTAX = 1;
const int ERR_SEMANTIC = 2;
const int ERR_RUNTIME = 3;

string errorKindName(int k)
{
	if (k == ERR_SYNTAX)
	{
		return "Syntax Error";
	}
	if (k == ERR_SEMANTIC)
	{
		return "Semantic Error";
	}
	if (k == ERR_RUNTIME)
	{
		return "Runtime Error";
	}
	return "No Error";
}

class ExpressionEngine
{
public:
	bool isOperator(char c) const
	{
		return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
	}

	int precedence(char op) const
	{
		if (op == '~')
		{
			return 3;
		}
		if (op == '*' || op == '/' || op == '%')
		{
			return 2;
		}
		if (op == '+' || op == '-')
		{
			return 1;
		}
		return 0;
	}

	int applyOp(int a, int b, char op, bool& ok, string& err)
	{
		ok = true;
		if (op == '+')
		{
			return a + b;
		}
		if (op == '-')
		{
			return a - b;
		}
		if (op == '*')
		{
			return a * b;
		}
		if (op == '/')
		{
			if (b == 0)
			{
				ok = false;
				err = "division by zero";
				return 0;
			}
			return a / b;
		}
		if (op == '%')
		{
			if (b == 0)
			{
				ok = false;
				err = "modulo by zero";
				return 0;
			}
			return a % b;
		}
		ok = false;
		err = "unknown operator";
		return 0;
	}

	string infixToPostfix(const string& expr, bool& ok, string& err)
	{
		ok = true;
		string out = "";
		Stack<char> ops;
		bool expectOperand = true;

		size_t i = 0;
		while (i < expr.size())
		{
			char c = expr[i];

			if (isSpace(c))
			{
				i++;
				continue;
			}

			if (isDigit(c))
			{
				if (!expectOperand)
				{
					ok = false;
					err = "missing operator before number at position " + to_string(i + 1) + " of expression";
					return "";
				}
				string num = "";
				while (i < expr.size() && isDigit(expr[i]))
				{
					num += expr[i];
					i++;
				}
				if (i < expr.size() && isIdentStart(expr[i]))
				{
					ok = false;
					err = "malformed identifier or number near position " + to_string(i + 1) + " of expression";
					return "";
				}
				out += num + " ";
				expectOperand = false;
				continue;
			}

			if (isIdentStart(c))
			{
				if (!expectOperand)
				{
					ok = false;
					err = "missing operator before variable at position " + to_string(i + 1) + " of expression";
					return "";
				}
				string name = "";
				while (i < expr.size() && isIdentChar(expr[i]))
				{
					name += expr[i];
					i++;
				}
				out += name + " ";
				expectOperand = false;
				continue;
			}

			if (c == '(')
			{
				if (!expectOperand)
				{
					ok = false;
					err = "missing operator before '(' at position " + to_string(i + 1) + " of expression";
					return "";
				}
				ops.push(c);
				i++;
				continue;
			}

			if (c == ')')
			{
				if (expectOperand)
				{
					ok = false;
					err = "missing operand before ')' at position " + to_string(i + 1) + " of expression";
					return "";
				}

				bool foundOpen = false;
				while (!ops.isEmpty())
				{
					char top = ops.pop();
					if (top == '(')
					{
						foundOpen = true;
						break;
					}
					out += charToString(top) + " ";
				}
				if (!foundOpen)
				{
					ok = false;
					err = "extra ')' at position " + to_string(i + 1) + " of expression";
					return "";
				}
				i++;
				continue;
			}

			if (isOperator(c))
			{
				if (expectOperand)
				{
					if (c == '-')
					{
						ops.push('~');
						i++;
						continue;
					}
					if (c == '+')
					{
						i++;
						continue;
					}
					ok = false;
					err = "operator '" + charToString(c) + "' at position " + to_string(i + 1) + " of expression has no left operand";
					return "";
				}

				while (!ops.isEmpty() && ops.peek() != '(' && precedence(ops.peek()) >= precedence(c))
				{
					out += charToString(ops.pop()) + " ";
				}
				ops.push(c);
				expectOperand = true;
				i++;
				continue;
			}

			ok = false;
			err = "invalid character '" + charToString(c) + "' at position " + to_string(i + 1) + " of expression";
			return "";
		}

		if (expectOperand)
		{
			ok = false;
			if (out == "" && ops.isEmpty())
			{
				err = "empty expression";
			}
			else
			{
				err = "expression ends without an operand";
			}
			return "";
		}

		while (!ops.isEmpty())
		{
			char top = ops.pop();
			if (top == '(')
			{
				ok = false;
				err = "'(' is never closed in expression";
				return "";
			}
			out += charToString(top) + " ";
		}
		return out;
	}

	int evaluatePostfix(const string& postfix, SymbolTableDLL& symbols, int& kind, bool& ok, string& err)
	{
		ok = true;
		kind = ERR_NONE;
		Stack<int> values;
		size_t i = 0;

		while (i < postfix.size())
		{
			char c = postfix[i];

			if (c == ' ')
			{
				i++;
			}
			else if (isDigit(c))
			{
				int number = 0;
				while (i < postfix.size() && isDigit(postfix[i]))
				{
					number = number * 10 + (postfix[i] - '0');
					i++;
				}
				values.push(number);
			}
			else if (isIdentStart(c))
			{
				string name = "";
				while (i < postfix.size() && isIdentChar(postfix[i]))
				{
					name += postfix[i];
					i++;
				}

				SymbolNode* sym = symbols.find(name);
				if (sym == nullptr)
				{
					ok = false;
					kind = ERR_SEMANTIC;
					err = "variable '" + name + "' used before declaration";
					return 0;
				}
				if (!sym->initialized)
				{
					ok = false;
					kind = ERR_SEMANTIC;
					err = "variable '" + name + "' is declared but uninitialized";
					return 0;
				}
				values.push(sym->value);
			}
			else if (c == '~')
			{
				i++;
				if (values.isEmpty())
				{
					ok = false;
					kind = ERR_SYNTAX;
					err = "malformed expression (nothing to negate)";
					return 0;
				}
				int a = values.pop();
				values.push(-a);
			}
			else
			{
				i++;
				if (values.size() < 2)
				{
					ok = false;
					kind = ERR_SYNTAX;
					err = "malformed expression (not enough operands)";
					return 0;
				}
				int b = values.pop();
				int a = values.pop();
				int result = applyOp(a, b, c, ok, err);
				if (!ok)
				{
					kind = ERR_RUNTIME;
					return 0;
				}
				values.push(result);
			}
		}

		if (values.size() != 1)
		{
			ok = false;
			kind = ERR_SYNTAX;
			err = "malformed expression";
			return 0;
		}
		return values.pop();
	}

	bool evaluate(const string& expr, SymbolTableDLL& symbols, int& result, int& kind, string& err)
	{
		kind = ERR_NONE;
		bool ok = true;

		string postfix = infixToPostfix(expr, ok, err);
		if (!ok)
		{
			kind = ERR_SYNTAX;
			return false;
		}

		result = evaluatePostfix(postfix, symbols, kind, ok, err);
		return ok;
	}
};

const int STMT_DECL = 0;
const int STMT_DECL_INIT = 1;
const int STMT_ASSIGN = 2;
const int STMT_OUTPUT = 3;

void skipSpaces(const string& t, size_t& pos)
{
	while (pos < t.size() && isSpace(t[pos]))
	{
		pos++;
	}
}

string readIdentifier(const string& t, size_t& pos)
{
	string name = "";
	if (pos >= t.size() || !isIdentStart(t[pos]))
	{
		return name;
	}
	while (pos < t.size() && isIdentChar(t[pos]))
	{
		name += t[pos];
		pos++;
	}
	return name;
}

bool isReservedWord(const string& name)
{
	return name == "int" || name == "cout";
}

bool parseStatement(const Statement& s, int& kind, string& name, string& expr, string& err)
{
	if (!s.hasSemicolon)
	{
		err = "missing semicolon at end of statement";
		return false;
	}

	const string& t = s.text;
	if (t == "")
	{
		err = "empty statement";
		return false;
	}

	size_t pos = 0;
	string word = readIdentifier(t, pos);
	if (word == "")
	{
		err = "unsupported statement (must start with 'int', 'cout' or a variable name)";
		return false;
	}

	if (word == "cout")
	{
		skipSpaces(t, pos);
		if (pos + 1 >= t.size() || t[pos] != '<' || t[pos + 1] != '<')
		{
			err = "malformed output statement, expected: cout << expression;";
			return false;
		}
		expr = trim(subFrom(t, pos + 2));
		if (expr == "")
		{
			err = "missing expression after 'cout <<'";
			return false;
		}
		kind = STMT_OUTPUT;
		return true;
	}

	if (word == "int")
	{
		skipSpaces(t, pos);
		name = readIdentifier(t, pos);
		if (name == "")
		{
			err = "malformed declaration or identifier, expected: int <identifier>;";
			return false;
		}
		if (isReservedWord(name))
		{
			err = "'" + name + "' is a reserved word and cannot be a variable name";
			return false;
		}

		skipSpaces(t, pos);
		if (pos == t.size())
		{
			kind = STMT_DECL;
			return true;
		}
		if (t[pos] != '=')
		{
			err = "unexpected text after variable name '" + name + "'";
			return false;
		}
		expr = trim(subFrom(t, pos + 1));
		if (expr == "")
		{
			err = "missing expression after '='";
			return false;
		}
		kind = STMT_DECL_INIT;
		return true;
	}

	name = word;
	skipSpaces(t, pos);
	if (pos < t.size() && t[pos] == '=')
	{
		expr = trim(subFrom(t, pos + 1));
		if (expr == "")
		{
			err = "missing expression after '='";
			return false;
		}
		kind = STMT_ASSIGN;
		return true;
	}

	err = "unsupported statement '" + t + "'";
	return false;
}

class Interpreter
{
	SymbolTableDLL& symbols;
	ExpressionEngine engine;
	Queue<int> outputs;

	bool execute(const Statement& s, int& kind, string& err)
	{
		int stmtKind = 0;
		string name = "";
		string expr = "";

		if (!parseStatement(s, stmtKind, name, expr, err))
		{
			kind = ERR_SYNTAX;
			return false;
		}

		int value = 0;

		if (stmtKind == STMT_DECL || stmtKind == STMT_DECL_INIT)
		{
			SymbolNode* existing = symbols.find(name);
			if (existing != nullptr)
			{
				kind = ERR_SEMANTIC;
				err = "duplicate declaration of '" + name + "' (first declared at line " + to_string(existing->declLine) + ")";
				return false;
			}

			if (stmtKind == STMT_DECL)
			{
				symbols.append(name, false, 0, s.line);
				return true;
			}

			if (!engine.evaluate(expr, symbols, value, kind, err))
			{
				return false;
			}
			symbols.append(name, true, value, s.line);
			return true;
		}

		if (stmtKind == STMT_ASSIGN)
		{
			SymbolNode* node = symbols.find(name);
			if (node == nullptr)
			{
				kind = ERR_SEMANTIC;
				err = "variable '" + name + "' used before declaration";
				return false;
			}
			if (!engine.evaluate(expr, symbols, value, kind, err))
			{
				return false;
			}
			node->initialized = true;
			node->value = value;
			return true;
		}

		if (!engine.evaluate(expr, symbols, value, kind, err))
		{
			return false;
		}
		outputs.enqueue(value);
		return true;
	}

public:
	Interpreter(SymbolTableDLL& table) : symbols(table)
	{
	}

	bool run(Queue<Statement>& statements, int& kind, string& err)
	{
		Statement s;
		while (statements.dequeue(s))
		{
			string shown = s.text;
			if (s.hasSemicolon)
			{
				shown += ";";
			}

			if (!execute(s, kind, err))
			{
				cout << "Line " << s.line << ": " << shown << " -> ERROR [" << errorKindName(kind) << "]: " << err << endl;
				return false;
			}
			cout << "Line " << s.line << ": " << shown << " -> OK" << endl;
		}
		return true;
	}

	void printOutputs()
	{
		if (outputs.isEmpty())
		{
			cout << "(no output)" << endl;
			return;
		}

		int value;
		while (outputs.dequeue(value))
		{
			cout << value << endl;
		}
	}
};

const int LOAD_OK = 0;
const int LOAD_CANNOT_OPEN = 1;
const int LOAD_STRUCTURAL_ERROR = 2;

void splitLine(const string& line, int lineNo, Queue<Statement>& q)
{
	string current = "";
	for (size_t i = 0; i < line.size(); i++)
	{
		char c = line[i];

		if ((c == '{' || c == '}') && trim(current) == "")
		{
			continue;
		}

		if (c == ';')
		{
			Statement s;
			s.line = lineNo;
			s.text = trim(current);
			s.hasSemicolon = true;
			q.enqueue(s);
			current = "";
		}
		else
		{
			current += c;
		}
	}

	if (trim(current) != "")
	{
		Statement s;
		s.line = lineNo;
		s.text = trim(current);
		s.hasSemicolon = false;
		q.enqueue(s);
	}
}

int loadSource(const string& filename, Queue<Statement>& q, string& structuralErr)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		return LOAD_CANNOT_OPEN;
	}

	DelimiterChecker checker;
	string line;
	int lineNo = 0;

	while (getline(fin, line))
	{
		lineNo++;

		if (lineNo == 1 && line.size() >= 3 && (unsigned char)line[0] == 0xEF &&
			(unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF)
		{
			line = subFrom(line, 3);
		}

		line = removeComment(line);

		if (trim(line) == "")
		{
			continue;
		}

		if (!checker.scanLine(line, lineNo, structuralErr))
		{
			return LOAD_STRUCTURAL_ERROR;
		}
		splitLine(line, lineNo, q);
	}

	if (!checker.finish(structuralErr))
	{
		return LOAD_STRUCTURAL_ERROR;
	}
	return LOAD_OK;
}

int main(int argc, char* argv[])
{
	string filename;
	if (argc > 1)
	{
		filename = argv[1];
	}
	else
	{
		cout << "Source file name: ";
		getline(cin, filename);
	}
	filename = trim(filename);

	Queue<Statement> statements;
	string structuralErr = "";

	int loaded = loadSource(filename, statements, structuralErr);
	cout << "FILE: " << filename << endl;

	if (loaded == LOAD_CANNOT_OPEN)
	{
		cout << "ERROR: cannot open source file '" << filename << "'" << endl;
		return 1;
	}

	if (loaded == LOAD_STRUCTURAL_ERROR)
	{
		cout << "STRUCTURAL CHECK: FAIL" << endl;
		cout << structuralErr << endl;
		cout << endl;
		cout << "FINAL STATUS: FAILED (structural error)" << endl;
		return 0;
	}

	cout << "STRUCTURAL CHECK: PASS" << endl;
	cout << endl;
	cout << "EXECUTION TRACE" << endl;

	SymbolTableDLL symbols;
	Interpreter interpreter(symbols);
	int kind = ERR_NONE;
	string err = "";
	bool success = interpreter.run(statements, kind, err);

	cout << endl;
	cout << "PROGRAM OUTPUT" << endl;
	interpreter.printOutputs();

	cout << endl;
	cout << "SYMBOL TABLE - FORWARD" << endl;
	symbols.printForward();

	cout << endl;
	cout << "SYMBOL TABLE - BACKWARD" << endl;
	symbols.printBackward();

	cout << endl;
	if (success)
	{
		cout << "FINAL STATUS: SUCCESS" << endl;
	}
	else
	{
		cout << "FINAL STATUS: FAILED (" << errorKindName(kind) << ")" << endl;
	}
	return 0;
}

