#include <vector>
using namespace std;

#ifndef EXTENSIONS_HANDPARSER_H
#define EXTENSIONS_HANDPARSER_H

enum Hand {
    HIGHCARD,
    ONEPAIR,
    TWOPAIR,
    THREEOFAKIND,
    STRAIGHT,
    FLUSH,
    FULLHOUSE,
    FOUROFAKIND,
    STRAIGHTFLUSH,
    NONE
};

class HandParser {
private:
    static const int HAND_LEN[9];
    int valnums[13]{}, suitnums[4]{},
    straightindexes[5]{}, flushsuit;

public:
    Hand handenum;
    vector<vector<int>> cards;
    vector<int>* handfull[5]{};
    int handbase[5]{};

    explicit HandParser(vector<vector<int>> cards);
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
