#include <random>
#include <algorithm>

#include "../include/HandParser.h"
#include "../include/simulation.h"

using std::vector;

double py_ext::simulate(vector<vector<int>> incards, int nsim)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> yield_rand(0, 51);

    double wins = 0;
    int counter, rand, sample[15]{};
    bool in_sample[52]{};

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
            rand = yield_rand(gen);
            if (!*(in_sample + rand)) {
                *(in_sample + rand) = true;
                *(sample + counter) = rand;
                counter++;
            }
        }

        for (int& s : sample) *(in_sample + s) = false;

        for (int j = 0; j < 5; j++)
            player_cards[j] = cards[*(sample + j)];

        player_cards[5] = incards[0];
        player_cards[6] = incards[1];
        HandParser inhand = HandParser (player_cards);
        inhand.parse();

        for (int j = 0; j < 5; j++)
        {
            counter = 5 + 2 * j;
            player_cards[5] = cards[*(sample + counter)];
            player_cards[6] = cards[*(sample + counter + 1)];

            HandParser player_hand = HandParser (player_cards);
            player_hand.parse();
            if (player_hand > inhand)
                break;

            if (j == 4) wins++;
        }
    }

    return wins / nsim;
}