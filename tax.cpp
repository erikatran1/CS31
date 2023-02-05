// Erika Tran
// Project 2
// State Tax

#include <iostream>
#include <string>
using namespace std;

int main()
{
	double pay = 0;
	string name;
	double taxinc;
	string occupation;
	int children;

	cout << "Name: ";
	getline(cin, name);
	if (name == "")
	{
		cout << "---\nYou must enter a name";
		return 1;
	}

	cout << "Taxable income: ";
	cin >> taxinc;
	if (taxinc < 0)
	{
		cout << "---\nThe taxable income must not be negative";
		return 1;
	}
	cin.ignore(10000, '\n');

	cout << "Occupation: ";
	getline(cin, occupation);
	if (occupation == "")
	{
		cout << "---\nYou must enter an occupation";
		return 1;
	}

	cout << "Number of children: ";
	cin >> children;
	if (children < 0)
	{
		cout << "---\nThe number of children must not be negative";
		return 1;
	}

	if (taxinc <= 55000)
		pay += taxinc * .04;
	else if (taxinc <= 125000)
	{
		pay += 55000 * .04;
		if (occupation == "nurse" || occupation == "teacher")
		{
			pay += (taxinc - 55000) * .05;
		}
		else
		{
			pay += (taxinc - 55000) * .07;
		}
	}
	else
	{
		pay += 55000 * .04;
		if (occupation == "nurse" || occupation == "teacher")
		{
			pay += 70000 * .05;
		}
		else
		{
			pay += 70000 * .07;
		}
		pay += (taxinc - 125000) * .093;
	}

	if (taxinc < 125000)
		pay -= 200 * children;

	cout.setf(ios::fixed);
	cout.precision(2);

	if (pay < 0)
		pay = 0;

	cout << "---" << endl;
	cout << name << " would pay $" << pay << endl;
}