#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>

#include <conio.h>
#include "HandParser.h"

using std::cout;
using std::endl;
using namespace std::chrono;

double simulate(vector<vector<int>> incards, int nsim)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> yield_rand(0, 51);

    int counter, rand, sample[15];
    double sum = 0;
    bool in_sample[52];

    vector<vector<int>> cards{};
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 4; j++)
            cards.push_back(vector<int> {i, j});

    in_sample[incards[0][0] * 4 + incards[0][1]] = true;
    in_sample[incards[1][0] * 4 + incards[1][1]] = true;

    vector<vector<int>> player_cards{7};
    for (int i = 0; i < nsim; i++)
    {
        counter = 0;
        while (counter < 15) {
            rand = yield_rand(rd);
            if (!*(in_sample + rand)) {
                *(in_sample + rand) = true;
                *(sample + counter) = rand;
                counter++;
            }
        }
        for (int& i : sample) *(in_sample + i) = false;

        for (int i = 0; i < 5; i++)
            player_cards[i] = cards[*(sample + i)];

        player_cards[5] = incards[0];
        player_cards[6] = incards[1];
        HandParser inhand = HandParser (player_cards);
        inhand.parse();

        for (int i = 0; i < 5; i++) {
            counter = 5 + 2 * i;
            player_cards[5] = cards[*(sample + counter)];
            player_cards[6] = cards[*(sample + counter + 1)];

            HandParser player_hand = HandParser (player_cards);
            player_hand.parse();
            if (player_hand > inhand)
                break;

            if (i == 4) sum++;
        }
    }

    return sum / nsim;
}

int main() {
    vector<vector<int>> mycards = {{12,0}, {12,1}};
    auto start = high_resolution_clock::now();
    cout << simulate(mycards, pow(10, 3)) << endl;
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    cout << elapsed.count() << endl;
    getch();
    return 0;
}