// Day 4

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

using namespace std;

set<int> extractNums(string sNums) {
    set<int> nums{};
    stringstream ss;
    string temp;
    int num;

    // Storing the whole string into string stream
    ss << sNums;

    //Running loop till the end of the stream
    while (!ss.eof()) {
        // Extracting number by number from stream
        ss >> num;  // OBS: 
        nums.insert(num);
    }

    return nums;
}

int main() {
    int sum{0};

    // Go through file line by line
    ifstream file{"inputs/day-4-input.txt"};
    string line;
    while (getline(file, line))
    {
        // Strip line of card number prefix
        auto idxCard = line.find(":");
        string sAllNums = line.substr(idxCard + 1, line.size());

        // Seperate winning numbers from other numbers
        auto idxNums = sAllNums.find("|");
        string sWinningNums = sAllNums.substr(0, idxNums);
        string sNums = sAllNums.substr(idxNums + 1, sAllNums.size());

        // Place all numbers in sets
        set<int> winningNums{extractNums(sWinningNums)};
        set<int> nums{extractNums(sNums)};

        // Go through each winning number and check if it
        // exists in the other numbers, increase points for
        // this card
        int points{0};
        for (auto wNum : winningNums) {
            if (nums.find(wNum) != nums.end()) {
                // Found winning number in numbers
                points == 0 ? points = 1 : points *= 2;
            }
        }

        // Add points to total point sum
        sum += points;
    }

    cout << "Result: " << sum << endl;

    return 0;
}
