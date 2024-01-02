// Day 2 part 2

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    // Declare sum
    int sum{0};

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
        
        // Declare fewest amount for colors
        map<string, int> fewestColorAmount{{"red", 0}, {"green", 0}, {"blue", 0}};

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

                // Update fewest color amount if larger than present
                if (fewestColorAmount[color] < amount) {
                    fewestColorAmount[color] = amount;
                }

                // Update set
                set = set.substr(cubeIdx + 1, set.size());

                // Break while loop if no cubes left to count
                if (cubeIdx == string::npos) {
                    break;
                }
            }

            // Update rightSide
            rightSide = rightSide.substr(setIdx + 1, rightSide.size());

            // Break while loop if no sets left 
            if (setIdx == string::npos) {
                break;
            }
        }

        int gamePower = fewestColorAmount["red"] * fewestColorAmount["green"] * fewestColorAmount["blue"];
        sum += gamePower;
    }
    
    cout << "Result: " << sum << endl;

    return 0;
}
