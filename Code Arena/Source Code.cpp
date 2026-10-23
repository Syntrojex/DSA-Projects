#include <iostream>
#include <fstream>
#include <string>
using namespace std;

template <typename T>
struct StackNode {
	T data;
	StackNode* next;
};

template <typename T>
class Stack {
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
			cout << "Stack is Empty. Stack UnderFlow" << endl;
			return T();
		}

		StackNode<T>* n = top;
		T v = n->data;
		top = n->next;
		delete n;
		count--;
		return v;
	}

	T peek() const
	{
		if (isEmpty())
		{
			cout << "Stack is Empty. Stack UnderFlow" << endl;
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

struct ChallengeNode {
	string id;
	string expression;
	int answer;
	ChallengeNode* next;
};

class ChallengeQueue {
	ChallengeNode* front;
	ChallengeNode* rear;
	int count;

public:
	ChallengeQueue()
	{
		front = rear = NULL;
		count = 0;
	}

	~ChallengeQueue()
	{
		while (!isEmpty())
		{
			delete dequeue();
		}
	}

	void enqueue(const string& id, const string& expr, int answer)
	{
		ChallengeNode* newNode = new ChallengeNode{ id, expr, answer, nullptr };
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

	ChallengeNode* dequeue() // * bcz we have to return Node
	{
		if (isEmpty())
		{
			cout << "Queue is Empty" << endl;
			return nullptr;
		}

		ChallengeNode* toDelete = front;
		front = front->next;
		if (front == nullptr)
		{
			rear = nullptr;
		}

		toDelete->next = nullptr;
		count--;
		return toDelete;
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

struct PlayerNode {
	string id;
	string name;
	int score;
	int strikes;
	PlayerNode* next;
};

class CircularPlayerRing {
	PlayerNode* head;
	PlayerNode* tail;
	int count;

public:
	CircularPlayerRing()
	{
		head = tail = NULL;
		count = 0;
	}

	~CircularPlayerRing()
	{
		if (head == nullptr)
		{
			return;
		}

		tail->next = nullptr;       // break kia circle ko 

		while (head != nullptr)
		{
			PlayerNode* current = head;
			head = head->next;
			delete current;
		}
		tail = nullptr;
		count = 0;
	}

	void insertAtEnd(const string& id, const string& name)
	{
		PlayerNode* newNode = new PlayerNode{ id, name, 0, 0, nullptr };

		if (head == nullptr)
		{
			head = tail = newNode;
			newNode->next = newNode;            // single node point kre gi khud ko hi 
		}
		else
		{
			tail->next = newNode;
			newNode->next = head;
			tail = newNode;
		}
		count++;
	}

	bool idExists(const string& id) const
	{
		if (head == nullptr)
		{
			return false;
		}

		PlayerNode* temp = head;
		do
		{
			if (temp->id == id)
			{
				return true;
			}
			temp = temp->next;
		} while (temp != head);

		return false;
	}

	PlayerNode* removeNode(PlayerNode* prev, PlayerNode* curr)
	{
		if (curr == nullptr || head == nullptr)
		{
			return nullptr;
		}

		if (count == 1)
		{
			delete curr;
			head = tail = nullptr;
			count = 0;
			return nullptr;
		}

		PlayerNode* temp = curr->next;
		prev->next = temp;

		if (curr == head)
		{
			head = temp;
		}

		if (curr == tail)
		{
			tail = prev;
		}

		delete curr;
		count--;
		return temp;
	}

	bool isEmpty() const
	{
		return head == nullptr;
	}

	bool hasOneNode() const
	{
		return head != nullptr && head->next == head;
	}

	int size() const
	{
		return count;
	}

	PlayerNode* getHead() const
	{
		return head;
	}

	PlayerNode* getTail() const
	{
		return tail;
	}

	void printRing() const
	{
		if (head == nullptr)
		{
			cout << "EMPTY Ring" << endl;
			return;
		}

		cout << "Active ring is: ";
		PlayerNode* temp = head;
		do
		{
			cout << temp->id << " -> ";
			temp = temp->next;
		} while (temp != head);

		cout << "(back to " << head->id << ")" << endl;
	}

	void printRingDetailed() const
	{
		if (head == nullptr)
		{
			cout << "EMPTY Ring" << endl;
			return;
		}

		PlayerNode* temp = head;
		do
		{
			cout << temp->id << " " << temp->name << " (score=" << temp->score << " strikes=" << temp->strikes << ")";
			temp = temp->next;

			if (temp == head)
			{
				cout << endl;
			}
			else
			{
				cout << " -> ";
			}

		} while (temp != head);
	}
};

struct HistoryNode {
	int turnNo;
	string playerID;
	string playerName;
	string challengeID;
	string expression;
	string result;
	int submitted;
	string outcome;
	int score;
	int strikes;
	HistoryNode* next;
	HistoryNode* prev;
};

class MatchHistoryDLL {
	HistoryNode* head;
	HistoryNode* tail;
	int count;

	static void printNode(const HistoryNode* h)
	{
		cout << "Turn " << h->turnNo << " | " << h->playerID << " " << h->playerName
			<< " | " << h->challengeID << " | " << h->expression
			<< " | result=" << h->result << " | submitted=" << h->submitted
			<< " | " << h->outcome
			<< " | score=" << h->score << " strikes=" << h->strikes << endl;
	}

public:
	MatchHistoryDLL()
	{
		head = tail = NULL;
		count = 0;
	}

	~MatchHistoryDLL()
	{
		while (head != nullptr)
		{
			HistoryNode* toDelete = head;
			head = head->next;
			delete toDelete;
		}
	}

	void append(int turnNo, const string& pid, const string& pname, const string& cid, const string& expr, const string& result, int submitted, const string& outcome, int score, int strikes)
	{
		HistoryNode* newNode = new HistoryNode{ turnNo, pid, pname, cid, expr, result,submitted, outcome, score, strikes,nullptr, tail };

		if (tail == nullptr)
		{
			head = tail = newNode;
		}
		else
		{
			tail->next = newNode;
			tail = newNode;
		}
		count++;
	}

	void printForward() const
	{
		if (head == nullptr)
		{
			cout << "No turns were played" << endl;
			return;
		}

		for (HistoryNode* i = head; i != nullptr; i = i->next)
		{
			printNode(i);
		}
	}

	void printBackward() const
	{
		if (tail == nullptr)
		{
			cout << "No turns were played" << endl;
			return;
		}

		for (HistoryNode* i = tail; i != nullptr; i = i->prev)
		{
			printNode(i);
		}
	}

	bool isEmpty() const
	{
		return head == nullptr;
	}

	int size() const
	{
		return count;
	}
};

class ExpressionEngine {
	static char closerOf(char open)
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
public:
	bool isOperator(char c) const
	{
		return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
	}

	int precedence(char op) const
	{
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

	bool validateParentheses(const string& expr, string& err)
	{
		Stack<char> st;
		Stack<int> pos;
		for (size_t i = 0; i < expr.size(); i++)
		{
			char c = expr[i];
			if (c == '(' || c == '[' || c == '{')
			{
				st.push(c);
				pos.push((int)i + 1);
			}
			else if (c == ')' || c == ']' || c == '}')
			{
				if (st.isEmpty())
				{
					err = "extra closing '" + string(1, c) + "' at column " + to_string(i + 1) + " (no matching opener)";
					return false;
				}

				char open = st.pop();
				int openCol = pos.pop();

				if (closerOf(open) != c)
				{
					err = "delimiter mismatch: '" + string(1, open) + "' opened at column " +
						to_string(openCol) + " but found '" + string(1, c) +
						"' at column " + to_string(i + 1) + ", expected '" +
						string(1, closerOf(open)) + "'";
					return false;
				}
			}
		}

		if (!st.isEmpty())
		{
			char open = st.pop();
			int openCol = pos.pop();
			err = "'" + string(1, open) + "' opened at column " + to_string(openCol) +
				" is never closed, expected '" + string(1, closerOf(open)) + "'";
			return false;
		}
		return true;
	}

	int applyOp(int a, int b, char op, bool& ok, string& err)
	{
		ok = true;
		switch (op)
		{
		case '+':
		{
			return a + b;
		}

		case '-':
		{
			return a - b;
		}

		case '*':
		{
			return a * b;
		}

		case '/':
		{
			if (b == 0)
			{
				ok = false;
				err = "Division by zero";
				return 0;
			}

			return a / b;
		}

		case '%':
		{
			if (b == 0)
			{
				ok = false;
				err = "modulo by zero";
				return 0;
			}

			return a % b;
		}

		}
		ok = false;
		err = string("unknown operator '") + op + "'";
		return 0;
	}

	string infixToPostfix(const string& expr, bool& ok, string& err)
	{
		ok = true;
		string out;
		Stack<char> ops;
		bool expectOperand = true;

		size_t i = 0;
		while (i < expr.size())
		{
			char c = expr[i];
			if (c == ' ' || c == '\t')
			{
				i++;
				continue;
			}

			if (c >= '0' && c <= '9')
			{
				if (!expectOperand)
				{
					ok = false;
					err = "missing operator before number at column " + to_string(i + 1);
					return "";
				}

				long long val = 0;
				string num;
				while (i < expr.size() && expr[i] >= '0' && expr[i] <= '9')
				{
					val = val * 10 + (expr[i] - '0');
					if (val > 2147483647LL)
					{
						ok = false;
						err = "number too large near column " + to_string(i + 1);
						return "";
					}
					num += expr[i];
					i++;
				}
				out += num + " ";
				expectOperand = false;
				continue;
			}

			if (c == '(')
			{
				if (!expectOperand)
				{
					ok = false;
					err = "missing operator before '(' at column " + to_string(i + 1);
					return "";
				}
				ops.push(c);
				expectOperand = true;
				i++;
				continue;
			}

			if (c == ')')
			{
				if (expectOperand)
				{
					ok = false;
					err = "missing operand before ')' at column " + to_string(i + 1);
					return "";
				}
				bool found = false;
				while (!ops.isEmpty())
				{
					char t = ops.pop();
					if (t == '(')
					{
						found = true;
						break;
					}
					out += string(1, t) + " ";
				}
				if (!found)
				{
					ok = false;
					err = "extra ')' at column " + to_string(i + 1);
					return "";
				}
				expectOperand = false;
				i++;
				continue;
			}

			if (isOperator(c))
			{
				if (expectOperand)
				{
					ok = false;
					err = "operator '" + string(1, c) + "' at column " + to_string(i + 1) +
						" has no left operand";
					return "";
				}
				while (!ops.isEmpty() && ops.peek() != '(' &&
					precedence(ops.peek()) >= precedence(c))
				{
					out += string(1, ops.pop()) + " ";
				}
				ops.push(c);
				expectOperand = true;
				i++;
				continue;
			}

			ok = false;
			err = "invalid character '" + string(1, c) + "' at column " + to_string(i + 1);
			return "";
		}

		if (expectOperand)
		{
			ok = false;
			err = out.empty() && ops.isEmpty() ? "empty expression"
				: "expression ends without an operand";
			return "";
		}
		while (!ops.isEmpty())
		{
			char t = ops.pop();
			if (t == '(')
			{
				ok = false;
				err = "'(' is never closed";
				return "";
			}
			out += string(1, t) + " ";
		}
		return out;
	}

	int evaluatePostfix(const string& postfix, bool& ok, string& err)
	{
		ok = true;
		Stack<int> vals;
		size_t i = 0;
		while (i < postfix.size())
		{
			if (postfix[i] == ' ')
			{
				i++; continue;
			}
			if (postfix[i] >= '0' && postfix[i] <= '9') {
				long long v = 0;
				while (i < postfix.size() && postfix[i] >= '0' && postfix[i] <= '9') {
					v = v * 10 + (postfix[i] - '0');
					i++;
				}
				vals.push((int)v);
			}
			else {
				char op = postfix[i++];
				if (vals.size() < 2) {
					ok = false;
					err = "malformed expression (not enough operands)";
					return 0;
				}
				int b = vals.pop();
				int a = vals.pop();
				int r = applyOp(a, b, op, ok, err);
				if (!ok) return 0;
				vals.push(r);
			}
		}
		if (vals.size() != 1) {
			ok = false;
			err = "malformed expression";
			return 0;
		}
		return vals.pop();
	}

	bool evaluate(const string& expr, int& result, string& err)
	{
		if (!validateParentheses(expr, err)) return false;
		bool ok;
		string postfix = infixToPostfix(expr, ok, err);
		if (!ok) return false;
		result = evaluatePostfix(postfix, ok, err);
		return ok;
	}
};

string trim(const string& s)
{
	size_t a = s.find_first_not_of(" \t\r\n");
	if (a == string::npos) return "";
	size_t b = s.find_last_not_of(" \t\r\n");
	return s.substr(a, b - a + 1);
}

bool isNumber(const string& s)
{
	if (s.empty()) return false;
	size_t i = (s[0] == '-') ? 1 : 0;
	if (i == s.size()) return false;
	if (s.size() - i > 10) return false;

	for (; i < s.size(); i++)
		if (s[i] < '0' || s[i] > '9') return false;
	long long v = stoll(s);
	return v >= -2147483648LL && v <= 2147483647LL;
}

bool splitByPipe(const string& line, string parts[], int expected)
{
	int n = 0;
	size_t start = 0;
	while (true) {
		size_t bar = line.find('|', start);
		string field = (bar == string::npos) ? line.substr(start)
			: line.substr(start, bar - start);
		if (n >= expected) return false;
		parts[n++] = trim(field);
		if (bar == string::npos) break;
		start = bar + 1;
	}
	return n == expected;
}

bool loadPlayers(const string& filename, CircularPlayerRing& ring)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "ERROR: cannot open players file '" << filename << "'" << endl;
		return false;
	}
	string line, parts[2];
	int lineNo = 0;
	while (getline(fin, line))
	{
		lineNo++;
		if (lineNo == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) line = line.substr(3);
		line = trim(line);
		if (line.empty()) continue;
		if (!splitByPipe(line, parts, 2))
		{
			cout << "Players line " << lineNo << ": malformed (expected ID|Name) -> '" << line << "'\n";
			continue;
		}
		if (parts[0].empty() || parts[1].empty())
		{
			cout << "Players line " << lineNo << ": empty ID or name -> '" << line << "'\n";
			continue;
		}
		if (ring.idExists(parts[0]))
		{
			cout << "Players line " << lineNo << ": duplicate player ID '" << parts[0] << "' rejected\n";
			continue;
		}
		ring.insertAtEnd(parts[0], parts[1]);
	}
	return true;
}

bool loadChallenges(const string& filename, ChallengeQueue& q)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "ERROR: cannot open challenges file '" << filename << "'\n";
		return false;
	}
	string line, parts[3];
	int lineNo = 0;
	while (getline(fin, line))
	{
		lineNo++;
		if (lineNo == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) line = line.substr(3);
		line = trim(line);
		if (line.empty()) continue;
		if (!splitByPipe(line, parts, 3))
		{
			cout << "Challenges line " << lineNo << ": malformed (expected ID|Expression|Answer) -> '" << line << "'\n";
			continue;
		}
		if (parts[0].empty() || parts[1].empty())
		{
			cout << "Challenges line " << lineNo << ": empty ID or expression -> '" << line << "'\n";
			continue;
		}
		if (!isNumber(parts[2]))
		{
			cout << "Challenges line " << lineNo << ": submitted answer '" << parts[2] << "' is not an integer\n";
			continue;
		}
		q.enqueue(parts[0], parts[1], stoi(parts[2]));
	}
	return true;
}

class TournamentSimulator {
	CircularPlayerRing& ring;
	ChallengeQueue& queue;
	MatchHistoryDLL history;
	ExpressionEngine engine;
	int turnNo;

public:
	TournamentSimulator(CircularPlayerRing& r, ChallengeQueue& q)
		: ring(r), queue(q), turnNo(0) {
	}

	void printLoadReport() const
	{
		cout << "\nLOAD REPORT" << endl;
		cout << "Players loaded: " << ring.size() << endl;
		cout << "Challenges queued: " << queue.size() << endl;
	}

	void processTurn(PlayerNode*& current, PlayerNode*& prev)
	{
		ChallengeNode* card = queue.dequeue();
		turnNo++;

		int result = 0;
		string err;
		bool valid = engine.evaluate(card->expression, result, err);

		string outcome, resultStr;
		bool eliminated = false;
		if (!valid)
		{
			outcome = "INVALID_CHALLENGE";
			resultStr = "INVALID";
		}
		else {
			resultStr = to_string(result);
			if (result == card->answer)
			{
				current->score += 10;
				outcome = "CORRECT";
			}
			else
			{
				current->strikes++;
				outcome = "WRONG";
				if (current->strikes >= 2)
				{
					outcome = "ELIMINATED";
					eliminated = true;
				}
			}
		}

		string pid = current->id, pname = current->name;
		int score = current->score, strikes = current->strikes;

		history.append(turnNo, pid, pname, card->id, card->expression, resultStr,
			card->answer, outcome, score, strikes);

		cout << "Turn " << turnNo << " | " << pid << " " << pname << " | " << card->id
			<< " | result=" << resultStr << " | submitted=" << card->answer
			<< " | " << outcome << " | score=" << score << " strikes=" << strikes << "\n";
		if (!valid) cout << "   Invalid challenge " << card->id << ": " << err << "\n";
		if (eliminated) cout << "   " << pid << " " << pname << " is eliminated\n";

		if (eliminated)
		{
			current = ring.removeNode(prev, current);
		}
		else
		{
			prev = current;
			current = current->next;
		}
		ring.printRing();
		delete card;
	}

	void run()
	{
		printLoadReport();
		cout << "\nTURN TRACE" << endl;

		PlayerNode* current = ring.getHead();
		PlayerNode* prev = ring.getTail();

		while (!queue.isEmpty() && !ring.isEmpty())
		{
			processTurn(current, prev);
		}
		printFinalReport();
	}

	void printFinalReport() const
	{
		cout << "\nFINAL ACTIVE RING" << endl;
		ring.printRingDetailed();
		if (!queue.isEmpty())
			cout << "(unused challenges left in queue: " << queue.size() << ")" << endl;

		cout << "\nMATCH HISTORY - FORWARD" << endl;
		history.printForward();
		cout << "\nMATCH HISTORY - BACKWARD" << endl;
		history.printBackward();

		cout << "\nFINAL STATUS: "
			<< (ring.isEmpty() ? "NO_ACTIVE_PLAYERS" : "CHALLENGES_EXHAUSTED") << endl;
	}
};

int main()
{
	string playerFile, challengesFile;
	cout << "Players file Name: ";
	getline(cin, playerFile);
	cout << "Challenges file Name: ";
	getline(cin, challengesFile);

	playerFile = trim(playerFile);
	challengesFile = trim(challengesFile);

	CircularPlayerRing ring;
	ChallengeQueue queue;

	if (!loadPlayers(playerFile, ring)) return 1;
	if (!loadChallenges(challengesFile, queue)) return 1;

	TournamentSimulator sim(ring, queue);
	sim.run();

	return 0;
}
