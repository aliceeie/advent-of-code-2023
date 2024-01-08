// Day 6 part 2

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

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

vector<long long int> extractNumsLong(string sNums) {
    vector<long long int> nums{};
    stringstream ss;
    string temp;
    long long int num;

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
    vector<int> times;
    vector<long long int> distances;

    // Go through file line by line
    ifstream file{"inputs/day-6-input-part-2.txt"};
    string line;
    while (getline(file, line))
    {
        // Read in numbers
        auto idx = line.find(":");
        string numbers = line.substr(idx + 1, line.size());

        if (line.find("Time") != string::npos) {
            // Place in times
            times = extractNums(numbers);
        } else {
            // Place in distances
            distances = extractNumsLong(numbers);
        }
        
    }

    // Go through each race
    int marginOfError{1};
    for (int i{0}; i < times.size(); i++) {
        int time{times[i]};
        long long int distance{distances[i]};

        // Brute-force: Go thorugh each option and calc travelled distance
        int countWins{0};
        for (int t{1}; t < time; t++) {
            int speed{t};
            long long int timeToTravel{time - t};
            long long int travelDistance{speed * timeToTravel};

            if (travelDistance > distance) {
                countWins++;
            }
        }

        // Multiply to margin of errors
        marginOfError *= countWins;
    }

    cout << "Result: " << marginOfError << endl;

    return 0;
}
