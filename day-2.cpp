// Day 2

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    // Declare sum
    int sum{0};
    int reds{12};
    int greens{13};
    int blues{14};

    // Go through file line by line
    ifstream file{"inputs/day-2-input.txt"};
    string line;
    while (getline(file, line))
    {
        // Split up line
        auto index = line.find(":");
        
        // Abort if invalid input
        if (index == string::npos) {
            cerr << "Invalid input: " << line << endl;
            return 1;
        }
        
        // Determine game ID
        int gameId = stoi(line.substr(0, index).erase(0, 4));
        bool impossibleGame = false;

        string rightSide = line.substr(index + 1, line.size());
        
        // Step through game sets
        while (true)
        {
            auto setIdx = rightSide.find(";");
            string set;
            if (setIdx == string::npos) {
                // Last set
                set = rightSide;
            } else {
                set = rightSide.substr(0, setIdx);
            }

            // Keep track of color amounts
            map<string, int> colorCount{{"red", 0}, {"green", 0}, {"blue", 0}};

            // Step through set
            while (true) {
                auto cubeIdx = set.find(",");

                string cubes;
                if (cubeIdx == string::npos) {
                    // Last cube color
                    cubes = set;
                } else {   
                    cubes = set.substr(0,cubeIdx);
                }

                // Remove first space
                cubes.erase(0, 1);

                // Split color from amount
                auto colorIdx = cubes.find(' ');
                int amount = stoi(cubes.substr(0, colorIdx));
                string color = cubes.substr(colorIdx + 1, cubes.size());

                // Update color count
                //colorCount.insert(pair<string, int>(color, amount));
                colorCount[color] = amount;

                // Update set
                set = set.substr(cubeIdx + 1, set.size());

                // Break while loop if no cubes left to count
                if (cubeIdx == string::npos) {
                    break;
                }
            }

            // Check if set is impossible
            if (colorCount.at("red") > reds 
                || colorCount.at("green") > greens 
                || colorCount.at("blue") > blues) {
                // Set is impossible -> game is impossible
                impossibleGame = true;
                break;
            }

            // Update rightSide
            rightSide = rightSide.substr(setIdx + 1, rightSide.size());

            // Break while loop if no sets left 
            if (setIdx == string::npos) {
                break;
            }
        }

        if (impossibleGame) {
            // Skip to next game
            continue;
        } else {
            // If possible, add game ID to sum
            sum += gameId;
        }
    }
    
    cout << "Result: " << sum << endl;

    return 0;
}
