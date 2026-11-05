#include <iostream>
#include <string>

using namespace std;

struct StopNode {
	string name;
	StopNode* next;
	StopNode* prev;
};
// One entry in the undo history

struct Action {
	char type; // 'A' = a stop was added, 'R' = a stop was removed
	string stop; // the stop that was added or removed
	string after; // for 'R': the stop that was just before it
};

template <typename T>
class Stack {
	struct StackNode {
		T data;
		StackNode* next;

		StackNode(T data)
		{
			this->data = data;
			next = nullptr;
		}
	};

	StackNode* head;
public:

	Stack()
	{
		head = nullptr;
	}

	~Stack()
	{
		while (!isEmpty())
		{
			pop();
		}
	}

	bool isEmpty()
	{
		return head == nullptr;
	}

	void push(T value)
	{
		StackNode* newNode = new StackNode(value);
		newNode->next = head;
		head = newNode;
	}

	void pop()
	{
		if (isEmpty())
		{
			return;
		}

		StackNode* temp = head;
		head = head->next;
		delete temp;
	}

	T top()
	{
		if (!isEmpty())
		{
			return head->data;
		}
		return T();
	}
};

class Route {
	StopNode* current;
	Stack<Action> history;
public:
	Route() : current(nullptr) {}

	~Route()
	{
		if (!current)
		{
			return;
		}

		StopNode* head = current;
		StopNode* curr = current;
		do
		{
			StopNode* nextNode = curr->next;
			delete curr;
			curr = nextNode;
		} while (curr != head);

		current = nullptr;
	}

	void addStop(string name)
	{
		StopNode* newNode = new StopNode{ name, nullptr, nullptr };

		if (!current)
		{
			newNode->next = newNode;
			newNode->prev = newNode;
			current = newNode;
		}
		else
		{
			StopNode* previousNode = current->prev;

			newNode->next = current;
			newNode->prev = previousNode;
			previousNode->next = newNode;
			current->prev = newNode;
		}

		history.push(Action{ 'A', name, "" });
	}

	bool removeStop(string name)
	{
		if (!current)
		{
			return false;
		}

		StopNode* target = current;
		bool found = false;
		do
		{
			if (target->name == name)
			{
				found = true;
				break;
			}
			target = target->next;
		} while (target != current);

		if (!found)
		{
			return false;
		}

		string afterName = target->prev->name;
		history.push(Action{ 'R', name, afterName });

		if (target->next == target)
		{
			delete target;
			current = nullptr;
		}
		else
		{
			if (current == target)
			{
				current = target->next;
			}
			target->prev->next = target->next;
			target->next->prev = target->prev;
			delete target;
		}
		return true;
	}

	void moveForward(int k)
	{
		if (!current) return;
		for (int i = 0; i < k; i++)
		{
			current = current->next;
		}
	}

	void moveBackward(int k)
	{
		if (!current) return;
		for (int i = 0; i < k; i++)
		{
			current = current->prev;
		}
	}

	void printRoute()
	{
		if (!current)
		{
			cout << "(empty)" << endl;
			return;
		}
		StopNode* temp = current;
		do {
			cout << temp->name << " -> ";
			temp = temp->next;
		} while (temp != current);
		cout << "(loop)" << endl;
	}

	void printReverse()
	{
		if (!current)
		{
			cout << "(empty)" << endl;
			return;
		}
		printReverseRec(current, current, true);
	}

	void undo()
	{
		if (history.isEmpty()) return;

		Action lastAction = history.top();
		history.pop();

		if (lastAction.type == 'A')
		{
			string nameToRemove = lastAction.stop;
			if (!current) return;

			StopNode* target = current;
			bool found = false;
			do
			{
				if (target->name == nameToRemove)
				{
					found = true;
					break;
				}
				target = target->next;
			} while (target != current);

			if (found)
			{
				if (target->next == target)
				{
					delete target;
					current = nullptr;
				}
				else
				{
					if (current == target)
					{
						current = target->next;
					}
					target->prev->next = target->next;
					target->next->prev = target->prev;
					delete target;
				}
			}
		}
		else if (lastAction.type == 'R')
		{
			StopNode* newNode = new StopNode{ lastAction.stop, nullptr, nullptr };

			if (!current)
			{
				newNode->next = newNode;
				newNode->prev = newNode;
				current = newNode;
			}
			else
			{
				StopNode* anchor = current;
				bool foundAnchor = false;
				do
				{
					if (anchor->name == lastAction.after)
					{
						foundAnchor = true;
						break;
					}
					anchor = anchor->next;
				} while (anchor != current);

				if (foundAnchor)
				{
					StopNode* downstreamNode = anchor->next;
					newNode->next = downstreamNode;
					newNode->prev = anchor;
					anchor->next = newNode;
					downstreamNode->prev = newNode;
				}
				else
				{
					newNode->next = current;
					newNode->prev = current->prev;
					current->prev->next = newNode;
					current->prev = newNode;
				}
			}
		}
	}

private:
	void printReverseRec(StopNode* p, StopNode* start, bool first)
	{
		if (!first && p == start)
		{
			cout << "(loop)" << endl;
			return;
		}

		cout << p->prev->name << " -> ";
		printReverseRec(p->prev, start, false);
	}
};

int main()
{
	Route r;
	r.addStop("Kalma Chowk");
	r.addStop("Qaddafi Stadium");
	r.addStop("Jail Road");
	r.addStop("Chauburji");
	r.addStop("Azadi Chowk");

	r.printRoute();

	r.moveForward(2);
	r.printRoute();

	r.moveBackward(3);
	r.printReverse();

	r.removeStop("Jail Road");
	r.printRoute();

	r.undo();
	r.printRoute();

	r.undo();
	r.printRoute();

	return 0;
}
