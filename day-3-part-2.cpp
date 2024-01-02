// Day 3 part 2

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <set>

using namespace std;

int main() {
    // Go through file line by line
    ifstream file{"inputs/day-3-input.txt"};
    string line;

    // Define engine schematic variable
    vector<vector<char>> engineSchem;
    int sum{0};

    // Read in file contents to 2D vector
    while (getline(file, line))
    {
        vector<char> row;

        // Iterate through each character in the line
        for (const char& c : line) {
            row.push_back(c);
        }

        // Add new vector to represent new row
        engineSchem.push_back(row);
    }

    // Go through each char in engine and deal with '*' symbols
    for (int r{0}; r < engineSchem.size(); r++) {

        for (int c{0}; c < engineSchem.at(r).size(); c++) {

            // Cur character to check
            char curChar{engineSchem.at(r).at(c)};

            // If current char is not a gear (*), continue
            if (curChar != '*') {
                continue;
            }

            // Count numbers around gear (*)
            vector<int> numbers{};
            for (int ri{r - 1}; ri <= r + 1; ri++) {
                set<int> checkedColPoses{};
                for (int ci{c - 1}; ci <= c + 1; ci++) {
                    // Out of bounds check
                    if (ri < 0 || ri >= engineSchem.size() 
                        || ci < 0 || ci >= engineSchem.at(ri).size()) {
                        // Is out of bounds, continue
                        continue;
                    }

                    // Check if char is digit and has not been checked already
                    char charToCheck{engineSchem.at(ri).at(ci)};
                    if (isdigit(charToCheck) 
                        && checkedColPoses.find(charToCheck) == checkedColPoses.end()) {
                        // Char is digit and part of number
                        // Identify full number
                        string sFullNumber{""};

                        // Go to the right until non-digit
                        for (int i{ci}; i < engineSchem.at(ri).size() && isdigit(engineSchem.at(ri).at(i)); i++) {
                            sFullNumber = sFullNumber + engineSchem.at(ri).at(i);
                            checkedColPoses.insert(i);
                        }

                        // Go to the left until non-digit
                        for (int i{ci - 1}; i >= 0 && isdigit(engineSchem.at(ri).at(i)); i--) {
                            sFullNumber = engineSchem.at(ri).at(i) + sFullNumber;
                            checkedColPoses.insert(i);
                        }

                        // Convert char number to int number
                        numbers.push_back(stoi(sFullNumber));
                    }
                }
            }

            if (numbers.size() == 2) {
                // Exactly two numbers adjecent
                int gearRatio = numbers.at(0) * numbers.at(1);

                // Add to sum
                sum += gearRatio;
            }
        }
    }

    cout << "Result: " << sum << endl;

    return 0;
}
