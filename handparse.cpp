#include <iostream>
#include <cstring>
#include <vector>
#include <bits/stdc++.h>
#include <conio.h>
using std::vector;
using std::cout;
using std::endl;

enum Hand {
  HIGHCARD, ONEPAIR, TWOPAIR, THREEOFAKIND,
  STRAIGHT, FLUSH, FULLHOUSE, FOUROFAKIND,
  STRAIGHTFLUSH
};

class HandParser {

  public:
    Hand handenum;
    vector<vector<int>> cards;
    vector<int> handbase, kickers;
    int valnums[13] = {0}, suitnums[4] = {0}, hand[5];

    HandParser(vector<vector<int>> cards) : cards(cards) {
      sort(this->cards.begin(), this->cards.end());
      for (vector<int> card : this->cards) {
        this->valnums[card[0]]++;
        this->suitnums[card[1]]++;
      }
    }

    bool operator==(const HandParser& other) {
      if (this->handenum != other.handenum)
        return false;
      for (int i = 0; i < 5; i++) {
        int t_val = this->cards[this->hand[i]][0];
        int o_val = other.cards[this->hand[i]][0];
        if (t_val != o_val)
          return false;
      }
      return true;
    }

    bool operator>(const HandParser& other) {
      if (this->handenum != other.handenum)
        return this->handenum > other.handenum;
      for (int i = 0; i < 5; i++) {
        int t_val = this->cards[this->hand[i]][0];
        int o_val = other.cards[this->hand[i]][0];
        if (t_val != o_val)
          return t_val > o_val;
      }
      return false;
    }

    bool operator<(const HandParser& other) {
      if (this->handenum != other.handenum)
        return this->handenum < other.handenum;
      for (int i = 0; i < 5; i++) {
        int t_val = this->cards[this->hand[i]][0];
        int o_val = other.cards[this->hand[i]][0];
        if (t_val != o_val)
          return t_val < o_val;
      }
      return false;
    }

    void parse() {
      vector<vector<int>> cardholder;
      int pairholder[2] = {0,0};

      int npairs[5] = {0,0,0,0,0};
      for (int i = 0; i < 13; i++)
        npairs[this->valnums[i]]++;

      int straightstart = this->checkStraight(this->valnums);
      int flushsuit = -1;
      for (int i = 0; i < 4; i++) {
        if (this->suitnums[i] >= 5) {
          flushsuit = i;
          break;
        }
      }

      // straight flush
      if (straightstart >= 0 && flushsuit != -1) {
        int permut[7] = {0}, suited_vals[13] = {0}, i = 0;
        for (vector<int> card : this->cards) {
          if (card[1] == flushsuit) {
            permut[cardholder.size()] = i;
            suited_vals[card[0]]++;
            cardholder.push_back(card);
          }
          i++;
        }
        int sfstart = this->checkStraight(suited_vals);
        if (sfstart != -1) {
          this->handenum = Hand::STRAIGHTFLUSH;
          vector<int> suited_base = this->getStraightFrom(cardholder, sfstart);
          for (int i : suited_base) this->handbase.push_back(permut[i]);
          cardholder.clear();
          return;
        }
      }

      // four of a kind
      if (npairs[4]) {
        int val;
        for (int i = 0; i < 13; i++) {
          if (this->valnums[i] == 4)
            val = i;
        }
        for (int i = 7; i >= 0; i--) {
          if (this->cards[i][0] == val)
            this->handbase.push_back(i);
        } this->handenum = Hand::FOUROFAKIND;
      }

      // full house
      else if (npairs[3] == 2 || npairs[3] == 1 && npairs[2] >= 1) {
        int num, three = -1, two = -1;
        for (int i = 12; i >= 0; i--) {
          num = this->valnums[i];
          if (num == 3 && three == -1) three = i;
          else if (num == 2 && two == -1) two = i;
          else if (num == 3 && three != -1) {
            pairholder[0] = three;
            pairholder[1] = i;
            break;
          }
          if (two != -1 && three != -1) {
            pairholder[0] = three;
            pairholder[1] = two;
            break;
          }
        }
        int val, i = 7;
        while (this->handbase.size() < 5) {
          val = this->cards[--i][0];
          if (val == pairholder[0] || val == pairholder[1])
            this->handbase.push_back(i);
        } this->handenum = Hand::FULLHOUSE;
      }

      // flush
      else if (flushsuit != -1) {
        this->handenum = Hand::FLUSH;
        int i = 0;
        while (this->handbase.size() < 5) {
          if (this->cards[i][1] == flushsuit)
            this->handbase.push_back(i);
          i++;
        }
      }

      // straight
      else if (straightstart != -1) {
        this->handenum = Hand::STRAIGHT;
        this->handbase = this->getStraightFrom(this->cards, straightstart);
      }

      // three of a kind
      else if (npairs[3] == 1) {
        int val = 0;
        for (int i = 0; i < 13; i++) {
          if (this->valnums[i] == 3)
            val = i;
            break;
        }
        for (int i = 0; i < 7; i++) {
          if (this->cards[i][0] == val)
            this->handbase.push_back(i);
        } this->handenum = Hand::THREEOFAKIND;
      }

      // two pair
      else if (npairs[2] >= 2) {
        int count = 0;
        for (int i = 12; i >= 0; i--) {
          if (this->valnums[i] == 2) {
            pairholder[count++] = i;
            if (count == 2) break;
          }
        }
        int i = 7, val;
        while (this->handbase.size() < 4) {
          val = this->cards[--i][0];
          if (val == pairholder[0] || val == pairholder[1])
            this->handbase.push_back(i);
        } this->handenum = Hand::TWOPAIR;
      }

      // one pair
      else if (npairs[2] == 1) {
        int pair;
        for (int i = 12; i >= 0; i--) {
          if (this->valnums[i] == 2)
            pair = i;
        }
        int i = 7, val;
        while (this->handbase.size() < 2) {
          val = this->cards[--i][0];
          if (val == pair)
            this->handbase.push_back(i);
        } this->handenum = Hand::ONEPAIR;
      }

      // high card
      else {
        this->handenum = Hand::HIGHCARD;
        this->handbase.push_back(6);
      }
    }

