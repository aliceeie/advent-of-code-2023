// Day 1

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <list>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

bool potentiallyValidDigit(const map<string, int>& wordToDigits, const string& digitInLetters) {
    string validDigitAsWord;

    for (const auto& kv : wordToDigits) {
        validDigitAsWord = kv.first;
        // Check if word is starting substring of any of the valid digit words
        if (validDigitAsWord.find(digitInLetters) != string::npos
            && validDigitAsWord.substr(0, digitInLetters.length()) == digitInLetters) {
            // Is potentially a valid digit in letters
            return true;
        }
    }

    return false;
}

void stripDownDigitWord(const map<string, int>& wordToDigits, string& digitInLetters) {
    while (digitInLetters != ("")) {
        // Remove first char in the string
        digitInLetters = digitInLetters.substr(1, digitInLetters.length() - 1);

        // Check if potential digit
        if (potentiallyValidDigit(wordToDigits, digitInLetters)) {
            // Digit is potential digit in letter form
            break;
        }
    }
}

int main() {
    // Declare sum
    int sum{0};

    // Declare dict for word digits to digits
    map<string, int> wordToDigits{{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};

    // Set of valid letters to minimize work
    set<char> validChars{'o', 'n', 'e', 't', 'w', 'h', 'r', 'f', 'u', 'i', 'v', 's', 'x', 'g'};

    // Go through file line by line
    ifstream file{"inputs/day-1-input.txt"};
    string line;
    while (getline(file, line))
    {
        list<int> digits{};
        string digitInLetters{""};

        // Iterate through each character in the line
        for (const char& c : line) {
            if (isdigit(c)) {
                int digit = c - '0';
                digits.push_back(digit);

                // Reset digit in letters
                digitInLetters = "";
                continue;
            } else if (validChars.find(c) == validChars.end()) {
                // Char is not valid char
                // Reset digit in letters
                digitInLetters = "";
                continue;
            } else {
                // Start building a word that is 
                //possibly a digit spelled out
                digitInLetters = digitInLetters + c;

                // Check if word is valid digit
                if (wordToDigits.find(digitInLetters) != wordToDigits.end()) {
                    // Valid digit in letters
                    digits.push_back(wordToDigits.at(digitInLetters));

                    // Deal with stripping down digit in letters
                    stripDownDigitWord(wordToDigits, digitInLetters);
                    continue;
                }

                // Check if word is potentially a valid digit
                if (!potentiallyValidDigit(wordToDigits, digitInLetters)) {
                    // Word is not potentially valid digit

                    // Deal with stripping down digit in letters
                    stripDownDigitWord(wordToDigits, digitInLetters);
                }
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
