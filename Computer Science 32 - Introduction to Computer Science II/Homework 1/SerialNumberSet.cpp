#include "SerialNumberSet.h"
#include <iostream>
using namespace std;

SerialNumberSet::SerialNumberSet()
{

}

int SerialNumberSet::size() const
{
	return mSerialNumberSet.size();
}

bool SerialNumberSet::add(unsigned long serialNum)
{
	
	return mSerialNumberSet.insert(serialNum);
}

void SerialNumberSet::print() const
{

	ItemType s;
	for (int i = 0; i < mSerialNumberSet.size(); i++)
	{
		mSerialNumberSet.get(i, s);
		cout << s << endl;
	}


}