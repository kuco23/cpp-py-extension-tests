//
// Created by kuco on 04/08/2019.
//

#include <map>
#include <vector>
using namespace std;

#ifndef EXTENSIONS_HANDPARSER_H
#define EXTENSIONS_HANDPARSER_H

enum Hand {HIGHCARD, ONEPAIR, TWOPAIR, THREEOFAKIND, STRAIGHT, FLUSH, FULLHOUSE, FOUROFAKIND, STRAIGHTFLUSH};
std::map<Hand, int> HAND_LEN = {
        {Hand::HIGHCARD, 1}, {Hand::ONEPAIR, 2},
        {Hand::TWOPAIR, 4}, {Hand::THREEOFAKIND, 3},
        {Hand::STRAIGHT, 5}, {Hand::FLUSH, 5},
        {Hand::FULLHOUSE, 5}, {Hand::FOUROFAKIND, 4},
        {Hand::STRAIGHTFLUSH, 5}
};
static int SUM(int* const& ar, size_t len);

class HandParser {
private:
    int valnums[13], suitnums[4],
    straightindexes[5], flushsuit;

public:
    Hand handenum;
    vector<vector<int>> cards;
    vector<int>* handfull[5];
    int handbase[5];

    HandParser(vector<vector<int>> cards);
    bool operator==(const HandParser& other);
    bool operator>(const HandParser& other);
    bool operator<(const HandParser& other);
    bool setStraightFlush();
    void setFourOfAKind();
    void setFullHouse();
    void setFlush();
    void setStraight();
    void setThreeOfAKind();
    void setTwoPair();
    void setOnePair();
    void setHighCard();
    void parse();
    void getKickers();
    void getFullHand();
    void repr();

private:
    static int* getStraightIndexes(int* valnums);
};


#endif //EXTENSIONS_HANDPARSER_H
