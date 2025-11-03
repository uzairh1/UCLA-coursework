#include "Set.h"
using namespace std;

class Node;

Set::Set()
{
	head = nullptr;
	mSize = 0;
}

int Set::size() const
{
	return mSize;
}

bool Set::empty() const
{
	return mSize == 0;
}

bool Set::contains(const ItemType& value) const
{
	Node* current = head;
	for (current = head; current != nullptr; current = current->next)
	{
		if (current->value == value)
		{
			return true;
		}
	}
	return false;
}

Set::~Set()
{
	Node* current = head;
	while (current != nullptr)
	{
		Node* next = current->next;
		delete current;
		current = next;
	}

}

bool Set::insert(const ItemType& value) 
{
	if (this->contains(value)) // no duplicates
	{
		return false;
	}
	Node* newNode;
	newNode = new Node;
	newNode->value = value;
	newNode->next = head;
	newNode->prev = nullptr;
	if (head != nullptr)
	{
		head->prev = newNode;
	}
	head = newNode;
	mSize++;
	return true;


}

void Set::swap(Set& other)
{
	// Switch sizes

	int tempSize = this->mSize;
	this->mSize = other.mSize;
	other.mSize = tempSize;

	// Switch pointers

	Node* temp = this->head;
	this->head = other.head;
	other.head = temp;


}

Set& Set::operator=(const Set& rhs)
{
	if (this != &rhs)
	{
		Set temp(rhs);
		swap(temp);
	}
	return *this;
}

bool Set::erase(const ItemType& value)
{
	if (head == nullptr) // empty list
	{
		return false;
	}

	if ((head->value == value) && (head->next != nullptr)) // deleting the head in a multi-element list
	{
		Node* killMe = head;
		head = killMe->next;
		if (head != nullptr) {
		head->prev = nullptr;
		delete killMe;
		mSize--;
		return true;
		}
	}
	else if ((head->value == value) && (head->next == nullptr)) // deleting the head in a single element list
	{
		Node* killMe = head;
		head = nullptr;
		delete killMe;
		mSize--;
		return true;
	}
	else // relinking nodes to delete a node in the middle of a mult-element list
	{
		Node* current = head;
		while (current != nullptr)
		{
			if (current->next != nullptr && current->next->value == value)
			{
				break;
			}
			current = current->next;
		}
		if (current != nullptr)
		{
			Node* killMe = current->next;
			current->next = killMe->next;
			current->prev = current;
			delete killMe;
			mSize--;
			return true;
		}

	}
	return false;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	Set empty;
	for (int i = 0; i < s1.size(); i++)
	{
		ItemType x;
		s1.get(i, x);
		if (!empty.contains(x))
		{
		empty.insert(x);
		}
	}

	for (int j = 0; j < s2.size(); j++)
	{
		ItemType y;
		s2.get(j, y);
		if (!empty.contains(y))
		{ 
		empty.insert(y);
		}
	}

	result = empty;
} // relies on get 

Set::Set(const Set& other)
{
	mSize = other.mSize;
	if (other.head == nullptr) // copy an empty list
	{
		head = nullptr;
	}
	else
	{
		head = new Node; // copy the head
		head->value = other.head->value;
		head->next = nullptr;
		head->prev = nullptr;

		Node *thisCurrent = head;
		Node* otherCurrent = other.head;
		while (otherCurrent->next != nullptr)
		{
			thisCurrent-> next = new Node;
			thisCurrent->next->value = otherCurrent->next->value;
			thisCurrent->prev = thisCurrent; 
			thisCurrent->next->next = nullptr; // copy node by node


			thisCurrent = thisCurrent->next; // advance pointers
			otherCurrent = otherCurrent->next;
		}
	}
}

bool Set::get(int pos, ItemType& value) const  
{  
   if (!((pos >= 0) && (pos < mSize)))  
   {  
       return false;  
   }  

   int count = 0;  
   Node* current = head;  

   while (current != nullptr)  
   {  
       count = 0;  
       for (Node* compareAgainst = head; compareAgainst != nullptr; compareAgainst = compareAgainst->next)  
       {  
           if (current->value > compareAgainst->value)  
           {  
               count++;  
           }
       }  

       if (pos == count)  
       {  
           value = current->value;  
           return true;  
       }  

       current = current->next;  
   }  

   return false;  
}


void excludeInclude(const Set& s1, const Set& s2, Set& result) // relies on get
{

	Set empty;
	for (int i = 0; i < s1.size(); i++)
	{
		ItemType z;
		s1.get(i, z);
		if (!s2.contains(z) && !empty.contains(z))
		{
			empty.insert(z);
		}
	}

	for (int j = 0; j < s2.size(); j++)
	{
		ItemType y;
		s2.get(j, y);
		if (!s1.contains(y) && !empty.contains(y))
		{
			empty.insert(y);
		}
	}

	result = empty;
}
	
