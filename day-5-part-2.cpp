// Day 5 part 2

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const long long int MAX{__LONG_LONG_MAX__};

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

long long int getSeedSrcInMap (map<long long int,long long int> diffMap, long long int seed) {
    long long int prevSrc;

    for (auto diffEntry : diffMap) {
        long long int src{diffEntry.first};

        if (seed < src || src == MAX) {
            // Use prev src
            return prevSrc;
        } else if (seed == src) {
            // Use this src
            return src;
        } else {
            // Continue stepping through diffs
            prevSrc = src;
            continue;
        }
    }

    return 0;
}

long long int stepSeedWithMap (map<long long int,long long int> diffMap, long long int seed) {
    long long int prevDiff;
    long long int temp{seed};
    // Find correct range for temp
    for (auto diffEntry : diffMap) {
        long long int src{diffEntry.first};
        long long int diff{diffEntry.second};

        if (temp < src || src == MAX) {
            // Use prev diff
            temp += prevDiff;
            return temp;
        } else if (temp == src) {
            // Use this diff
            temp += diff;
            return temp;
        } else {
            // Continue stepping through diffs
            prevDiff = diff;
            continue;
        }
    }
}

int main() {

    vector<long long int> seedPairs;
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
            seedPairs = extractNums(numbers);

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

    // Calc inital seed ranges
    vector<long long int> seedRanges; // Contains pairs of start and end of ranges
    long long int start;
    for (int i{0}; i < seedPairs.size(); i++) {
        if (i % 2 == 0) {
            // Seed range start
            start = seedPairs[i];
        } else {
            long long int len{seedPairs[i]};
            long long int end{start + len - 1};
            seedRanges.push_back(start);
            seedRanges.push_back(end);
        }
    }

    // Go through each type of map
    for (int t{0}; t < mapTypes.size(); t++) {
        map<long long int,long long int> diffMap = diffMaps[t];

        // Find seeds to use in this round to represent all seed ranges
        long long int start;
        vector<long long int> copySeedRanges{seedRanges};
        for (int i{0}; i < seedRanges.size(); i++) {
            if (i % 2 == 0) {
                // Start of range
                start = seedRanges[i];
                continue;
            } else {
                // End of range

                // Check if whole range fits in one diff range
                // Find place of start and end in diff map,
                // if they are the same, keep them as range,
                // if not the same, split up into appropriate ranges

                long long int end{seedRanges[i]};

                long long int mapSrcStart{getSeedSrcInMap(diffMap, start)};
                long long int mapSrcEnd{getSeedSrcInMap(diffMap, end)};

                if (mapSrcStart == mapSrcEnd) {
                    // Whole range fits in one diff range
                    continue;
                } else {
                    // Find place of starts and ends in diff map
                    // (split a range up in multiple ranges to accommodate for diff map ranges)
                    long long int newStart{start};
                    long long int newEnd{end};
                    long long int prevEnd;

                    while (true) {
                        if (getSeedSrcInMap(diffMap, newStart) == getSeedSrcInMap(diffMap, newEnd)) {
                            // Last range part is complete, put in the last new start range in vector

                            // Find index of new range end in vector
                            auto itOfEnd = find(copySeedRanges.begin(), copySeedRanges.end(), newEnd);

                            // Place new range start before range end in vector
                            copySeedRanges.insert(itOfEnd, newStart);
                            break;
                        }

                        // Find new range end for this seed range part
                        long long int mapSrcNewStart{getSeedSrcInMap(diffMap, newStart)};   // Get map src for new start
                        auto itNewEnd = diffMap.find(mapSrcNewStart);                       // Get iterator for this src
                        itNewEnd++;                                                         // Increase by one to point to next src in map
                        newEnd = itNewEnd->first;                                           // Set new end as this src
                        newEnd--;                                                           // Decrease this src by one to get the new range end

                        // Find index of range start in vector
                        auto itOfStart = find(copySeedRanges.begin(), copySeedRanges.end(), newStart);

                        // Handle if new start range value does not exist in vector
                        if (itOfStart == copySeedRanges.end()) {
                            // Find index of prev range end in vector
                            auto itOfPrevEnd = find(copySeedRanges.begin(), copySeedRanges.end(), prevEnd);

                            // Place new range start after prev range end in vector
                            copySeedRanges.insert(itOfPrevEnd + 1, newStart);

                            itOfStart = find(copySeedRanges.begin(), copySeedRanges.end(), newStart);
                        }

                        // Place new range end after range start in vector
                        copySeedRanges.insert(itOfStart + 1, newEnd);

                        // Update range start and ends
                        prevEnd = newEnd;
                        newStart = newEnd + 1;
                        newEnd = end;
                    }
                }
            }
        }

        // Update seed ranges with newly added ranges
        seedRanges = copySeedRanges;

        // Get new seed ranges using current map
        // (use start and end of seed ranges as seeds to re-calc with map)
        vector<long long int> newSeedRanges;
        for (auto seed : seedRanges) {
            //Re-calc new seed
            long long int newSeed{stepSeedWithMap(diffMap, seed)};

            // Put new seed in temp vector
            newSeedRanges.push_back(newSeed);
        }

        // Replace seed range vector with new seed range vector
        seedRanges = newSeedRanges;
    }

    // Seed ranges are now equivalent to ranges of possible locations
    // Find lowest location number
    long long int minLoc{MAX};

    for (auto loc : seedRanges) {
        if (loc < minLoc) {
            minLoc = loc;
        }
    }

    cout << "Result: " << minLoc << endl;

    return 0;
}
