// special words:
//     1. pw: short for the probability of winning.

#include <stdio.h>
#include <math.h>

typedef unsigned long long      uint64;
typedef int                     bool;

#define true            1
#define false           0

#define CARD_TYPE_NUM   (4)     // the amount of cards type.
#define SINGLE_FLUSH    (13)    // the amount of single flush cards.
#define CARDS_SUM       (52)    // the amount of all cards.
#define CARDS_HOLD      (2)     // the amount of cards one player can hold.
#define CARDS_PUBLIC    (5)     // the amount of all public cards.
#define ALL_CARDS       (0xfffffffffffff000)    // every bits seted for every card, sorted with Spades (A－2...-K), Herts, Clubs, Diamonds.
#define SUCCESS         (0)
#define FAIL            (-1)

typedef enum
{
    PREFLOP,
    FLOPROUND,
    TURNROUND
}PHASE;

// calulate c(m, n).
int calc_combination(int sum, int choosen)
{
    int denominator = 1, numerator = 1;
    if (sum > (choosen<<2))
    {
        while (choosen > 0)
        {
            numerator *= sum--;
            denominator *= choosen--;
        }
        return numerator / denominator;
    } else
    {
        return calc_combination(sum, sum - choosen);
    }
}

// calulate n!.
int calc_permutation(int sum)
{
    int result = 0;
    while (sum > 1)
    {
        result *= sum--;
    }
    return result;
}

// calulate the combination of...
int calc_probability(int number)
{
    return calc_permutation(number) / pow(2, number/2);
}

// get all cards except cards_in.
void init_freecards(uint64 cards_in, uint64* free_cards)
{
    *free_cards = ALL_CARDS ^ cards_in;
}

//
bool get_next_group(uint64 free_cards, uint64* group_io, int cards_num)
{
    return false;
}

bool is_winner(uint64 hold_cards, uint64 opponent_hold, uint64 public_cards)
{
    return false;
}

int count_lose(uint64 hold_cards, uint64 public_cards, uint64 free_cards, int cards_num)
{
    uint64 opponent_hold = 0;          // The cards opponent hold.
    int loss_cards = 0;
    while (get_next_group(free_cards, &opponent_hold, CARDS_HOLD))
    {
        if (!is_winner(hold_cards, opponent_hold, public_cards))
        {
            loss_cards += calc_probability(cards_num - CARDS_HOLD);
        }
        else
        {
            loss_cards += count_lose(hold_cards, public_cards, free_cards ^ opponent_hold, cards_num - CARDS_HOLD);
        }
    }
    return loss_cards;
}

float pw_preflop(uint64 hold_cards, int player_num)
{
    int loss_cards;                     // Count when lose. No need to initialise.
    uint64 free_cards1, free_cards2;   // Cards can be choose. No need to initialise.
    uint64 public_cards = 0;           // Public cards.
    uint64 opponent_cards = 0;         // All cards that opponents can hold.
    uint64 opponent_hold = 0;          // The cards opponent hold.
    // The first loop is to test all public cards.
    init_freecards(hold_cards, &free_cards1);
    while (get_next_group(free_cards1, &public_cards, CARDS_PUBLIC))
    {
        // The second loop is to test all cards that oppenents can hold.
        init_freecards(hold_cards ^ public_cards, &free_cards2);
        while (get_next_group(free_cards2, &opponent_cards, player_num * CARDS_HOLD))
        {
            // Count if the player lose.
            loss_cards = count_lose(hold_cards, public_cards, opponent_cards, player_num * CARDS_HOLD);
        }
    }
    return 1.0 - (float) loss_cards / (float) (calc_combination(CARDS_SUM - CARDS_HOLD, CARDS_PUBLIC)
                                               * calc_combination(CARDS_SUM - CARDS_HOLD - CARDS_PUBLIC, player_num * CARDS_HOLD)
                                               * calc_probability(player_num * CARDS_HOLD));
}

int main(int argc, char** argv)
{
    return 0;
}
