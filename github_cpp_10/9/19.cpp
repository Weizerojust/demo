#include <iostream>
#include <string>

#include <stdlib.h>

using namespace std;

int main()
{
	string toReverse;
	string reversedString;

	cout << "Please enter the Phrase you want to reverse\n";
	getline(cin, toReverse);
	
	unsigned strLength = toReverse.length();
	reversedString.resize(strLength);

	int j = 0;

	for (int i = toReverse.length() - 1; i >= 0; i--)
	{
		reversedString[j] = toReverse.at(i);
		j+=1;
	}

	cout << '\n' << "Your input reversed: " << reversedString;

	
	cin.ignore();

	
	do {
		cout << "\n \n" << "Press the Enter key to continue.";
	} while (cin.get() != '\n');

	return 0;
}