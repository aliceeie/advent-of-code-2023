// Day 7 part 2

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

enum HandType { highCard, onePair, twoPair, threeOfAKind, fullHouse, fourOfAKind, fiveOfAKind };
const map<char,int> cardLabels{{'J', 0}, {'2', 1}, {'3', 2}, {'4', 3}, {'5', 4}, {'6', 5}, {'7', 6}, {'8', 7}, 
                                {'9', 8}, {'T', 9}, {'Q', 10}, {'K', 11}, {'A', 12}};

struct hand
{
    string cards;
    int bid;
    int type;
};

int getMaxCount (map<char,int> mCount) {
    int maxCount{0};
    for (auto count : mCount) {
        int val{count.second};
        if (val > maxCount) {
            maxCount = val;
        }
    }

    return maxCount;
}

char getKeyOfMaxCount (map<char,int> mCount) {
    int maxCount{0};
    char keyMaxCount;
    for (auto count : mCount) {
        char key{count.first};
        int val{count.second};
        if (val > maxCount) {
            maxCount = val;
            keyMaxCount = key;
        }
    }

    return keyMaxCount;
}

HandType getType (const string& cards) {
    set<char> sCount;
    map<char,int> mCount;
    
    // Identify hand type
    // Iterate through each character in the card hand
    for (const char& c : cards) {
        if ( c != 'J') {
            sCount.insert(c);
        }
        ++mCount[c];
    }

    // Put all jokers as something else
    if (mCount.find('J') != mCount.end()) {
        // Jokers exist in hand

        // Save amount of jokers
        int jokerCount{mCount['J']};

        // Remove jokers from mCount
        mCount.erase('J');

        // Deal with cas of card hand 'JJJJJ'
        if (mCount.empty()) {
            sCount.insert('J');
        } else {
            // Get max count of a card label
            char keyOfMaxCount{getKeyOfMaxCount(mCount)};

            // Add joker amount to this key
            mCount[keyOfMaxCount] += jokerCount;
        }
    }

    switch (sCount.size()) {
        case 5:
            return highCard;
        case 4:
            return onePair;
        case 3:
            {
                // Find highest amount of one card
                int maxCount{getMaxCount(mCount)};

                // Determine between two-pair and three-of-a-kind
                if (maxCount == 3) {
                    return threeOfAKind;
                } else {
                    return twoPair;
                }
            }
        case 2:
            {
                // Find highest amount of one card
                int maxCount{getMaxCount(mCount)};

                // Determine between four-of-a-kind and full-house
                if (maxCount == 4) {
                    return fourOfAKind;
                } else {
                    return fullHouse;
                }
            }
        case 1:
            return fiveOfAKind;
        default:
            cout << "Warning: card hand '" << cards << "' does not have a correct type.";
            exit(-1);
    }
}

bool handComp (const hand& a, const hand& b) {
    // First check on hand type
    if (a.type != b.type) {
        return a.type < b.type;
    }

    // If hand types are the same, check strongest card in order
    // Go through chars in cards and compare
    for (int i{0}; i < 5; i++) {
        if (cardLabels.find(a.cards[i]) == cardLabels.end()) {
            cout << "Char not found ni card lables map: " << a.cards[i] << endl;
        } else if (cardLabels.find(b.cards[i]) == cardLabels.end()) {
            cout << "Char not found ni card lables map: " << a.cards[i] << endl;
        }

        int valA = cardLabels.at(a.cards[i]);
        int valB = cardLabels.at(b.cards[i]);

        if (valA != valB) {
            return valA < valB;
        } else {
            continue;
        }
    }

    cout << "Warning: two card hands are identical" << endl;
    return true;
}

int main() {
    int sum{0};
    list<hand> hands;

    // Go through file line by line
    ifstream file{"inputs/day-7-input.txt"};
    string line;
    while (getline(file, line))
    {
        // Create hand from read line
        auto idx = line.find(" ");
        hand h = {.cards = line.substr(0, idx), 
                .bid = stoi(line.substr(idx + 1, line.size())),
                .type = getType(line.substr(0, idx))};

        // Place hand in list
        hands.push_back(h);
    }

    // Sort list
    hands.sort(handComp);

    // Calc winnings
    int rank{1};
    for (auto hand : hands) {
        sum += hand.bid * rank;

        // Increase rank by one
        rank++;
    }

    cout << "Result: " << sum << endl;

    return 0;
}
