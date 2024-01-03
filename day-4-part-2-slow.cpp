// Day 4 part 2

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <numeric>
#include <map>
#include <queue>

using namespace std;

set<int> extractNums(string sNums) {
    set<int> nums{};
    stringstream ss;
    string temp;
    int num;

    // Storing the whole string into string stream
    ss << sNums;

    //Running loop till the end of the stream
    while (!ss.eof()) {
        // Extracting number by number from stream
        ss >> num;  // OBS: 
        nums.insert(num);
    }

    return nums;
}

int getCardId(string card) {
    // Split up card
    auto idxCard = card.find(":");

    // Identify card id
    string cardPrefix = card.substr(0, idxCard);
    auto idxId = cardPrefix.find(" ");
    int cardId{stoi(cardPrefix.substr(idxId + 1, cardPrefix.size()))};
    
    return cardId;
}

vector<int> processCard(string card) {
    // Get card id
    int cardId{getCardId(card)};

    // Strip line of card number prefix
    auto idxCard = card.find(":");
    string sAllNums = card.substr(idxCard + 1, card.size());

    // Seperate winning numbers from other numbers
    auto idxNums = sAllNums.find("|");
    string sWinningNums = sAllNums.substr(0, idxNums);
    string sNums = sAllNums.substr(idxNums + 1, sAllNums.size());

    // Place all numbers in sets
    set<int> winningNums{extractNums(sWinningNums)};
    set<int> nums{extractNums(sNums)};

    // Go through each winning number and check if it
    // exists in the other numbers, increase points for
    // this card
    int countWins{0};
    for (auto wNum : winningNums) {
        if (nums.find(wNum) != nums.end()) {
            // Found winning number in numbers
            countWins++;
        }
    }

    // Get vector of card copies that was won
    vector<int> cardCopyIds(countWins);
    iota(cardCopyIds.begin(), cardCopyIds.end(), cardId + 1);

    return cardCopyIds;
}

int main() {
    int sum{0};
    map<int,string> cards;

    // Go through file line by line and create "dictionary" of cards
    ifstream file{"inputs/day-4-input.txt"};
    string line;
    while (getline(file, line))
    {
       cards[getCardId(line)] = line;
    }

    // Start with one of each card in queue
    queue<int> cardsToProcess;
    for (int i{1}; i <= cards.size(); i++) {
        cardsToProcess.push(i);
    }

    // Process cards until no more left
    while (!cardsToProcess.empty()) {
        // Process card
        vector<int> newIds = processCard(cards[cardsToProcess.front()]);

        // Add new card copy ids to queue
        for (auto id : newIds) {
            cardsToProcess.push(id);
        }

        // Remove processed card id from queue
        cardsToProcess.pop();

        // Increase sum of processed cards
        sum++;

        if (sum % 100000 == 0) {
            cout << "Sum: " << sum << endl;
        }
    }

    cout << "Result: " << sum << endl;

    return 0;
}
