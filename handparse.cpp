#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <random>

#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <iterator>

#include <conio.h>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

enum Hand {
  HIGHCARD, ONEPAIR, TWOPAIR, THREEOFAKIND,
  STRAIGHT, FLUSH, FULLHOUSE, FOUROFAKIND,
  STRAIGHTFLUSH
};

static std::map<Hand, int> HAND_LEN = {
      {Hand::HIGHCARD, 1}, {Hand::ONEPAIR, 2},
      {Hand::TWOPAIR, 4}, {Hand::THREEOFAKIND, 3},
      {Hand::STRAIGHT, 5}, {Hand::FLUSH, 5},
      {Hand::FULLHOUSE, 5}, {Hand::FOUROFAKIND, 4},
      {Hand::STRAIGHTFLUSH, 5}
    };


static int SUM(int* ar, size_t len) {
  int count = 0;
  for (size_t i = 0; i < len; i++)
    count += *(ar+i);
  return count;
}

class HandParser {

  private:
    int valnums[13] = {0}, suitnums[4] = {0},
    straightindexes[5], flushsuit = -1;

  public:
    Hand handenum;
    vector<vector<int>> cards;
  	vector<int>* handfull[5];
  	int handbase[5];

    HandParser(vector<vector<int>> cards) : cards(cards) {
      sort(this->cards.begin(), this->cards.end());

      vector<int> card;
      for (int i = 0; i < 7; i++) {
        card = this->cards[i];
        (*(this->valnums+card[0]))++;
        (*(this->suitnums+card[1]))++;
      }

      int* straightindexes = getStraightIndexes(this->valnums);
      for (int i = 0; i < 5; i++)
        *(this->straightindexes+i) = *(straightindexes+i);

      for (int i = 0; i < 4; i++)
        if (*(this->suitnums+i) >= 5) {
          this->flushsuit = i;
          break;
        }
    }

    bool operator==(const HandParser& other) {
      if (this->handenum != other.handenum)
        return false;
      for (int i = 0; i < 5; i++) {
        int t_val = (*(*(this->handfull+i)))[0];
        int o_val = (*(*(other.handfull+i)))[0];
        if (t_val != o_val)
          return false;
      }
      return true;
    }

    bool operator>(const HandParser& other) {
      if (this->handenum != other.handenum)
        return this->handenum > other.handenum;
      for (int i = 0; i < 5; i++) {
        int t_val = (*(*(this->handfull+i)))[0];
        int o_val = (*(*(other.handfull+i)))[0];
        if (t_val != o_val)
          return t_val > o_val;
      }
      return false;
    }

    bool operator<(const HandParser& other) {
      if (this->handenum != other.handenum)
        return this->handenum < other.handenum;
      for (int i = 0; i < 5; i++) {
        int t_val = (*(*(this->handfull+i)))[0];
        int o_val = (*(*(other.handfull+i)))[0];
        if (t_val != o_val)
          return t_val < o_val;
      }
      return false;
    }

    bool setStraightFlush() {
      int counter = 0, suited_vals[13] = {0}, permut[7] = {0};

      vector<int>* card;
      for (int i = 0; i < 7; i++) {
        card = &this->cards[i];
        if ((*card)[1] == this->flushsuit) {
          (*(suited_vals+(*card)[0]))++;
          (*(permut+counter)) = i;
          counter++;
        }
      }

      int* suited_handbase = this->getStraightIndexes(suited_vals);
      if (*(suited_handbase+4) != -1) {
        this->handenum = Hand::STRAIGHTFLUSH;
        for (int i = 0; i < 5; i++)
          *(this->handbase+i) = *(permut+(*(suited_handbase+i)));
        return true;
      }

      return false;
    }

    void setFourOfAKind() {
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

    void setFullHouse() {
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

    void setFlush() {
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

    void setStraight() {
      this->handenum = Hand::STRAIGHT;
      for (int i = 0; i < 5; i++)
        *(this->handbase+i) = *(this->straightindexes+i);
    }

    void setThreeOfAKind() {
      this->handenum = Hand::THREEOFAKIND;

      int hindex = -1, valnum, count = 0;
      for (int i = 0; i < 13; i++) {
        valnum = *(this->valnums+i);
        hindex += valnum;
        if (valnum == 3) break;
      }

      for (int i = hindex-2; i <= hindex; i++)
        *(this->handbase+count++) = i;
    }

    void setTwoPair() {
      this->handenum = Hand::TWOPAIR;

      int hindex = 7, counter = 0, valnum;
      for (int i = 12; i >= 0; i--) {
        valnum = *(this->valnums+i);
        hindex -= valnum;
        if (valnum == 2) {
          *(this->handbase+2*counter) = hindex;
          *(this->handbase+2*counter+1) = hindex+1;
          counter++;
          if (counter == 2)
            break;
        }
      }
    }

    void setOnePair() {
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

    void setHighCard() {
      this->handenum = Hand::HIGHCARD;
      *this->handbase = 6;
    }

    void parse() {
      int npairs[5] = {0,0,0,0,0};
      for (int i = 0; i < 13; i++)
        (*(npairs+*(this->valnums+i)))++;

      if (this->flushsuit != -1 &&
        *(this->straightindexes+4) != -1 &&
        this->setStraightFlush());

      else if (npairs[4])
        this->setFourOfAKind();

      else if (npairs[3] == 2 || npairs[3] == 1 && npairs[2] >= 1)
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

    void getKickers() {
      int i = 7, counter = 0, hlen = HAND_LEN[this->handenum];

      bool inhand[7];
      for (int j = 0; j < hlen; j++)
        *(inhand+(*(this->handbase+j))) = true;

      while (counter < 5 - hlen)
        if (!inhand[--i]) *(this->handbase+hlen+counter++) = i;
    }

    void getFullHand() {
      for (int i = 0; i < 5; i++)
        *(this->handfull+i) = &this->cards[*(this->handbase+i)];
    }

    void repr() {
      cout << "handenum " << this->handenum << endl;
      for (int i = 0; i < 7; i++)
        cout << "card " << this->cards[i][0] << " " << this->cards[i][1] << endl;
      for (int i = 0; i < HAND_LEN[this->handenum]; i++)
        cout << "handbase " << *(this->handbase+i) << endl;
    }

  private:

    static int* getStraightIndexes(int* valnums) {
      static int straightindexes[5];

      for (int i = 0; i < 5; i++) straightindexes[i] = -1;
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

};

int main() {
  vector<vector<int>> cards, in;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 13; j++) {
      cards.push_back(vector<int> {j,i});
    }
  }

  double n = pow(10,5);
  std::srand(time(0));
  auto start = high_resolution_clock::now();

  for (int i = 0; i < n; i++) {
    in.clear();
    std::random_shuffle(cards.begin(), cards.end());
    for (int i = 0; i < 7; i++) in.push_back(cards.at(i));

    HandParser hand(in);
    hand.parse();
  }

  auto end = high_resolution_clock::now();
  duration<double> elapsed = end - start;
  cout << "ellapsed: " << elapsed.count() << endl;

  getch();
  return 0;
}
