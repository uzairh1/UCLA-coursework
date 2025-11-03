

// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const string a[], int n);

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const string a[], int n);

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const string a[], int n);

// Return the subscript of the first string in the array that is >= all
// strings in the array (i.e., return the smallest subscript m such
// that a[m] >= a[k] for all k from 0 to n-1).  If the function is told
// that no strings are to be considered to be in the array, return -1.
int locateMax(const string a[], int n);


// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    "donald" "keir" "claudia" "jinping" "keir" "claudia" "narendra"
// then the function should return true if a2 is
//    "keir" "jinping" "narendra"
// or
//    "keir" "claudia" "claudia"
// and it should return false if a2 is
//    "keir" "narendra" "jinping"
// or
//    "donald" "jinping" "jinping"


bool contains(const string a1[], int n1, const string a2[], int n2); 



bool allTrue(const string a[], int n)
{
	if (n <= 0)
	{
		return false;
	}
	if (n == 1)
	{
		return somePredicate(a[0]);
	}
	return allTrue(a + 1, n - 1);

}

int firstTrue(const string a[], int n)
{
	if (n <= 0)
	{
		return -1;
	}
	if (somePredicate(a[n-1]))
	{
		return n-1;
	}
	return firstTrue(a + 1, n - 1);
}

int locateMax(const string a[], int n)
{
	if (n <= 0)
	{
		return -1;
	}
	if (n == 1)
	{
		return 0;
	}

	int position = locateMax(a + 1, n - 1);
	if (a[0] > a[position + 1])
	{
		return 0;
	}
	return position + 1;

	

}

int countTrue(const string a[], int n)
{
	if (n <= 0)
	{
		return 0;
	}
	int count = countTrue(a + 1, n - 1);
	if (somePredicate(a[0]))
	{
		count++;
	}
	return count;

}

bool contains(const string a1[], int n1, const string a2[], int n2)
{
	if (n2 <= 0)
	{
		return true;
	}
	if (n1 <= 0)
	{
		return false;
	}

	if (a2[0] != a1[0])
	{
		return contains(a1 + 1, n1-1, a2, n2);
	}

	else if (a2[0] == a1[0])
	{
		return contains(a1+1, n1-1, a2+1, n2-1);
	}



}
