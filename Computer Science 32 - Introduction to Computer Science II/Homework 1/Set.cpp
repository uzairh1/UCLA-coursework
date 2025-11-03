#include "Set.h"
#include <iostream>
using namespace std;

Set::Set()
{
	mSize = 0;
}

bool Set::empty() const
{
	return mSize == 0;
}

int Set::size() const
{
	return mSize;
}

bool Set::contains(const ItemType& value) const
{
	bool found = false;
	for (int i = 0; i < mSize; i++)
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
	for (int i = 0; i < mSize; i++)
	{
		if (mSet[i] == value)
		{
			return false;
		}
	}

	if (mSize == DEFAULT_MAX_ITEMS)
	{
		return false;
	}

	mSet[mSize] = value; // ?
	mSize++;
	return true;
}

bool Set::erase(const ItemType& value)
{
	for (int j = 0; j < mSize; j++)
	{
		if (mSet[j] == value)
		{
			for (int k = j; k < mSize; k++)
			{
				mSet[k] = mSet[k + 1];
			}
			// move everyone after mSet[j] one to the left! Remember rotateLeftByOne from CS31!
			mSize--;
			return true;
		}
	}
	return false;
}

bool Set::get(int i, ItemType& value) const
{
	if (!((i >= 0) && (i < mSize)))
	{
		return false;
	}

	for (int j = 0; j < mSize; j++)
	{
		int count = 0;
		for (int k = 0; k < mSize; k++)
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
	Set copyOfFirstSet = Set(*this);
	Set copyOfSecondSet = Set(other);
	Set tempSet = copyOfFirstSet;
	*this = copyOfSecondSet;
	other = tempSet;



}