#include<iostream>
using namespace std;

class PlayList {
public:

	class Node {
	public:
		Node* next;
		Node* prev;
		string song;

		Node(string song)
		{
			next = NULL;
			prev = NULL;
			this->song = song;
		}
	};

	class Iterator {
		Node* current;

	public:
		Iterator(Node* node)
		{
			current = node;
		}

		void operator++()
		{
			current = current->next;
		}

		void operator--()
		{
			current = current->prev;
		}

		string operator*()
		{
			return current->song;
		}

		bool operator!=(const Iterator& other)
		{
			return current != other.current;
		}
	};

	Node* head;
	Node* tail;
	int size;

public:
	PlayList()
	{
		head = NULL;
		tail = NULL;
		size = 0;
	}

	Iterator begin()
	{
		return Iterator(head);
	}

	Iterator end()
	{
		return Iterator(head); // one full revolution back to head marks end
	}

	Iterator rbegin()
	{
		return Iterator(tail);
	}

	Iterator rend()
	{
		return Iterator(tail); // one full revolution back to tail marks end
	}

	void addAtBeginning(string title)
	{
		Node* newNode = new Node(title);

		if (head == NULL)
		{
			head = newNode;
			tail = newNode;
			newNode->next = newNode;
			newNode->prev = newNode;
		}
		else
		{
			newNode->next = head;
			newNode->prev = tail;
			head->prev = newNode;
			tail->next = newNode;
			head = newNode;
		}

		size++;
	}

	void addAtEnd(string title)
	{
		Node* newNode = new Node(title);

		if (head == NULL)
		{
			head = newNode;
			tail = newNode;
			newNode->next = newNode;
			newNode->prev = newNode;
		}
		else
		{
			newNode->prev = tail;
			newNode->next = head;
			tail->next = newNode;
			head->prev = newNode;
			tail = newNode;
		}

		size++;
	}

	void addAtPosition(string title, int pos)
	{
		if (pos > size || pos < 0)
		{
			cout << "ERROR: Index Out of Bounds" << endl;
			return;
		}

		if (pos == 0)
		{
			addAtBeginning(title);
			return;
		}

		if (pos == size)
		{
			addAtEnd(title);
			return;
		}

		Node* temp = head;
		for (int i = 0; i < pos; i++)
		{
			temp = temp->next;
		}

		Node* newNode = new Node(title);

		newNode->next = temp;
		newNode->prev = temp->prev;
		temp->prev->next = newNode;
		temp->prev = newNode;

		size++;
	}

	void removeFromBeginning()
	{
		if (head == NULL)
		{
			cout << "PlayList is empty" << endl;
			return;
		}

		if (head == tail)
		{
			delete head;
			head = NULL;
			tail = NULL;
			size--;
			return;
		}

		Node* oldHead = head;
		head = head->next;
		head->prev = tail;
		tail->next = head;

		delete oldHead;
		size--;
	}

	void removeFromEnd()
	{
		if (head == NULL)
		{
			cout << "PlayList is empty" << endl;
			return;
		}

		if (head == tail)
		{
			delete head;
			head = NULL;
			tail = NULL;
			size--;
			return;
		}

		Node* oldTail = tail;
		tail = tail->prev;
		tail->next = head;
		head->prev = tail;

		delete oldTail;
		size--;
	}

	void removeFromPosition(int pos)
	{
		if (head == NULL)
		{
			cout << "PlayList is empty" << endl;
			return;
		}

		if (pos >= size || pos < 0)
		{
			cout << "ERROR: Index Out of Bounds" << endl;
			return;
		}

		if (pos == 0)
		{
			removeFromBeginning();
			return;
		}

		if (pos == size - 1)
		{
			removeFromEnd();
			return;
		}

		Node* temp = head;
		for (int i = 0; i < pos; i++)
		{
			temp = temp->next;
		}

		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;

		delete temp;
		size--;
	}

	void searchSong(string title)
	{
		if (head == NULL)
		{
			cout << "PlayList is Empty" << endl;
			return;
		}

		Node* temp = head;
		do
		{
			if (temp->song == title)
			{
				cout << "Song Found in PlayList" << endl;
				return;
			}
			temp = temp->next;
		} while (temp != head);

		cout << "Song Not Found in PlayList" << endl;
	}

	void displayPlayList()
	{
		if (head == NULL)
		{
			cout << "PlayList is Empty" << endl;
			return;
		}

		Node* temp = head;
		do
		{
			cout << temp->song << " -> ";
			temp = temp->next;
		} while (temp != head);
		cout << "(back to " << head->song << ")" << endl;
	}

	void traverseForward()
	{
		if (head == NULL)
		{
			cout << "PlayList is Empty" << endl;
			return;
		}

		cout << "Forward: ";
		Node* temp = head;
		do
		{
			cout << temp->song << " ";
			temp = temp->next;
		} while (temp != head);
		cout << endl;
	}

	void traverseReverse()
	{
		if (head == NULL)
		{
			cout << "PlayList is Empty" << endl;
			return;
		}

		cout << "Backward: ";
		Node* temp = tail;
		do
		{
			cout << temp->song << " ";
			temp = temp->prev;
		} while (temp != tail);
		cout << endl;
	}

	void playInLoop(int n)
	{
		if (head == NULL)
		{
			cout << "PlayList is Empty" << endl;
			return;
		}

		Node* temp = head;
		for (int i = 0; i < n; i++)
		{
			cout << temp->song;
			if (i != n - 1) cout << " -> ";
			temp = temp->next;
		}
		cout << endl;
	}

