// Day 1

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <list>

using namespace std;

int main() {
    // Declare sum
    int sum{0};

    // Go through file line by line
    ifstream file{"inputs/day-1-input.txt"};
    string line;
    while (getline(file, line))
    {
        list<int> digits{};

        // Iterate through each character in the line
        for (const char& c : line) {
            if (isdigit(c)) {
                int digit = c - '0';
                digits.push_back(digit);
            }
        }

        // Sanity check digits list
        if (digits.size() < 1) {
            cerr << "A line has less than one digits, aborting" << endl;
            return 1;
        }

        // Get first and last digit
        int firstDigit{digits.front()};
        int lastDigit{digits.back()};

        // Assemble two-digit number
        int twoDigitNum{stoi(to_string(firstDigit) + to_string(lastDigit))};

        // Add to sum
        sum += twoDigitNum;
    }
    
    cout << "Result: " << sum << endl;

    return 0;
}
