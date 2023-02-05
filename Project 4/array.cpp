// Erika Tran
// Project 4
// Ask Your Boss For Arrays

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// add value to end of n array elements
// returns n or -1 if n is negative
int appendToAll(string a[], int n, string value)
{
	if (n < 0) // return -1 if n is negative
		return -1;

	for (int i = 0; i < n; i++) // iterate up to element n of array
	{
		a[i] += value; // add value to element
	}

	return n;
}

// returns value of target in n elements of array or smallest value if multiple occur
// return -1 if no string matching target
int lookup(const string a[], int n, string target)
{
	for (int i = 0; i < n; i++) // iterate up to element n of array
	{
		if (a[i] == target) // if element is equal to target
			return i; // value where target is located
	}

	return -1; // returns -1 if no matching target
}

// return position of string latest in alphabet for n elements in array or position of smallest value if > 1 string
// return -1  if array has no elements
int positionOfMax(const string a[], int n)
{
	if (n <= 0) // returns -1 if n is negative or zero
		return -1;

	int max = 0; // set max to zero index

	for (int i = 1; i < n; i++) // iterate up to element n of array
	{
		if (a[i] > a[max]) // if string is greater than max string
			max = i; // set max to index of new element
	}

	return max;
}

// string at element pos gets moved to end of array
// return pos
int rotateLeft(string a[], int n, int pos)
{
	if (n < 0 || pos < 0 || pos >= n) // return -1 if n or p is negative or if pos is greater than n
		return -1;

	string temp = a[pos]; // temporary string to hold pos element

	for (int i = pos; i < n - 1; i++) // iterate up to element n of array
	{
		a[i] = a[i+1]; // shifting elements left, set element equal to next element
	}
	a[n - 1] = temp; // set end element to what was shifting

	return pos;
}

// sequence is one or more consecutive identical items
// returns number of sequences in array
int countRuns(const string a[], int n)
{
	if (n < 0) // return -1 if n is negative
		return -1;
	else if (n == 0) // return 0 if n is 0
		return 0;
	
	int repCount = 0; // repCount used to track how many duplicates in array
	
	for (int i = 0; i < n - 1; i++) // iterate up to element n of array
	{
		if (a[i] == a[i + 1]) // if element is equal to next element
			repCount++; // increment repCount
	}
	
	return n - repCount; // num sequences is total elements minus duplicates
}

// reverses order of n elements in array
// returns n
int flip(string a[], int n)
{
	if (n < 0) // return -1 if n is negative
		return -1;

	string temp; // temporary string to hold element

	for (int i = 0; i < n / 2; i++) // iterate through half of array
	{
		temp = a[i]; // set temp equal to the current element
		a[i] = a[n - i - 1]; // put element on opposite side where current element was
		a[n - i - 1] = temp; // set opposite element equal to current element
	}

	return n;
}

// return position of element that a1 and a2 are not equal
int differ(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0 || (n1 == 0 && n2 == 0)) // return -1 if n1 or n2 is negative or n1 and n2 are zero
		return -1;

	int num; // determine which n value to use
	
	// compare number of elements that smallest n value
	// set that value to num
	if (n1 > n2)
		num = n2;
	else
		num = n1;
	
	for (int i = 0; i < num; i++) // iterate up to element n of array
	{
		if (a1[i] != a2[i]) // if element in a1 is different than a2, return that value
			return i;
	}
	
	return num; // return num if all elements up til num are equal
}

// return position in a1 where a2 begins
int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0 || n1 < n2) // return -1 if n1 or n2 are negative or n1 less than n2
		return -1;
	else if ((n1 == 0 && n2 == 0) || n2 == 0) // return 0 if n1 and n2 are zero or n2 is zero
		return 0;

	int counter = 0; // used to count number of matches

	for (int i = 0; i < n1; i++) // iterating through a1
	{
		for (int j = 0; j < n2; j++) // iterating through a2
		{
			if (a1[i] == a2[j])
			{
				counter++;
				if (counter == n2) // if number of same elements equals number of elements in a2
				{
					return i - counter + 1; // return first element that is the same on a1
				}
				i++; // increment i to compare next value
			}
			else
				counter = 0; // if no match set counter to 0
		}
	}

	return -1; // returns -1 if a2 is not subsequence in a1

}

// return first value of a1 that is equal to any a2 value
int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0) // return -1 if n1 or n2 is negative
		return -1;

	for (int i = 0; i < n1; i++) // iterate through a1
	{
		for (int j = 0; j < n2; j++) // iterate through a2
		{
			// compare each a1 element to all a2 elements
			if (a2[j] == a1[i])
				return i;
		}
	}

	return -1; // return -1 if no value equals
}

// rearrange array, elements < splitter come first and elements > splitter come last
// return position of first element after rearrangement that is not < splitter
int split(string a[], int n, string splitter)
{
	if (n < 0) // return -1 if n is negative
		return -1;

	string temp; // to hold temp string
	int element = 0; // track where position of splitter in array

	// push elements > splitter to end of string, rotateLeft function
	for (int i = n - 1; i >= 0; i--) // iterate through array starting at end
	{
		if (a[i] >= splitter) // if current element is greater than splitter
		{
			// rotateLeft function
			temp = a[i]; // set current element to temp 
			for (int j = i; j < n - 1; j++) // start and i value and increment up
			{
				a[j] = a[j + 1]; // set current value to the next value
			}
			a[n - 1] = temp; // set last value as temp
		}
		else
			element++; // otherwise increment element
	}

	// assign splitter at correct index
	for (int j = 0; j < n; j++) // iterate through array
	{
		if (a[j] == splitter) // if current element is equal to splitter
		{
			temp = a[j]; // set temp value as current element
			a[j] = a[element]; // set current element as element of where splitter should be
			a[element] = temp; // set above element to temp
			element++; // increment element
		}
	}

	// returns proper element value
	for (int k = 0; k < n; k++) // iterate through adjusted array
	{
		if (a[k] >= splitter) // if current element is greater than or equal to splitter
			return k; // return index of splitter or one that comes right after
	}

	return n; // otherwise return n
}

int main()
{
	string h[7] = { "rishi", "margaret", "gordon", "tony", "", "john", "liz" };
	assert(lookup(h, 7, "john") == 5);
	assert(lookup(h, 7, "gordon") == 2);
	assert(lookup(h, 2, "gordon") == -1);
	assert(positionOfMax(h, 7) == 3);

	string g[4] = { "rishi", "margaret", "liz", "theresa" };
	assert(differ(h, 4, g, 4) == 2);
	assert(appendToAll(g, 4, "?") == 4 && g[0] == "rishi?" && g[3] == "theresa?");
	assert(rotateLeft(g, 4, 1) == 1 && g[1] == "liz?" && g[3] == "margaret?");

	string e[4] = { "gordon", "tony", "", "john" };
	assert(subsequence(h, 7, e, 4) == 2);

	string d[5] = { "margaret", "margaret", "margaret", "tony", "tony" };
	assert(countRuns(d, 5) == 2);

	string f[3] = { "liz", "gordon", "tony" };
	assert(lookupAny(h, 7, f, 3) == 2);
	assert(flip(f, 3) == 3 && f[0] == "tony" && f[2] == "liz");

	assert(split(h, 7, "liz") == 3);

	cout << "All tests succeeded" << endl;
}