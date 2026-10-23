#include <iostream>
#include <string>
#include<vector>
using namespace std;

struct Node {
	int departureTime;
	string destination;
	int seatsAvailable;
	Node* left;
	Node* right;

	Node(int time, string dest, int seats)
	{
		departureTime = time;
		destination = dest;
		seatsAvailable = seats;
		left = nullptr;
		right = nullptr;
	}
};

class ShuttleBST {
	Node* root;

	void clear(Node* current)
	{
		if (current == NULL)
		{
			return;
		}

		clear(current->left);
		clear(current->right);
		delete current;
	}

	Node* add(Node* temp, int time, string dest, int seats)
	{
		if (temp == NULL)
		{
			return new Node(time, dest, seats);
		}

		if (time < temp->departureTime)
		{
			temp->left = add(temp->left, time, dest, seats);
		}

		else if (time > temp->departureTime)
		{
			temp->right = add(temp->right, time, dest, seats);
		}

		return temp;
	}

	Node* search(Node* temp, int time)
	{
		if (temp == NULL || temp->departureTime == time)
		{
			return temp;
		}

		if (time < temp->departureTime)
		{
			return search(temp->left, time);
		}
		else
		{
			return search(temp->right, time);
		}
	}

	Node* minNode(Node* temp)
	{
		while (temp->left != NULL)
		{
			temp = temp->left;
		}
		return temp;
	}

	Node* remove(Node* temp, int time)
	{
		if (temp == NULL)
		{
			return NULL;
		}

		if (time < temp->departureTime)
		{
			temp->left = remove(temp->left, time);
		}
		else if (time > temp->departureTime)
		{
			temp->right = remove(temp->right, time);
		}
		else
		{
			if (temp->departureTime == time)
			{
				// Case-1: Leaf Node
				if (temp->left == NULL && temp->right == NULL)
				{
					delete temp;
					return NULL;
				}

				// Case-2(a): Left child Exists
				if (temp->left != NULL && temp->right == NULL)
				{
					Node* temp1 = temp->left;
					delete temp;
					return temp1;
				}

				// Case-2(b): Right Child Exists
				if (temp->left == NULL && temp->right != NULL)
				{
					Node* temp1 = temp->right;
					delete temp;
					return temp1;
				}

				// Case-3: Both Childs Exist
				if (temp->left != NULL && temp->right != NULL)
				{
					Node* succesor = minNode(temp->right);
					temp->departureTime = succesor->departureTime;
					temp->destination = succesor->destination;
					temp->seatsAvailable = succesor->seatsAvailable;

					temp->right = remove(temp->right, succesor->departureTime);
				}
			}
		}
		return temp;
	}

	void inorder(Node* temp, vector<Node*>& departures)
	{
		if (temp == nullptr)
			return;

		inorder(temp->left, departures);

		departures.push_back(temp);

		inorder(temp->right, departures);
	}


public:
	ShuttleBST()
	{
		root = nullptr;
	}

	~ShuttleBST()
	{
		clear(root);
	}

	void scheduleDeparture(int departTime, string dest, int seats)
	{
		if (departTime < 0 || departTime > 1439)
		{
			cout << "Invalid departure time" << endl;
			return;
		}

		if (seats <= 0)
		{
			cout << "Seats must be positive" << endl;
			return;
		}

		if (search(root, departTime) != nullptr)
		{
			cout << "Departure time already exists!" << endl;
			return;
		}

		root = add(root, departTime, dest, seats);
	}

	Node* findDeparture(int time)
	{
		return search(root, time);
	}

	void cancelDeparture(int time)
	{
		if (search(root, time) == nullptr)
		{
			cout << "Departure not found!" << endl;
			return;
		}

		root = remove(root, time);
		cout << "Departure cancelled successfully!" << endl;
	}


