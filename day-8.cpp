// Day 8

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

struct lRPair {
    string left;
    string right;
};

int main() {
    int steps{0};
    string lRSecquence{""};
    map<string,lRPair> network;

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

        // Add to network map
        network[node] = lRPair{.left = left, .right = right};
    }

    // Step through the network until ZZZ is reached
    string curNode{"AAA"};
    int curSeqPos{0};
    while (curNode != "ZZZ") {
        // Get next direction
        bool goLeft = lRSecquence.at(curSeqPos) == 'L';

        // Go to next node
        if (goLeft) {
            curNode = network[curNode].left;
        } else {
            curNode = network[curNode].right;
        }

        // Increase steps and pos
        steps++;
        curSeqPos++;

        // Check bounds of sequence
        if (curSeqPos >= lRSecquence.size()) {
            curSeqPos = 0;
        }
    }

    cout << "Result: " << steps << endl;

    return 0;
}
