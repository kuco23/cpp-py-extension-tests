#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

#include <bits/stdc++.h>
#include "../include/HandParser.h"

using std::vector;
using std::cout;
using std::endl;
using std::accumulate;

// length of base cards according to hand enum (index)
const int HandParser::HAND_LEN[9] = {1,2,4,3,5,5,5,4,5};
// value shouldn't be changed
const int HandParser::NCARDS = 7;

/*
  HandParser "set" methods should
  only be called when there exists
  a hand for which setting is called
*/

HandParser::HandParser(vector<vector<int>> cards) : cards(std::move(cards)) {
    sort(this->cards.begin(), this->cards.end());

    for (vector<int> &card : this->cards) {
        (*(this->valnums+card[0]))++;
        (*(this->suitnums+card[1]))++;
    }

    int *sindexes = getStraightIndexes(this->valnums);
    std::copy(sindexes, sindexes+5, this->straightindexes);

    this->flushsuit = -1;
    for (int i = 0; i < 4; i++)
        if (*(this->suitnums+i) >= 5) {
            this->flushsuit = i;
            break;
        }

    this->handenum = Hand::NONE;
}

bool HandParser::operator==(const HandParser &other) {
    if (this->handenum != other.handenum)
        return false;
    for (int i = 0; i < 5; i++) {
        int t_val = (**(this->handfull+i))[0];
        int o_val = (**(other.handfull+i))[0];
        if (t_val != o_val)
            return false;
    }
    return true;
}

bool HandParser::operator>(const HandParser &other) {
    if (this->handenum != other.handenum)
        return this->handenum > other.handenum;
    for (int i = 0; i < 5; i++) {
        int t_val = (**(this->handfull+i))[0];
        int o_val = (**(other.handfull+i))[0];
        if (t_val != o_val)
            return t_val > o_val;
    }
    return false;
}

bool HandParser::operator<(const HandParser &other) {
    if (this->handenum != other.handenum)
        return this->handenum < other.handenum;
    for (int i = 0; i < 5; i++) {
        int t_val = (**(this->handfull+i))[0];
        int o_val = (**(other.handfull+i))[0];
        if (t_val != o_val)
            return t_val < o_val;
    }
    return false;
}

bool HandParser::setStraightFlush() {
    int permut[NCARDS]{}, suited_vals[13]{},
    count{};

    vector<int> *card;
    for (int i = 0; i < 7; i++) {
        card = &this->cards[i];
        if ((*card)[1] == this->flushsuit) {
            (*(suited_vals+(*card)[0]))++;
            (*(permut+count)) = i;
            count++;
        }
    }

    int *suited_handbase = getStraightIndexes(suited_vals);
    if (*(suited_handbase+4) != -1) {
        this->handenum = Hand::STRAIGHTFLUSH;
        for (int i = 0; i < 5; i++)
            *(this->handbase+i) = permut[*(suited_handbase+i)];
        return true;
    }
    return false;
}

void HandParser::setFourOfAKind() {
    this->handenum = Hand::FOUROFAKIND;

    int valnum, hindex = -1, count = 2;
    for (int i = 0; i < 13; i++) {
        valnum = *(this->valnums+i);
        hindex += valnum;
        if (valnum == 4) break;
    }

    for (int i = hindex-3; i <= hindex; i++)
        *(this->handbase+count--) = i;
}

void HandParser::setFullHouse() {
    this->handenum = Hand::FULLHOUSE;

    int valnum, two = -1, three = -1, hindex = NCARDS;
    for (int i = 12; i >= 0; i--) {
        valnum = *(this->valnums+i);
        hindex -= valnum;
        if (valnum == 3) ((three == -1) ? three : two) = hindex;
        else if (valnum == 2 && two == -1) two = hindex;
        if (three != -1 && two != -1) break;
    }

    int idxs[5] = {three+2, three+1, three, two+1, two};
    std::copy(idxs, idxs+5, this->handbase);
}

void HandParser::setFlush() {
    this->handenum = Hand::FLUSH;

    int count{};
    for (int i = NCARDS - 1; count < 5; i--)
        if (this->cards[i][1] == this->flushsuit)
            *(this->handbase+count++) = i;
}

