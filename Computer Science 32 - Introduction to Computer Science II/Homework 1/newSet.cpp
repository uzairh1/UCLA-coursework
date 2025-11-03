#include "newSet.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Set::Set()
{
	mSize = DEFAULT_MAX_ITEMS;
	mSet = new ItemType[DEFAULT_MAX_ITEMS];
	currentFullness = 0;
}

Set::Set(int desiredSize)
{
	if (desiredSize < 0)
	{
		cout << "Bad argument passed into constructor.";
		exit(1);
	}
	mSize = desiredSize;
	mSet = new ItemType[desiredSize];
	currentFullness = 0;
}

bool Set::empty() const
{
	return currentFullness == 0;
}

int Set::size() const
{
	return currentFullness;
}

bool Set::contains(const ItemType& value) const
{
	bool found = false;
	for (int i = 0; i < currentFullness; i++)
	{
		if (mSet[i] == value)
		{
			found = true;
		}
	}
	return found;

}

bool Set::insert(const ItemType& value)
{
	for (int i = 0; i < currentFullness; i++)
	{
		if (mSet[i] == value)
		{
			return false;
		}
	}

	if (currentFullness == mSize) 
	{
		return false;
	}

	mSet[currentFullness] = value; // ?
	currentFullness++;
	return true;
}

bool Set::erase(const ItemType& value)
{
	for (int j = 0; j < currentFullness; j++)
	{
		if (mSet[j] == value)
		{
			for (int k = j; k < currentFullness; k++)
			{
				mSet[k] = mSet[k + 1];
			}
			// move everyone after mSet[j] one to the left! Remember rotateLeftByOne from CS31!
			currentFullness--;
			return true;
		}
	}
	return false;
}

bool Set::get(int i, ItemType& value) const
{
	if (!((i >= 0) && (i < currentFullness)))
	{
		return false;
	}

	for (int j = 0; j < currentFullness; j++)
	{
		int count = 0;

		for (int k = 0; k < currentFullness; k++)
		{
			if (mSet[j] > mSet[k])
			{
				count++;
			}
			if (mSet[j] == mSet[k])
			{
				continue;
			}
		}
		if (count == i)
		{
			value = mSet[j];
			return true;
		}
		else
		{
			;
		}
	}
	return false;
}

void Set::swap(Set& other)
{

	int temp = this->currentFullness;
	this->currentFullness = other.currentFullness;
	other.currentFullness = temp;

	int tempSize = this->mSize;
	this->mSize = other.mSize;
	other.mSize = tempSize;

	ItemType* mSetPtrTemp = mSet;
	mSet = other.mSet;
	other.mSet = mSetPtrTemp;

}

Set::~Set()
{
	delete[] mSet;
}

Set& Set::operator=(const Set& other)
{
	// other is on the RHS (right hand side)
	Set temp = Set(other);
	swap(temp); // I believe I don't need to refactor swap according to spec because I am using pointers
	return *this;



}

Set::Set(const Set& other)
{
	mSet = new ItemType[other.mSize];
	mSize = other.mSize;
	currentFullness = other.currentFullness;
	for (int j = 0; j < currentFullness; j++) 
	{
		mSet[j] = other.mSet[j];
	}
}