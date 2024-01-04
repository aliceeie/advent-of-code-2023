// Day 5

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

vector<long long int> extractNums(string sNums) {
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
    const long long int MAX{__LONG_LONG_MAX__};

    vector<long long int> seeds;
    map<string,int> mapTypes{{"seed-to-soil", 0}, {"soil-to-fertilizer", 1}, 
                        {"fertilizer-to-water", 2}, {"water-to-light", 3}, 
                        {"light-to-temperature", 4}, {"temperature-to-humidity", 5}, 
                        {"humidity-to-location", 6}};
    int curMap{-1};
    map<int,map<long long int,long long int>> diffMaps;  // one entry for each map type containing (src -> dest-to-src-diff)
    map<int,map<long long int,long long int>> lenMaps;  // one entry for each map type containing (src -> len)

    // Go through file line by line and collect data
    ifstream file{"inputs/day-5-input.txt"};
    string line;

    while (getline(file, line))
    {
        // Handle seeds
        if (line.find("seeds") != string::npos) {
            // Take out numbers
            auto idx = line.find(":");
            string numbers = line.substr(idx + 1, line.size());

            // Place numbers in vector
            seeds = extractNums(numbers);

            continue;
        }

        // Handle switch map type
        if (line.find("map") != string::npos) {
            // Take out map type
            auto idx = line.find(" ");
            string type = line.substr(0, idx);

            // Switch current map type
            curMap = mapTypes[type];

            continue;
        }

        // Handel empty line
        if (line == "") {
            continue;
        }

        // Handle map data line
        
        // Read dest, src and length
        vector<long long int> nums = extractNums(line);
        long long int dest{nums.at(0)};
        long long int src{nums.at(1)};
        long long int len{nums.at(2)};

        // Place in correct data map
        (diffMaps[curMap])[src] = dest - src;
        (lenMaps[curMap])[src] = len;
    }

    // Go through and check that all source ranges are covered in maps
    for (int i{0}; i < diffMaps.size(); i++) {
        // Go though in between every source in map
        for (auto lenEntry : lenMaps[i]) {
            long long int src{lenEntry.first};
            long long int len{lenEntry.second};

            if (diffMaps[i].find(src + len) == diffMaps[i].end()) {
                // Missing range in map, add
                (diffMaps[i])[src + len] = 0;
            }
        }

        // Fix first entry as 0 if it does not exist
        if (diffMaps[i].begin()->first != 0) {
            (diffMaps[i])[0] = 0;
        }

        // Fix last entry as MAX
        (diffMaps[i])[MAX] = 0;
    }

    // Calc locations from seeds and get min location id
    long long int minLoc{MAX};
    for (auto seed : seeds) {
        long long int temp{seed};
        // Go through each type of map
        for (int t{0}; t < mapTypes.size(); t++) {
            map<long long int,long long int> diffMap = diffMaps[t];
            long long int prevDiff;

            // Find correct range for temp
            for (auto diffEntry : diffMap) {
                long long int src{diffEntry.first};
                long long int diff{diffEntry.second};

                if (temp < src || src == MAX) {
                    // Use prev diff
                    temp += prevDiff;
                    break;
                } else if (temp == src) {
                    // Use this diff
                    temp += diff;
                    break;
                } else {
                    // Continue stepping through diffs
                    prevDiff = diff;
                    continue;
                }
            }
        }

        // Location is reached
        // Check if this loc is smaller than current min loc
        if (temp < minLoc) {
            minLoc = temp;
        }
    }

    cout << "Result: " << minLoc << endl;

    return 0;
}