void HandParser::setStraight() {
    this->handenum = Hand::STRAIGHT;

    std::copy(
       this->straightindexes,
       this->straightindexes+5,
       this->handbase
     );
}

void HandParser::setThreeOfAKind() {
    this->handenum = Hand::THREEOFAKIND;

    int valnum, hindex = -1, count = 2;
    for (int i = 0; i < 13; i++) {
        valnum = *(this->valnums+i);
        hindex += valnum;
        if (valnum == 3) break;
    }

    for (int i = hindex-2; i <= hindex; i++)
        *(this->handbase+count--) = i;
}

void HandParser::setTwoPair() {
    this->handenum = Hand::TWOPAIR;

    int valnum, count{}, hindex = NCARDS;
    for (int i = 12; i >= 0; i--) {
        valnum = *(this->valnums+i);
        hindex -= valnum;
        if (valnum == 2) {
            *(this->handbase+2*count) = hindex+1;
            *(this->handbase+2*count+++1) = hindex;
            if (count == 2) break;
        }
    }
}

void HandParser::setOnePair() {
    this->handenum = Hand::ONEPAIR;

    int valnum, hindex = -1;
    for (int i = 0; i < 13; i++) {
        valnum = *(this->valnums+i);
        hindex += valnum;
        if (valnum == 2) break;
    }

    *(this->handbase) = hindex;
    *(this->handbase+1) = hindex-1;
}

void HandParser::setHighCard() {
    this->handenum = Hand::HIGHCARD;
    *this->handbase = NCARDS - 1;
}

void HandParser::parse() {
    int npairs[5]{};
    for (int i = 0; i < 13; i++)
        (*(npairs+*(this->valnums+i)))++;

    if (this->flushsuit != -1 &&
        *(this->straightindexes+4) != -1 &&
        this->setStraightFlush());

    else if (npairs[4])
        this->setFourOfAKind();

    else if (npairs[3] == 2 || (npairs[3] == 1 && npairs[2] >= 1))
        this->setFullHouse();

    else if (this->flushsuit != -1)
        this->setFlush();

    else if (*(this->straightindexes+4) != -1)
        this->setStraight();

    else if (npairs[3] == 1)
        this->setThreeOfAKind();

    else if (npairs[2] >= 2)
        this->setTwoPair();

    else if (npairs[2] == 1)
        this->setOnePair();

    else this->setHighCard();

    this->getKickers();
    this->getFullHand();
}

void HandParser::getKickers() {
    int hlen, count{}, i = NCARDS;
    hlen = HAND_LEN[static_cast<int>(this->handenum)];

    bool inhand[NCARDS];
    for (int j = 0; j < hlen; j++)
        *(inhand+(*(this->handbase+j))) = true;

    while (count < 5 - hlen)
        if (!inhand[--i])
            *(this->handbase+hlen+count++) = i;
}

void HandParser::getFullHand() {
    for (int i = 0; i < 5; i++)
        *(this->handfull + i) = &this->cards[*(this->handbase + i)];
}

int* HandParser::getStraightIndexes(int *valnums) {
    static int straightindexes[5] = {-1,-1,-1,-1,-1};
    int valnum_sum = accumulate(valnums, valnums+13, 0);
    int straightlen = 1, hindex = valnum_sum;

    for (int i = 12; i >= 0; i--) {
        hindex -= *(valnums+i);
        if (*(valnums+((i == 0) ? 12 : (i-1))) && *(valnums+i)) {
            *(straightindexes+straightlen-1) = hindex;
            straightlen++;
            if (straightlen == 5) {
                hindex -= *(valnums+i-1);
                if (i==0) hindex = valnum_sum-1;
                *(straightindexes+4) = hindex;
                return straightindexes;
            }
        } else straightlen = 1;
    }

    return straightindexes;
}

void HandParser::repr() {
    int hlen = HAND_LEN[static_cast<int>(this->handenum)];
    cout << "handenum " << this->handenum << endl;
    for (int i = 0; i < 7; i++)
        cout << "card " << this->cards[i][0] << " " << this->cards[i][1] << endl;
    if (this->handenum != Hand::NONE) {
        for (int i = 0; i < hlen; i++)
            cout << "handbase " << *(this->handbase + i) << endl;
    }
}

