// Erika Tran
// Project 3
// Poll Watcher

#include <iostream>
#include <cassert>
#include <string>
using namespace std;

// Check if state is valid code
bool isValidUppercaseStateCode(string stateCode)
{
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.FL.GA.HI.ID.IL.IN.IA.KS.KY."
		"LA.ME.MA.MD.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC.ND."
		"OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}

// Check if input is valid data poll
bool isValidPollString(string pollData)
{
	if (pollData[0] == ',' || pollData[pollData.size()] == ',') // cannot start or end with ','
	{
		return false;
	}
	for (int i = 0; i != pollData.size(); i++)
	{
		if (pollData[i] == ' ') // cannot have any spaces
		{
			return false;
		}
		else if (isalpha(pollData[i]) && isalpha(pollData[i + 1]))
		{
			if (!isValidUppercaseStateCode(pollData.substr(i, 2))) // check if valid state code
			{
				return false;
			}
			i++;
		}
	}
	return true;
}

// Counts the number of seats for given party
int countSeats(string pollData, char party, int& seatCount)
{
	if (!isValidPollString(pollData)) // if poll data is a valid poll string
	{
		return 1;
	}
	else if (!isalpha(party)) // if party is not a letter
	{
		return 2;
	}

	int countNum = 0;
	int seats = 0;
	string tempNum = "";
	for (int i = 0; i != pollData.size(); i++)
	{
		if (!isalpha(pollData[i])) // if character is a number
		{
			tempNum += pollData[i]; // add to tempNum
		}
		else if (tolower(pollData[i]) != party) // if letter character is not the right party
		{
			tempNum = ""; // empty tempNum
		}
		else if (tolower(pollData[i]) == party) // if letter is correct party
		{
			countNum = stoi(tempNum); // convert tempNum to integer
			seats += countNum; // add value to seats
		}
	}
	seatCount = seats; // set seatCount to seats

	return 0;
}

// Does not output anything
int main()
{
	// party variables
	int democraticCount;
	char democratic = 'd';
	int republicanCount;
	char republican = 'r';
	int independentCount;
	char independent = 'i';
	int libertarianCount;
	char libertarian = 'l';
	int greenCount;
	char green = 'g';
	int otherCount;
	char other = 'o';

	cout << "Enter poll data string: ";
	string pds;
	getline(cin, pds); // input data string

	// calling countSeats for each party
	countSeats(pds, democratic, democraticCount);
	countSeats(pds, democratic, republicanCount);
	countSeats(pds, democratic, independentCount);
	countSeats(pds, democratic, libertarianCount);
	countSeats(pds, democratic, greenCount);
	countSeats(pds, democratic, otherCount);
}