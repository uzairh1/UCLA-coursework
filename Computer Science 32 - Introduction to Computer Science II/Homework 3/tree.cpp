void split(string a[], int n, string splitter,
    int& firstNotLess, int& firstGreater)
{
    if (n < 0)
        n = 0;

    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotLess <= firstUnknown and firstUnknown <= firstGreater
    //  Every element earlier than position firstNotLess is < splitter
    //  Every element from position firstNotLess to firstUnknown-1 is
    //    == splitter
    //  Every element from firstUnknown to firstGreater-1 is not known yet
    //  Every element at position firstGreater or later is > splitter

    firstNotLess = 0;
    firstGreater = n;
    int firstUnknown = 0;
    while (firstUnknown < firstGreater)
    {
        if (a[firstUnknown] > splitter)
        {
            firstGreater--;
            exchange(a[firstUnknown], a[firstGreater]);
        }
        else
        {
            if (a[firstUnknown] < splitter)
            {
                exchange(a[firstNotLess], a[firstUnknown]);
                firstNotLess++;
            }
            firstUnknown++;
        }
    }
}

// Exchange two strings
void exchange(string& x, string& y)
{
    string t = x;
    x = y;
    y = t;
}

// Return the number of ways that all n2 elements of a2 appear in
      // the n1 element array a1 in the same order (though not necessarily
      // consecutively).  We decree that the empty sequence (i.e., one
      // where n2 is 0) appears in a sequence of length n1 in 1 way, even
      // if n1 is 0.  For example, if a1 is the 7 element array
      //    "donald" "keir" "claudia" "jinping" "keir" "claudia" "narendra"
      // then for this value of a2            the function must return
      //    "donald" "jinping" "claudia"                 1
      //    "donald" "claudia" "narendra"                2
      //    "jinping" "donald" "claudia"                 0
      //    "keir" "claudia" "narendra"                  3
int countContains(const string a1[], int n1, const string a2[], int n2);



// Rearrange the elements of the array so that
// a[0] <= a[1] <= a[2] <= ... <= a[n-2] <= a[n-1]
// If n <= 1, do nothing.
void order(string a[], int n)
{
    if (n <= 1)
        return;

    int firstNotLess = 0;
    int firstGreater = 0;

    string pivot = a[n / 2];

    split(a, n, pivot, firstNotLess, firstGreater);

    order(a, firstNotLess);
    order(a + firstGreater, n - firstGreater);
}


int countContains(const string a1[], int n1, const string a2[], int n2)
{
    if (n2 == 0)
    {
        return 1;
    }
    if (n1 == 0)
    {
        return 0;
    }

    if (a1[0] == a2[0])
    {
        return countContains(a1 + 1, n1 - 1, a2 + 1, n2 - 1) + countContains(a1 + 1, n1 - 1, a2, n2);
    }

    else
    {
        return countContains(a1 + 1, n1 - 1, a2, n2);
    }
}