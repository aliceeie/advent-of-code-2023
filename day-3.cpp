// Day 3

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

    // Go through each char in engine and and sum part numbers
    for (int r{0}; r < engineSchem.size(); r++) {
        set<int> checkedColPoses{};

        for (int c{0}; c < engineSchem.at(r).size(); c++) {

            // If this char is already checked (part of other number), continue
            if (auto search = checkedColPoses.find(c); search != checkedColPoses.end()) {
                // Already checked this char
                continue;
            }

            // Cur character to check
            char curChar{engineSchem.at(r).at(c)};

            // If current char is not digit, continue
            if (!isdigit(curChar)) {
                continue;
            }

            // Locate full number by stepping on the row
            vector<int> colPosFullNumber;
            for (int i{c}; i < engineSchem.at(r).size() && isdigit(engineSchem.at(r).at(i)); i++) {
                colPosFullNumber.push_back(i);
                checkedColPoses.insert(i);
            }

            // Check around full number if symbol exist
            bool symbolExist{false};
            for (int ri{r - 1}; ri <= r + 1; ri++) {
                for (int ci{colPosFullNumber.front() - 1}; ci <= colPosFullNumber.back() + 1; ci++) {
                    // Out of bounds check
                    if (ri < 0 || ri >= engineSchem.size() 
                        || ci < 0 || ci >= engineSchem.at(ri).size()) {
                        // Is out of bounds, continue
                        continue;
                    }

                    // Check if char is symbol
                    char charToCheck{engineSchem.at(ri).at(ci)};
                    if (charToCheck != '.' && !isdigit(charToCheck)) {
                        // Char is symbol
                        symbolExist = true;
                        break;
                    }
                }

                if (symbolExist) {
                    break;
                }
            }

            if (symbolExist) {
                // Convert to number
                string sNumber{""};
                for (int i{colPosFullNumber.front()}; i <= colPosFullNumber.back(); i++) {
                    sNumber += engineSchem.at(r).at(i);
                }

                // Add to sum
                int iNumber = stoi(sNumber);
                sum += iNumber;
            }
        }
    }

    cout << "Result: " << sum << endl;

    return 0;
}