    void getKickers() {
      bool inhand[7] = {false}; int i = 7;
      this->kickers.clear();
      for (int i : this->handbase) inhand[i] = true;
      while (this->kickers.size() < 5 - this->handbase.size())
        if (!inhand[--i]) this->kickers.push_back(i);
    }

    void getHand() {
      int hlen = this->handbase.size(), klen = this->kickers.size();
      for (int i = 0; i < hlen; i++)
        hand[i] = this->handbase[i];
      for (int i = hlen; i < hlen + klen; i++)
        hand[i] = this->handbase[i - hlen];
    }

  private:

    static int
    checkStraight(int valnums[]) {
      int straightcounter = 1;
      for (int i = 12; i >= 0; i--) {
        if (valnums[(i == 0) ? 12 : (i-1)] & valnums[i]) {
          straightcounter++;
          if (straightcounter == 5)
            return i;
        } else straightcounter = 1;
      }
      return -1;
    }

    static vector<int>
    getStraightFrom(vector<vector<int>> cards, int start) {
      int len = cards.size();
      bool instraight[13]; vector<int> idxs;
      for (int i = start - 1; i < start + 4; i++) {
        instraight[(i == -1) ? 12 : i] = true;
      }
      for (int i = 0; i < len; i++) {
        if (instraight[cards[i][0]]) {
          idxs.push_back(i);
          instraight[cards[i][0]] = false;
        }
      }
      return idxs;
    }

};

int main() {
  vector<vector<int> > first {
    {12,2}, {12,1}, {0,1},
    {2,1}, {3,1}, {4,2},
    {1,1}
  };
  vector<vector<int> > sec {
    {2,1}, {3,2}, {3,1},
    {0,2}, {11,0}, {5,2},
    {5,3}
  };
  HandParser* me = new HandParser(first);
  HandParser* you = new HandParser(sec);
  me->parse(); you->parse();
  me->getKickers(); you->getKickers();
  me->getHand(); you->getHand();

  bool win = *me < *you;
  cout << "me " << me->handenum << " you " << you->handenum << endl;
  cout << win << endl;

  delete me, you;
  getch();
  return 0;
}
