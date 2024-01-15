// Day 9 part 2

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

vector<int> extractNums(string sNums) {
    vector<int> nums{};
    stringstream ss;
    string temp;
    int num;

    // Storing the whole string into string stream
    ss << sNums;

    //Running loop till the end of the stream
    while (!ss.eof()) {
        // Extracting number by number from stream
        ss >> num;  // OBS: 
        nums.push_back(num);
    }

    return nums;
}

int main() {
    int sum{0};

    // Go through file line by line
    ifstream file{"inputs/day-9-input.txt"};
    string line;
    while (getline(file, line))
    {
        // Read in ints to vector
        vector<vector<int>> diffs;     // level -> vector of history values
        diffs.push_back(extractNums(line));

        // Get differences recursively
        bool allZeros{false};
        // Step through each level
        for (int l{0}; !allZeros; l++) {
            // Init vector in ne<t map
            diffs.push_back(vector<int>{});

            // Step through each pair of history values
            allZeros = true;
            for (int i{1}; i < diffs[l].size(); i++) {
                int valA{diffs[l][i-1]};
                int valB{diffs[l][i]};

                // Place diff in map
                int diff{valB - valA};
                diffs[l+1].push_back(diff);

                // Check if zero
                if (diff != 0) {
                    allZeros = false;
                }
            }
        }

        // Extrapolate the history

        // Add a zero to last vector at beginnning
        int lastLevel{diffs.size() - 1};
        auto it = diffs[lastLevel].begin();
        diffs[lastLevel].insert(it, 0);

        // Go upwards in levels
        for (int l{lastLevel - 1}; l >= 0; l--) {
            // Add new value based on diff in level below
            it = diffs[l].begin();
            diffs[l].insert(it, diffs[l].front() - diffs[l + 1].front());
        }
        
        // Add new calculated value to sum
        sum += diffs.front().front();
    }

    cout << "Result: " << sum << endl;

    return 0;
}
