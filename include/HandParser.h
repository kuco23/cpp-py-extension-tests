#ifndef EXTENSIONS_HANDPARSER_H
#define EXTENSIONS_HANDPARSER_H

#include <vector>
using std::vector;

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
    static const int NCARDS;
    static const int HAND_LEN[9];
    int valnums[13]{}, suitnums[4]{},
    straightindexes[5]{}, flushsuit;

public:
    vector<vector<int>> cards;
    vector<int> *handfull[5]{};
    Hand handenum;
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

private:
    static int* getStraightIndexes(int *valnums);
};

#endif //EXTENSIONS_HANDPARSER_H
