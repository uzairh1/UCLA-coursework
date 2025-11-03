#include <string>
#include <iostream>
#include <cctype>
#include "header.h"
using namespace std;

string findMaximum(const  string  array[], int  n)
{
	string max = "";
	if (n <= 0)
	{
		return "";
	}
	for (int i = 0; i <= n - 1; i++)
	{
		if (array[i] > max)
		{
			max = array[i];
		}
	}
	return max;
}

int countFloatingPointValues(const string array[], int  n)
{
	int count = 0;
	int decimalCount = 0;
	if (n <= 0)
	{
		return -1;
	}
	for (int i = 0; i <= n - 1; i++)
	{

		bool result = true;
		for (int j = 0; j <= array[i].size() - 1; j++) // loop through each individual string element
		{
			if (isdigit(array[i].at(j)) || (array[i].at(j) == '.'))
			{
				if (array[i].at(j) == '.')
				{
					decimalCount++;
				}
				result = true;
				if ((decimalCount != 0) && (decimalCount != 1))
				{
					result = false;
				}
			}
			else
			{
				result = false;
			}

		}
		if (result)
		{
			count++;
		}
	}
	return count;
}

bool hasNoCapitals(const string array[], int n)
{
	bool result = true;
	if (n <= 0)
	{
		return true;
	}
	for (int i = 0; i <= n - 1; i++)
	{
		for (int j = 0; j <= array[i].size() - 1; j++) // loop through individual string element
		{
			if (isupper(array[i].at(j)))
			{
				result = false;
			}
		}
	}
	return result;
}


bool identicalValuesTogether(const string array[], int n)
{
	bool result = true;
	if (n <= 0)
	{
		return false;
	}
	for (int i = 0; i <= n - 2; i++)
	{
		if (array[i] == array[i + 1])
		{
			result = true;
		}
		else
		{
			result = false;
		}
	}
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		string currValue = array[i];

		for (int j = i + 1; j < n; j++)
		{
			if (currValue == array[j])
			{
				count++;
				break;
			}
		}
	}
	if (count == 0)
	{
		result = true;
	}

	return result;
}
bool hasThreeOfX(const string array[], int n, string x)
{
	bool result = false;
	int count = 0;
	if (n <= 0)
	{
		return false;
	}
	for (int i = 0; i < n; i++)
	{
		if (array[i] == x)
		{
			count++;
		}

	}
	if (count == 3)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}


bool rotateLeft(string array[], int n, int amount)
{
	bool result = false;
	if (n <= 0 || (amount < 1))
	{
		return false;
	}
	if (amount % n == 0)
	{
		return false;
	}
	string oldValue = array[n - amount];
	array[n - amount] = array[0];
	for (int i = 1; i < n; i++)
	{
		array[i - amount] = array[i];
		if (i == n - amount)
		{
			array[i - amount] = oldValue;
		}
	}
	result = true;
	return result;

}

int replaceFirstAndLastOccurrences(string array[], int n, char charToFind, char charToReplace)
{
	int count = 0;
	if (n <= 0)
	{
		return -1;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j <= array[i].size() - 1; j++) // loop through individual string element to find first occurrence of charToFind.
		{
			if (array[i].at(j) == charToFind)
			{
				array[i].at(j) = charToReplace;
				count++;
				break;
			}
		}
		for (int k = array[i].size() - 1; k >= 0; k--) // loop through individual string element to find last occurrence of charToFind.
		{
			if (array[i].at(k) == charToFind)
			{
				array[i].at(k) = charToReplace;
				count++;
				break;
			}
		}

	}
	return count;

}

bool hasTwoOrMoreDuplicates(const string array[], int n)
{
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		string currValue = array[i];

		for (int j = i + 1; j < n; j++)
		{
			if (currValue == array[j])
			{
				count++;
				break;
			}
		}
	}
	return (count >= 2);
}


