#pragma once
#include <iostream>
using namespace std;

template <class T>
class Dlist {
public:
	class Node {
	public:
		T data;
		Node* next;
		Node* prev;

		Node(T d = NULL, Node* n = nullptr, Node* p = nullptr)
		{
			data = d;
			next = n;
			prev = p;
		}
	};

	class Iterator
	{
	public:
		friend class dlist;
		Node* curr;  

		Iterator(Node* cur = nullptr)
		{
			curr = cur;
		}

		Iterator& operator ++ ()
		{
			curr = curr->next;
			return *this;
		}

		Iterator operator ++ (int)

		{
			Iterator old = *this;
			curr = curr->next;
			return old;
		}

		Iterator& operator -- ()
		{
			curr = curr->prev;
			return *this;
		}

		Iterator operator -- (int)
		{
			Iterator old = *this;
			curr = curr->prev;
			return old;
		}

		T& operator * ()
		{
			return curr->data;
		}

		bool operator == (const Iterator& rhs)
		{
			if (this->curr->data == rhs.curr->data)
				return true;
			else
				return false;
		}

		bool operator != (const Iterator& rhs)
		{
			if (this->curr->data != rhs.curr->data)
				return true;
			else
				return false;
		}
	};

public:
	Node* head;
	Node* tail;

	Dlist()
	{
		head = new Node();
		tail = new Node();
		head->next = tail;
		tail->prev = head;
	}

	// move constructor 
	Dlist(Dlist&& other) 
	{
		head = other.head;
		tail = other.tail;

		other.head = nullptr;
		other.tail = nullptr;
	}


	void InsertAtStart(T d)
	{
		Node* temp = new Node(d);

		if (head->next == nullptr)
		{
			head->next = temp;
			temp->next = tail;
			temp->prev = head;
			tail->prev = temp;
		}

		else
		{
			head->next->prev = temp;
			temp->next = head->next;
			temp->prev = head;
			head->next = temp;
		}
	}

	void InsertAtEnd(T d)
	{
		Node* temp = new Node(d);
		if (head->next == nullptr)
		{
			InsertAtStart(d);
		}
		else
		{
			temp->prev = tail->prev;
			tail->prev->next = temp;
			tail->prev = temp;
			temp->next = tail;
		}
	}

	bool InsertAfter(Node* ptr, T d)
	{
		Node* temp = new Node();
		for (int i = 0; temp->next != nullptr; i++)
		{
			if (temp == ptr)
			{
				Node* NewNode = new Node(d);
				temp->next = ptr->next;
				temp->prev = ptr->prev;
				temp->next->prev = temp;
				ptr->next = temp;
				return true;
			}
			temp = temp->next;
		}
		return false;
	}

	void printlist()
	{
		cout << "Printing list: \n\n";
		Node* temp = head->next;
		for (int i = 0; temp->next != nullptr; i++)
		{
			cout << "Node number: " << i + 1;
			cout << "\nValue: " << *(temp->data) << endl << endl;
			temp = temp->next;
		}
	}

	T DeleteStart()
	{
		Node* temp = head->next;
		T temp1 = temp->data;
		if (head->next != nullptr)
		{
			head->next = temp->next;
			temp->next->prev = head;
			delete temp;
			return temp1;
		}
		return nullptr;
	}

	T DeleteEnd()
	{
		Node* temp = tail->prev;
		T temp1 = temp-> data;
		if (tail->prev != nullptr)
		{
			tail->prev = temp->prev;
			temp->prev->next = tail;
			delete temp;
			return temp1;
		}
	}

	T DeleteElement(T d)
	{
		Node* temp = head->next;
		for (int i = 0; temp != nullptr; i++)
		{
			if (temp->data == d)
			{
				temp->next->prev = temp->prev;
				temp->prev->next = temp->next;
				delete temp;
				break;
			}
			temp = temp->next;
		}
	}

	void RemoveDuplicates()
	{
		Node* temp = head->next;
		while (temp != nullptr)
		{
			Node* temp1 = temp->next;
			while (temp1 != nullptr)
			{
				if (temp->data == temp1->data)
				{
					temp1->next->prev = temp1->prev;
					temp1->prev->next = temp1->next;
				}
				temp1 = temp1->next;
			}
			temp = temp->next;
		}
	}

	~Dlist()
	{
		Node* temp = head;
		Node* temp1 = head->next;
		for (int i = 0; temp->next != nullptr; i++)
		{
			delete temp;
			temp = temp1;
			temp1 = temp1->next;
		}
		delete tail;
	}

	//
	// Iterator functions
	// 

	Iterator begin()
	{
		Dlist<T>::Iterator temp;
		temp.curr = head->next;
		return temp;
	}

	Iterator end()
	{
		Iterator temp;
		temp.curr = tail;
		return temp;
	}

	void printList()
	{
		Dlist <int> ::Iterator it;

		cout << "Printing list:\nHead";
		for (it = begin(); it != end(); it++)
		{
			cout << " -> ";
			cout << it.curr->data;
		}
	}

	bool isEmpty()
	{
		Iterator temp = begin();
		if(temp == end())
		{
			return true;
		}
		return false;
	}

	void displayTopCard()
	{
		Node* temp = head -> next;
		if (temp != tail)
		{
			cout << *(temp->data);
		}
		else
		{
			cout << hue::black;
			cout << "  " << "|" << "    " << "|";
			cout << hue::on_light_green;
		}
	}

	int size()
	{
		int i = 0;
		for (Iterator it = begin(); it != tail; it++)
		{
			i++;
		}
		return i;
	}

	/*T printElementTail(int i)
	{
		Iterator it = end();
		for (int j = 1; j <= i; j++)
		{
			it--;
			if (it.curr == head)
			{
				return nullptr;
			}
		}
		return it.curr->data;
	}*/

	/*void Mean()
	{
		Node* temp = head->next;
		int sum = 0;
		int j = 0;
		for (int i = 0; temp != nullptr; i++)
		{
			sum += temp->data;
			temp = temp->next;
			j = i;
		}

		cout << "The mean is: " << (sum / j);
	}*/

	/*void Median()
	{
		Node* fast = head->next;
		Node* slow = head;

		int j = 0;
		for (int i = 1; fast->next != tail; i++)
		{
			slow = slow->next;
			fast = fast->next->next;
			j = i;
		}

		j = j * 2;

		if ((j % 2) == 0)
		{
			slow = slow->next;
		}

		cout << "\nThe Median value is: " << slow->data << endl;
	}*/

	//void sort()   // bubble sort in ascending order
	//{
	//	Node* temp = head->next;
	//	for (int i = 0; temp != tail;)
	//	{
	//		Node* temp2 = head->next;
	//		for (int j = 0; temp2->next != tail;)
	//		{
	//			if (temp2->data > temp2->next->data)
	//			{
	//				T d = temp2->data;
	//				temp2->data = temp2->next->data;
	//				temp2->next->data = d;
	//			}
	//			temp2 = temp2->next;
	//		}
	//		temp = temp->next;
	//	}
	//}

	//void maxmin() // printing maximum and minimum data value in list
	//{
	//	Node* temp = head->next;

	//	int max = -999;
	//	int min = 999;

	//	for (int i = 0; temp != tail; temp = temp->next)
	//	{
	//		if (temp->data > max)
	//		{
	//			max = temp->data;
	//		}

	//		if (temp->data < min)
	//		{
	//			min = temp->data;
	//		}
	//	}

	//	cout << "\nThe maximum value is: " << max;
	//	cout << "\nThe minimum value is: " << min << endl;
	//}
};