	void rescheduleDeparture(int oldTime, int newTime)
	{
		if (oldTime < 0 || oldTime > 1439 || newTime < 0 || newTime > 1439)
		{
			cout << "Invalid departure time" << endl;
			return;
		}

		if (oldTime == newTime)
		{
			cout << "Both times are same" << endl;
			return;
		}

		Node* oldNode = search(root, oldTime);

		if (oldNode == NULL)
		{
			cout << "Old departure not found!" << endl;
			return;
		}

		if (search(root, newTime) != NULL)
		{
			cout << "New time already occupied!" << endl;
			return;
		}

		string dest = oldNode->destination;
		int seats = oldNode->seatsAvailable;

		root = remove(root, oldTime);
		root = add(root, newTime, dest, seats);

		cout << "Departure rescheduled successfully!" << endl;
	}

	Node* nextDepartureAfter(int time)
	{
		Node* temp = root;
		Node* candidate = NULL;

		while (temp != NULL)
		{
			if (temp->departureTime > time)
			{
				candidate = temp;
				temp = temp->left;
			}
			else
			{
				temp = temp->right;
			}
		}

		if (candidate == NULL)
		{
			cout << "No next departure found!" << endl;
			return NULL;
		}

		cout << "Next Departure Time: " << candidate->departureTime << endl;

		cout << "Destination: " << candidate->destination << endl;

		cout << "Seats Available: " << candidate->seatsAvailable << endl;

		return candidate;
	}

	void listDeparturesBetween(int start, int end)
	{
		if (root == NULL)
		{
			cout << "No Departures Sheduled" << endl;
			return;
		}

		if (start == end)
		{
			cerr << "Start & End can't be Equal" << endl;
			return;
		}

		vector<Node*> list;
		inorder(root, list);


		cout << "Departure List from " << start << " - " << end << " is: " << endl;
		bool found = false;

		for (Node* departure : list)
		{
			if (departure->departureTime >= start && departure->departureTime <= end)
			{
				cout << "Departure Time: " << departure->departureTime << endl;
				cout << "Destination: " << departure->destination << endl;
				cout << "Seats Available: " << departure->seatsAvailable << endl;

				found = true;
			}
		}

		if (!found)
		{
			cout << "No departures found in this time range!" << endl;
		}
	}

	void printDailySchedule()
	{
		if (root == nullptr)
		{
			cout << "No departures scheduled!" << endl;
			return;
		}

		vector<Node*> departures;
		inorder(root, departures);

		cout << "\n===== Daily Shuttle Schedule =====" << endl;

		for (Node* departure : departures)
		{
			cout << "Departure Time: " << departure->departureTime << endl;
			cout << "Destination: " << departure->destination << endl;
			cout << "Seats Available: " << departure->seatsAvailable << endl;

			cout << "-----------------------------" << endl;
		}
	}

	void boardPassengers(int time, int passengers)
	{
		if (time < 0 || time > 1439)
		{
			cout << "Invalid departure time!" << endl;
			return;
		}

		if (passengers <= 0)
		{
			cout << "Passenger count must be positive!" << endl;
			return;
		}

		Node* departure = search(root, time);

		if (departure == nullptr)
		{
			cout << "Departure not found!" << endl;
			return;
		}

		if (passengers > departure->seatsAvailable)
		{
			cout << "Not enough seats available!" << endl;
			return;
		}

		departure->seatsAvailable -= passengers;

		cout << passengers << " passengers boarded successfully!" << endl;
		cout << "Remaining seats: "
			<< departure->seatsAvailable << endl;
	}
};

void menu()
{
	cout << "1 Schedule departure" << endl;
	cout << "2 Search departure" << endl;
	cout << "3 Cancel departure" << endl;
	cout << "4 Reschedule departure" << endl;
	cout << "5 Next departure" << endl;
	cout << "6 List departures within a time range" << endl;
	cout << "7 Board passengers" << endl;
	cout << "8 Print daily schedule" << endl;
	cout << "0 Exit" << endl;
}

