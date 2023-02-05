// Erika Tran
// Project 6
// What's The Point?

#include <iostream>
using namespace std;

 Example 1
int main()
{
    cout << "30\n20\n10" << endl;
}


 Problem A
int main()
{
    int arr[3] = { 5, 10, 15 };
    int* ptr = arr;

    *ptr = 10;          // set arr[0] to 10
    *(++ptr) = 20;      // set arr[1] to 20
    ptr += 2;
    ptr[-1] = 30;        // set arr[2] to 30

    while (ptr > arr)
    {
        ptr--;
        cout << *ptr << endl;    // print values
    }
}

int main()
{
    int arr[3] = { 5, 10, 15 };
    int* ptr = arr;

    *ptr = 30;          // set arr[0] to 30
    *(ptr + 1) = 20;      // set arr[1] to 20
    ptr += 2;
    ptr[0] = 10;        // set arr[2] to 10

    ptr = arr;
    while (ptr <= (arr+2))
    {
        cout << *ptr << endl;    // print values
        ptr++;
    }
}

 Problem B
void findMax(int arr[], int n, int*& pToMax) 
{
    if (n <= 0)
        return;      // no items, no maximum!

    pToMax = arr;

    for (int i = 1; i < n; i++)
    {
        if (arr[i] > *pToMax)
            pToMax = arr + i;

    }
}

int main()
{
    int nums[4] = { 5, 3, 15, 6 };
    int* ptr = &nums[0];

    findMax(nums, 4, ptr);
    cout << "The maximum is at address " << ptr << endl;
    cout << "It's at position " << ptr - nums << endl;
    cout << "Its value is " << *ptr << endl;
}

 Problem C
void computeCube(int n, int* ncubed)
{
    *ncubed = n * n * n;
}

int main()
{
    int* ptr; // need to initialize pointer
    ptr = new int();
    computeCube(5, ptr);
    cout << "Five cubed is " << *ptr << endl;
}

 Problem D
 return true if two C strings are equal
bool strequal(char str1[], char str2[])
{
    char* s1 = str1;
    char* s2 = str2;
    while (*s1 != 0 && *s2 != 0)  // zero bytes at ends
    {
        if (*s1 != *s2)  // compare corresponding characters
            return false;
        s1++;            // advance to the next character
        s2++;
    }
    return true;   // both ended at same time?
}

int main()
{
    char a[15] = "Chang";
    char b[15] = "Zhang";

    if (strequal(a, b))
        cout << "They're the same person!\n";
}

bool strequal(const char str1[], const char str2[])
{
    while (*str1 != 0 && *str2 != 0)  // zero bytes at ends
    {
        if (*str1 != *str2)  // compare corresponding characters
            return false;
        str1++;            // advance to the next character
        str2++;
    }
    return *str1 == *str2;   // both ended at same time?
}

int main()
{
    char a[15] = "Chang";
    char b[15] = "Zhang";

    if (strequal(a, b))
        cout << "They're the same person!\n";
}

 Problem E
int* nochange(int* p)
{
    return p;
}

int* getPtrToArray(int& m)
{
    int anArray[100];
    for (int j = 0; j < 100; j++)
        anArray[j] = 100 - j;
    return nochange(anArray);
}

void f()
{
    int junk[100];
    for (int k = 0; k < 100; k++)
        junk[k] = 123400000 + k;
    junk[50]++;
}

int main()
{
    int n=100;
    int* ptr = getPtrToArray(n);
    f();
    for (int i = 0; i < 3; i++)
        cout << ptr[i] << ' ';
    for (int i = n - 3; i < n; i++)
        cout << ptr[i] << ' ';
    cout << endl;
}

void removeS(char* message)
{
    char* nMessage = message;
    for (; *nMessage != '\0'; nMessage++)
    {
        if (*nMessage == 'S' || *nMessage == 's')
        {
            for (; *nMessage != '\0'; nMessage++)
            {
                *nMessage = *(nMessage + 1);
            }
            nMessage = message;
        }
    }
}

int main()
{
    char msg[50] = "She'll shave a massless princess.";
    removeS(msg);
    cout << msg;  // prints   he'll have a male prince.
}