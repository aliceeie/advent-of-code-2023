// Day 8 part 2

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

int gcd(int aIn, int bIn){
    int a{aIn};
    int b{bIn};

    // Make sure a is larger than b
    if (b > a) {
        a = bIn;
        b = aIn;
    }

    // Check if reched end
    if (b == 0) {
        // a is greatest common divisor
        return a;
    } else {
        // Recursively call
        return gcd(b, a % b);
    }
}

struct lRPair {
    string left;
    string right;
    bool endsWithA;
    bool endsWithZ;
};

int main() {
    int steps{0};
    string lRSecquence{""};
    map<string,lRPair> network;
    vector<string>curNodes;

    // Read in map of netword and L/R sequence
    // Go through file line by line
    ifstream file{"inputs/day-8-input.txt"};
    string line;
    while (getline(file, line))
    {
        // Skip and continue if empty line
        if (line == "") {
            continue;
        }

        // Handle LR sequence
        if (line.find("=") == string::npos) {
            lRSecquence = line;
            continue;
        }

        // Handle netword node
        auto idx = line.find("=");
        string node = line.substr(0, idx - 1);

        // Separate L/R for node
        string sLeftRightPair = line.substr(idx + 1, line.size());
        string left = sLeftRightPair.substr(2, 3);
        string right = sLeftRightPair.substr(7, 3);
        bool endsWithA = node.back() == 'A';
        bool endsWithZ = node.back() == 'Z';

        // Add to network map
        network[node] = lRPair{.left = left, .right = right,
                                .endsWithA = endsWithA, .endsWithZ = endsWithZ};

        // Add node to start vector if it ends with A
        if (endsWithA) {
            curNodes.push_back(node);
        }
    }

    // Step through the network until ZZZ is reached
    int curSeqPos{0};
    bool reachedEnd{false};
    map<int,int> stepsCycles;
    vector<int> lastSteps{0, 0, 0, 0, 0, 0};
    while (!reachedEnd) {
        // Get next direction
        bool goLeft = lRSecquence.at(curSeqPos) == 'L';

        // Go to next node
        if (goLeft) {
            for (auto& node : curNodes) {
                node = network[node].left;
            }
        } else {
            for (auto& node : curNodes) {
                node = network[node].right;
            }
        }

        // Increase steps and pos
        steps++;
        curSeqPos++;

        // Check bounds of sequence
        if (curSeqPos >= lRSecquence.size()) {
            curSeqPos = 0;
        }

        // Check if reached end
        int idx{0};
        for (auto& node : curNodes) {
            if (network[node].endsWithZ)  {
                // Update last steps
                int newCycle{steps - lastSteps[idx]};

                // Add to map if it does not exist for this node
                if (stepsCycles.find(idx) == stepsCycles.end()) {
                    stepsCycles[idx] = newCycle;
                }

                // Update last steps
                lastSteps[idx] = steps;
            }
            idx++;
        }

        if (stepsCycles.size() == 6) {
            reachedEnd = true;
        }
    }

    cout << "Step cycles: ";
    for (auto s : stepsCycles) {
        cout << s.second << " ";
    }
    cout << endl;

    // Now we have all step cycles for which 
    // each start node reaches its end node
    
    // Find greatest common divisor and product of steps
    int lastGcd{0};
    for (auto s : stepsCycles) {
        steps = s.second;

        // Gcd stuff
        if (lastGcd == 0) {
            // First step
            lastGcd = steps;
            continue;
        } else {
            // Calc gcd with current steps and last gcd
            lastGcd = gcd(lastGcd, steps);
        }
    }

    cout << "GCD is: " << lastGcd << endl;

    long long int stepsProduct1{1};
    long long int stepsProduct2{1};
    for (auto s : stepsCycles) {
        steps = s.second;
        if (stepsProduct1 > INT64_MAX / steps) {
            // Will overflow long long int
            cout << "Overflow 1" << endl;
            if (stepsProduct2 > INT64_MAX / steps) {
                cout << "Overflow 2" << endl;
                exit(1);
            } else {
                stepsProduct2 *= steps;
            }
        } else {
            stepsProduct1 *= steps;
        }
    }

    // Find least common multiple
    long long int lcm{(stepsProduct1 / lastGcd) * stepsProduct2};

    cout << "Result: " << lcm << endl;

    return 0;
}
