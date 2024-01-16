// Day 10

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <map>

using namespace std;

struct direction {
    int id;
    int x;
    int y;
};

bool possibleNextTile(direction dir, char tile) {
    switch (dir.id)
    {
    case 0:
        {
            // Looking north
            string validTiles{"|7FS"};
            if (validTiles.find(tile) != string::npos) {
                return true;
            } else {
                return false;
            }
        }
    case 1:
        {
            // Looking east
            string validTiles{"-J7S"};
            if (validTiles.find(tile) != string::npos) {
                return true;
            } else {
                return false;
            }
        }
    case 2:
        {
            // Looking south{
            string validTiles{"|JLS"};
            if (validTiles.find(tile) != string::npos) {
                return true;
            } else {
                return false;
            }
        }
    case 3:
        {
            // Looking west
            string validTiles{"-LFS"};
            if (validTiles.find(tile) != string::npos) {
                return true;
            } else {
                return false;
            }
        }
    default:
        return false;
    }
}

bool possibleDir(direction dir, char tile) {
    switch (dir.id)
    {
    case 0:
        {
            // North
            string validTiles{"|LJS"};
            if (validTiles.find(tile) != string::npos) {
                return true;
            } else {
                return false;
            }
        }
    case 1:
        {
            // East
            string validTiles{"-LFS"};
            if (validTiles.find(tile) != string::npos) {
                return true;
            } else {
                return false;
            }
        }
    case 2:
        {
            // South{
            string validTiles{"|7FS"};
            if (validTiles.find(tile) != string::npos) {
                return true;
            } else {
                return false;
            }
        }
    case 3:
        {
            // West
            string validTiles{"-J7S"};
            if (validTiles.find(tile) != string::npos) {
                return true;
            } else {
                return false;
            }
        }
    default:
        return false;
    }
}

int main() {
    direction north{0,-1,0}, east{1,0,1}, south{2,1,0}, west{3,0,-1};
    vector<direction> directions{north, east, south, west};

    vector<vector<char>> sketch;
    tuple<int,int> startPos;

    // Read in sketch to grid
    // Go through file line by line
    ifstream file{"inputs/day-10-small-input.txt"};
    string line;
    while (getline(file, line))
    {
        sketch.push_back(vector<char>{});
        copy(line.begin(), line.end(), back_inserter(sketch.back()));

        // Check if S is on this line
        if (auto it = line.find('S'); it != string::npos) {
            // Found start, save for later
            startPos = {sketch.size() - 1, it};
        }
    }

    // Start at start and find a way back to start tile
    vector<tuple<int,int>> path{startPos};
    tuple<int,int> curPos{startPos};
    tuple<int,int> prevPos;
    int x, y;
    char curTile;
    bool backAtStart{false};

    while (!backAtStart) {
        // Look around to find next tile
        for (direction dir : directions) {
            // Positions for tile to check
            x = get<0>(curPos) + dir.x;
            y = get<1>(curPos) + dir.y;
            curTile = sketch[get<0>(curPos)][get<1>(curPos)];

            // Check if this tile has opening to the dir
            if (!possibleDir(dir, curTile)) {
                // Not possible to go in this dir, continue
                continue;
            }

            // Check bounds
            if (x < 0 || x >= sketch.size() ||
                y < 0 || y >= sketch.front().size()) {
                // Out of bounds, continue
                continue;
            }

            // Check if possible tile to go to
            if (tuple<int,int>{x,y} != prevPos
                && possibleNextTile(dir, sketch[x][y])) {
                // Possible tile, stop looking, go there
                prevPos = {curPos};
                curPos = {x, y};
                path.push_back(curPos);
                break;
            }
        }

        // Check if back at start
        if (sketch[get<0>(curPos)][get<1>(curPos)] == 'S') {
            backAtStart = true;
        }
    }

    // Get furthest away distance
    int distance{path.size() / 2};

    cout << "Result: " << distance << endl;

    return 0;
}
