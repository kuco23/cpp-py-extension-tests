#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>

#include <bits/stdc++.h>
#include "HandParser.h"

using namespace std;

static int SUM(int* const& ar, size_t len) {
  int count = 0;
  for (size_t i = 0; i < len; i++)
    count += *(ar+i);
  return count;
}

const int HandParser::HAND_LEN[9] = {1,2,4,3,5,5,5,4,5};

HandParser::HandParser(vector<vector<int>> cards) : cards(cards) {
    sort(this->cards.begin(), this->cards.end());

    vector<int> card;
    for (int i = 0; i < 7; i++) {
        card = this->cards[i];
        (*(this->valnums+card[0]))++;
        (*(this->suitnums+card[1]))++;
    }

    int* sindexes = getStraightIndexes(this->valnums);
    for (int i = 0; i < 5; i++)
        *(this->straightindexes+i) = *(sindexes+i);

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
    int counter = 0, permut[7] = {0};
    int suited_vals[13] = {0};

    vector<int>* card;
    for (int i = 0; i < 7; i++) {
        card = &this->cards[i];
        if ((*card)[1] == this->flushsuit) {
            (*(suited_vals+(*card)[0]))++;
            (*(permut+counter)) = i;
            counter++;
        }
    }

    int* suited_handbase = getStraightIndexes(suited_vals);
    if (*(suited_handbase+4) != -1) {
        this->handenum = Hand::STRAIGHTFLUSH;
        for (int i = 0; i < 5; i++)
            *(this->handbase+i) = *(permut+(*(suited_handbase+i)));
        return true;
    }

    return false;
}

void HandParser::setFourOfAKind() {
    this->handenum = Hand::FOUROFAKIND;

    int hindex = -1, valnum, count = 0;
    for (int i = 0; i < 13; i++) {
        valnum = *(this->valnums+i);
        hindex += valnum;
        if (valnum == 4) break;
    }

    for (int i = hindex-3; i <= hindex; i++)
        *(this->handbase+count++) = i;
}

void HandParser::setFullHouse() {
    this->handenum = Hand::FULLHOUSE;

    int valnum, three = -1, two = -1, hindex = 7;
    for (int i = 12; i >= 0; i--) {
        valnum = *(this->valnums+i);
        hindex -= valnum;
        if (valnum == 3 && three == -1) three = hindex;
        else if (valnum == 2 && two == -1) two = hindex;
        else if (valnum == 3 && three != -1) {
            two = hindex;
            break;
        }
        if (two != -1 && three != -1) break;
    }

    int count = 0,
            idxs[5] = {three, three+1, three+2, two, two+1};
    for (int i = 0; i < 5; i++)
        *(this->handbase+count++) = *(idxs+i);
}

void HandParser::setFlush() {
    this->handenum = Hand::FLUSH;

    int i = 6, count = 0;
    while (count < 5) {
        if (this->cards[i][1] == this->flushsuit) {
            *(this->handbase+count) = i;
            count++;
        }
        i--;
    }
}

void HandParser::setStraight() {
    this->handenum = Hand::STRAIGHT;
    for (int i = 0; i < 5; i++)
        *(this->handbase+i) = *(this->straightindexes+i);
}

void HandParser::setThreeOfAKind() {
    this->handenum = Hand::THREEOFAKIND;

    int hindex = -1, valnum, count = 0;
    for (int i = 0; i < 13; i++) {
        valnum = *(this->valnums+i);
        hindex += valnum;
        if (valnum == 3)
            break;
    }

    for (int i = hindex-2; i <= hindex; i++)
        *(this->handbase+count++) = i;
}

void HandParser::setTwoPair() {
    this->handenum = Hand::TWOPAIR;

    int hindex = 7, counter = 0, valnum;
    for (int i = 12; i >= 0; i--) {
        valnum = *(this->valnums+i);
        hindex -= valnum;
        if (valnum == 2) {
            *(this->handbase+2*counter) = hindex;
            *(this->handbase+2*++counter) = hindex+1;
            if (counter == 2)
                break;
        }
    }
}

void HandParser::setOnePair() {
    this->handenum = Hand::ONEPAIR;

    int hindex = -1, count = 0, valnum;
    for (int i = 0; i < 13; i++) {
        valnum = *(this->valnums+i);
        hindex += valnum;
        if (valnum == 2)
            break;
    }

    *(this->handbase) = hindex-1;
    *(this->handbase+1) = hindex;
}

void HandParser::setHighCard() {
    this->handenum = Hand::HIGHCARD;
    *this->handbase = 6;
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
    int i = 7, counter = 0, hlen;
    hlen = HAND_LEN[static_cast<int>(this->handenum)];

    bool inhand[7];
    for (int j = 0; j < hlen; j++)
        *(inhand+(*(this->handbase+j))) = true;

    while (counter < 5 - hlen)
        if (!inhand[--i]) *(this->handbase+hlen+counter++) = i;
}

void HandParser::getFullHand() {
    for (int i = 0; i < 5; i++)
        *(this->handfull+i) = &this->cards[*(this->handbase+i)];
}

int* HandParser::getStraightIndexes(int* valnums) {
    static int straightindexes[5];
    for (int & straightindex : straightindexes) straightindex = -1;
    int straightlen = 1, valnum_sum = SUM(valnums, 13);
    int hindex = valnum_sum;

    for (int i = 12; i >= 0; i--) {
        hindex -= *(valnums+i);
        if (*(valnums+((i == 0) ? 12 : (i-1))) && *(valnums+i)) {
            *(straightindexes+straightlen-1) = hindex;
            straightlen++;
            if (straightlen == 5) {
                hindex = (i==0) ? valnum_sum-1 : hindex - *(valnums+i-1);
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