int main()
{
	ShuttleBST bst;
	int choice = -1;

	while (choice != 0)
	{
		menu();
		do
		{
			cout << "Enter Your Choice: ";
			cin >> choice;

			if (cin.fail())
			{
				cerr << "ERROR: Invalid Input. Try Again" << endl;
				cout << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				choice = -1;
				continue;
			}

			if (choice < 0 || choice > 8)
			{
				cerr << "ERROR: Invalid Input. Try Again" << endl;
				cout << endl;
			}

		} while (choice < 0 || choice > 8);

		switch (choice)
		{
		case 0:
		{
			cout << "Exit Successful" << endl;
			return 0;
		}

		case 1:
		{
			cout << "Shedule A Deaprture Time" << endl;
			int time;
			string dest;
			int seats;

			do
			{
				cout << "Enter Departure Time: ";
				cin >> time;

				if (time < 0 || time > 1439)
				{
					cerr << "Invalid departure time" << endl;
					cout << endl;
				}

			} while (time < 0 || time > 1439);

			cout << "Enter Destination: ";
			cin >> dest;

			do
			{
				cout << "Enter Seats Available: ";
				cin >> seats;

				if (seats <= 0)
				{
					cerr << "Seats must be positive" << endl;
					cout << endl;
				}

			} while (seats <= 0);

			bst.scheduleDeparture(time, dest, seats);

			break;
		}

		case 2:
		{
			int time;

			do
			{
				cout << "Enter Departure Time to Find: ";
				cin >> time;

				if (time < 0 || time > 1439)
				{
					cerr << "Invalid departure time" << endl;
					cout << endl;
				}

			} while (time < 0 || time > 1439);

			Node* foundResult = bst.findDeparture(time);

			if (foundResult != nullptr)
			{
				cout << "Departure Time: " << foundResult->departureTime << endl;

				cout << "Destination: " << foundResult->destination << endl;

				cout << "Seats Available: " << foundResult->seatsAvailable << endl;
			}
			else
			{
				cout << "Departure not found!" << endl;
			}

			break;
		}

		case 3:
		{
			int time;

			do
			{
				cout << "Enter Departure Time to Cancel: ";
				cin >> time;

				if (time < 0 || time > 1439)
				{
					cerr << "Invalid departure time" << endl;
					cout << endl;
				}

			} while (time < 0 || time > 1439);

			bst.cancelDeparture(time);

			break;
		}

		case 4:
		{
			int oldTime;

			do
			{
				cout << "Enter Old Departure Time: ";
				cin >> oldTime;

				if (oldTime < 0 || oldTime > 1439)
				{
					cerr << "Invalid departure time" << endl;
					cout << endl;
				}

			} while (oldTime < 0 || oldTime > 1439);

			int newTime;

			do
			{
				cout << "Enter New Departure: ";
				cin >> newTime;

				if (newTime < 0 || newTime > 1439)
				{
					cerr << "Invalid departure time" << endl;
					cout << endl;
				}

			} while (newTime < 0 || newTime > 1439);

			bst.rescheduleDeparture(oldTime, newTime);

			break;
		}

		case 5:
		{
			int time;

			cout << "Enter departure time: ";
			cin >> time;

			if (time < 0 || time > 1439)
			{
				cout << "Invalid departure time!" << endl;
				break;
			}

			bst.nextDepartureAfter(time);


			break;
		}

		case 6:
		{
			int start, end;

			cout << "Enter Start Time: ";
			cin >> start;

			cout << "Enter End Time: ";
			cin >> end;

			if (start < 0 || start > 1439 || end < 0 || end > 1439 || start > end)
			{
				cout << "Invalid time range!" << endl;
				break;
			}

			bst.listDeparturesBetween(start, end);

			break;
		}

		case 7:
		{
			int time, passengers;

			cout << "Enter Departure Time: ";
			cin >> time;

			cout << "Enter Number of Passengers: ";
			cin >> passengers;

			bst.boardPassengers(time, passengers);

			break;
		}

		case 8:
		{
			bst.printDailySchedule();
			break;
		}
		}
	}

	return 0;
}