	void removeDuplicates()
	{
		if (head == NULL || head == tail)
			return;

		Node* outer = head;

		do
		{
			Node* inner = outer->next;

			while (inner != head)
			{
				Node* nextInner = inner->next;

				if (inner->song == outer->song)
				{
					inner->prev->next = inner->next;
					inner->next->prev = inner->prev;

					if (inner == tail)
						tail = inner->prev;

					delete inner;
					size--;
				}

				inner = nextInner;
			}

			outer = outer->next;

		} while (outer != head);
	}

	void nthFromEnd(int n)
	{
		if (head == NULL)
		{
			cout << "PlayList is Empty" << endl;
			return;
		}

		if (n <= 0 || n > size)
		{
			cout << "ERROR: Invalid N" << endl;
			return;
		}

		Node* temp = tail;
		for (int i = 1; i < n; i++)
		{
			temp = temp->prev;
		}

		cout << "Song " << n << " from end: " << temp->song << endl;
	}

	void isPalindrome()
	{
		if (head == NULL)
		{
			cout << "PlayList is Empty" << endl;
			return;
		}

		Node* front = head;
		Node* back = tail;

		for (int i = 0; i < size / 2; i++)
		{
			if (front->song != back->song)
			{
				cout << "Not a Palindrome" << endl;
				return;
			}
			front = front->next;
			back = back->prev;
		}

		cout << "Playlist is a Palindrome" << endl;
	}

	~PlayList()
	{
		if (head == NULL) return;

		Node* temp = head->next;
		while (temp != head)
		{
			Node* next = temp->next;
			delete temp;
			temp = next;
		}
		delete head;
		head = NULL;
		tail = NULL;
	}

};

void menu()
{
	cout << "\n==== Circular Playlist Manager ====" << endl;
	cout << "0.  Display PlayList" << endl;
	cout << "1.  Add Song at Beginning" << endl;
	cout << "2.  Add Song at End" << endl;
	cout << "3.  Add Song at Position" << endl;
	cout << "4.  Remove Song from Beginning" << endl;
	cout << "5.  Remove Song from End" << endl;
	cout << "6.  Remove Song from Position" << endl;
	cout << "7.  Search Song" << endl;
	cout << "8.  Traverse Forward" << endl;
	cout << "9.  Traverse Backward" << endl;
	cout << "10. Play N Songs on Loop" << endl;
	cout << "11. Remove Duplicate Songs" << endl;
	cout << "12. Find Song N from End" << endl;
	cout << "13. Check Playlist Palindrome" << endl;
	cout << "14. Exit" << endl;
	cout << "Enter Your Choice: ";
}

int main()
{
	PlayList p1;
	int choice = -1;

	while (choice != 14)
	{
		do {
			menu();
			cin >> choice;

			if (cin.fail())
			{
				cout << "ERROR: Invalid Choice! Try again." << endl;
				cin.clear();
				cin.ignore(10000, '\n');
				choice = -1;
			}
			else if (choice < 0 || choice > 14) {
				cout << "ERROR: Out of Range! Try again." << endl;
			}
		} while (choice < 0 || choice > 14);


		switch (choice)
		{

		case 0:
		{
			p1.displayPlayList();
			break;
		}

		case 1:
		{
			string song;
			cout << "Enter Song Name: ";
			cin >> song;

			p1.addAtBeginning(song);

			cout << "Song Added At Beginning Successfully" << endl;

			break;
		}

		case 2:
		{
			string song;
			cout << "Enter Song Name: ";
			cin >> song;

			p1.addAtEnd(song);

			cout << "Song Added At End Successfully" << endl;

			break;
		}

		case 3:
		{
			string song;
			cout << "Enter Song Name: ";
			cin >> song;

			int pos;
			cout << "Enter Position of Element: ";
			cin >> pos;

			while (cin.fail() || pos < 0)
			{
				cout << "ERROR:Try again" << endl;
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Enter Position of Element: ";
				cin >> pos;
			}

			p1.addAtPosition(song, pos);
			cout << "Song Added At " << pos << " Successfully" << endl;

			break;
		}

		case 4:
		{
			p1.removeFromBeginning();
			cout << "Song Removed From Beginning" << endl;

			break;
		}

		case 5:
		{
			p1.removeFromEnd();
			cout << "Song Removed From End" << endl;

			break;
		}

		case 6:
		{
			int pos;
			cout << "Enter Position of Element: ";
			cin >> pos;

			while (cin.fail() || pos < 0)
			{
				cout << "ERROR:Try again" << endl;
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Enter Position of Element: ";
				cin >> pos;
			}

			p1.removeFromPosition(pos);
			cout << "Song Removed from " << pos << endl;

			break;
		}

		case 7:
		{
			string song;
			cout << "Enter Song Name: ";
			cin >> song;
			p1.searchSong(song);

			break;
		}

		case 8:
		{
			p1.traverseForward();
			break;
		}

		case 9:
		{
			p1.traverseReverse();
			break;
		}

		case 10:
		{
			int n;
			cout << "Enter N: ";
			cin >> n;

			while (cin.fail() || n < 0)
			{
				cout << "ERROR:Try again" << endl;
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Enter N: ";
				cin >> n;
			}

			p1.playInLoop(n);
			break;
		}

		case 11:
		{
			p1.removeDuplicates();
			cout << "Duplicate Songs Removed" << endl;
			break;
		}

		case 12:
		{
			int n;
			cout << "Enter N: ";
			cin >> n;

			while (cin.fail() || n < 0)
			{
				cout << "ERROR:Try again" << endl;
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Enter N: ";
				cin >> n;
			}

			p1.nthFromEnd(n);
			break;
		}

		case 13:
		{
			p1.isPalindrome();
			break;
		}

		case 14:
		{
			return 0;
		}

		default:
			break;
		}
	}

	return 0;
}
