/* Branko Milosevic
Coursera C For Everyone - Structured Programming
wk02-Honors assignment
*/

#include <stdio.h>
#include <stdlib.h>

#define CARDS_IN_DECK 52
#define CARDS_IN_HAND 7
#define PIPS 13
#define SUITS 4
#define NUMBER_OF_DEALS 1000000
#define SLICE 1000

typedef enum suits { Hearts, Diamonds, Spades, Clubs } my_suits;
typedef struct cards { short pip; my_suits suit; } card;

void deck_init(card *deck)
{
    int i;
    int j;
    int index;

    for (i = 0; i < PIPS; i++)
    {
        for (j = 0; j < SUITS; j++)
        {
            index = i*SUITS + j;
            deck[index].pip = i;
            deck[index].suit = j;
        }
    }
}

void print_deck(card *deck)
{
    int i;

    printf("\nDeck:");
    for (i = 0; i < CARDS_IN_DECK; i++)
    {
        if (i % SUITS == 0) printf ("\n");

        printf("|");
        switch (deck[i].pip + 1)
        {
            case 1:  printf(" A"); break;
            case 2:  printf(" 2"); break;
            case 3:  printf(" 3"); break;
            case 4:  printf(" 4"); break;
            case 5:  printf(" 5"); break;
            case 6:  printf(" 6"); break;
            case 7:  printf(" 7"); break;
            case 8:  printf(" 8"); break;
            case 9:  printf(" 9"); break;
            case 10: printf("10"); break;
            case 11: printf(" J"); break;
            case 12: printf(" Q"); break;
            case 13: printf(" K"); break;
        }
        switch (deck[i].suit)
        {
            case Hearts: printf("-H"); break;
            case Diamonds: printf("-D"); break;
            case Spades: printf("-S"); break;
            case Clubs: printf("-C"); break;
        }
        printf("|");
    }
    printf("\n=====\n");
}

void deck_shuffle (card *deck)
{
    int i;
    int index_1;
    int index_2;
    int pip_temp;

    srand(time(0));

    for (i = 0; i < CARDS_IN_DECK; i++)
    {
        index_1 = rand() % CARDS_IN_DECK;
        index_2 = rand() % CARDS_IN_DECK;

        if (index_1 != index_2)
        {
            pip_temp = deck[index_1].pip;
            deck[index_1].pip = deck[index_2].pip;
            deck[index_2].pip = pip_temp;
        }
    }
}

void hand_deal(card *deck, card *hand)
{
    int i;
    int j;
    card temp;

    for (i = 0; i < CARDS_IN_HAND; i++)
    {
        hand[i].pip = deck[i].pip;
        hand[i].suit = deck[i].suit;
    }

    for (i = 0; i < CARDS_IN_HAND-1; i++)
        for (j = 0; j < CARDS_IN_HAND-i-1; j++)
        {
            if (hand[j].pip > hand[j+1].pip)
            {
                temp.pip = hand[j].pip;
                temp.suit = hand[j].suit;

                hand[j].pip = hand[j+1].pip;
                hand[j].suit = hand[j+1].suit;

                hand[j+1].pip = temp.pip;
                hand[j+1].suit = temp.suit;
            }
        }
}

int four_of_a_kind(card *hand)
{
    int p1 = hand[0].pip;
    int p2 = hand[1].pip;
    int p3 = hand[2].pip;
    int p4 = hand[3].pip;
    int p5 = hand[4].pip;
    int p6 = hand[5].pip;
    int p7 = hand[6].pip;

    if ( (p1 == p2) && (p2 == p3) && (p3 == p4) ) return 1;
    if ( (p2 == p3) && (p3 == p4) && (p4 == p5) ) return 1;
    if ( (p4 == p5) && (p5 == p6) && (p6 == p7) ) return 1;

    return 0;
}

int full_house (card *hand)
{
    int p1 = hand[0].pip;
    int p2 = hand[1].pip;
    int p3 = hand[2].pip;
    int p4 = hand[3].pip;
    int p5 = hand[4].pip;
    int p6 = hand[5].pip;
    int p7 = hand[6].pip;

    // three of a ind followed by pair
    if ( (p1 == p2) && (p2 == p3) && (p4 == p5) ) return 1;
    if ( (p1 == p2) && (p2 == p3) && (p5 == p6) ) return 1;
    if ( (p1 == p2) && (p2 == p3) && (p6 == p7) ) return 1;
    if ( (p2 == p3) && (p3 == p4) && (p5 == p6) ) return 1;
    if ( (p2 == p3) && (p3 == p4) && (p6 == p7) ) return 1;
    if ( (p3 == p4) && (p4 == p5) && (p6 == p7) ) return 1;

    // par followed by three of a kind
    if ( (p1 == p2) && (p3 == p4) && (p4 == p5) ) return 1;
    if ( (p1 == p2) && (p4 == p5) && (p5 == p5) ) return 1;
    if ( (p1 == p2) && (p5 == p6) && (p6 == p7) ) return 1;
    if ( (p2 == p3) && (p4 == p5) && (p5 == p6) ) return 1;
    if ( (p2 == p3) && (p5 == p6) && (p6 == p7) ) return 1;
    if ( (p3 == p4) && (p5 == p6) && (p6 == p7) ) return 1;

    return 0;
}

int three_of_a_kind(card *hand)
{
    int p1 = hand[0].pip;
    int p2 = hand[1].pip;
    int p3 = hand[2].pip;
    int p4 = hand[3].pip;
    int p5 = hand[4].pip;
    int p6 = hand[5].pip;
    int p7 = hand[6].pip;

    if ( (p1 == p2) && (p2 == p3) ) return 1;
    if ( (p2 == p3) && (p3 == p4) ) return 1;
    if ( (p4 == p5) && (p5 == p6) ) return 1;
    if ( (p5 == p6) && (p6 == p7) ) return 1;

    return 0;
}

int two_pairs (card *hand)
{
    int p1 = hand[0].pip;
    int p2 = hand[1].pip;
    int p3 = hand[2].pip;
    int p4 = hand[3].pip;
    int p5 = hand[4].pip;
    int p6 = hand[5].pip;
    int p7 = hand[6].pip;

    if ( (p1 == p2) && (p3 == p4) ) return 1;
    if ( (p1 == p2) && (p4 == p5) ) return 1;
    if ( (p1 == p2) && (p5 == p6) ) return 1;
    if ( (p1 == p2) && (p6 == p7) ) return 1;

    if ( (p2 == p3) && (p4 == p5) ) return 1;
    if ( (p2 == p3) && (p5 == p6) ) return 1;
    if ( (p2 == p3) && (p6 == p7) ) return 1;

    if ( (p3 == p4) && (p5 == p6) ) return 1;
    if ( (p3 == p4) && (p6 == p7) ) return 1;

    if ( (p4 == p5) && (p6 == p7) ) return 1;

    return 0;
}

int pair (card *hand)
{
    int p1 = hand[0].pip;
    int p2 = hand[1].pip;
    int p3 = hand[2].pip;
    int p4 = hand[3].pip;
    int p5 = hand[4].pip;
    int p6 = hand[5].pip;
    int p7 = hand[6].pip;

    if (p1 == p2) return 1;
    if (p2 == p3) return 1;
    if (p3 == p4) return 1;
    if (p4 == p5) return 1;
    if (p5 == p6)  return 1;
    if (p6 == p7)  return 1;

    return 0;
}

void print_hand(card *hand)
{
    int i;

    printf("\nHand:");
    for (i = 0; i < CARDS_IN_HAND; i++)
    {
        printf("|");
        switch (hand[i].pip + 1)
        {
            case 1:  printf(" A"); break;
            case 2:  printf(" 2"); break;
            case 3:  printf(" 3"); break;
            case 4:  printf(" 4"); break;
            case 5:  printf(" 5"); break;
            case 6:  printf(" 6"); break;
            case 7:  printf(" 7"); break;
            case 8:  printf(" 8"); break;
            case 9:  printf(" 9"); break;
            case 10: printf("10"); break;
            case 11: printf(" J"); break;
            case 12: printf(" Q"); break;
            case 13: printf(" K"); break;
        }
        switch (hand[i].suit)
        {
            case Hearts: printf("-H"); break;
            case Diamonds: printf("-D"); break;
            case Spades: printf("-S"); break;
            case Clubs: printf("-C"); break;
        }
        printf("|");
    }
    // printf("\n=====\n");
}

int main()
{
    int i;
    int j;
    int four_oak_num = 0;
    int three_oak_num = 0;
    int two_pairs_num = 0;
    int pair_num = 0;
    int f_house_num = 0;
    int nothing_num = 0;

    card deck[CARDS_IN_DECK];
    card hand[CARDS_IN_HAND];

    deck_init(deck);
//    print_deck(deck);

    for (j = 0; j < NUMBER_OF_DEALS/SLICE; j++)
    {
        for (i = 0; i < SLICE; i++)
        {
            deck_shuffle(deck);
            hand_deal(deck, hand);
    //        print_hand(hand);
    //        if (four_of_a_kind(hand))  { printf(" -> 4 of a kind \n"); four_oak_num++;  continue; }
    //        if (full_house(hand))      { printf(" -> full house  \n"); f_house_num++;   continue; }
    //        if (three_of_a_kind(hand)) { printf(" -> 3 of a kind \n"); three_oak_num++; continue; }
    //        if (two_pairs(hand))       { printf(" -> two pairs   \n"); two_pairs_num++; continue; }
    //        if (pair(hand))            { printf(" -> pair        \n"); pair_num++;      continue; }

            if (four_of_a_kind(hand))  { four_oak_num++;  continue; }
            if (full_house(hand))      { f_house_num++;   continue; }
            if (three_of_a_kind(hand)) { three_oak_num++; continue; }
            if (two_pairs(hand))       { two_pairs_num++; continue; }
            if (pair(hand))            { pair_num++;      continue; }

            nothing_num++;
        }
        printf (".");
    }

    printf ("\n\n=== Report ===\n");
    printf ("FOUR OF A KIND:  %6d\t %5.2f%%\n", four_oak_num,  100.0*four_oak_num/NUMBER_OF_DEALS);
    printf ("FULL HOUSE:      %6d\t %5.2f%%\n", f_house_num,   100.0*f_house_num/NUMBER_OF_DEALS);
    printf ("THREE OF A KIND: %6d\t %5.2f%%\n", three_oak_num, 100.0*three_oak_num/NUMBER_OF_DEALS);
    printf ("TWO PAIRS:       %6d\t %5.2f%%\n", two_pairs_num, 100.0*two_pairs_num/NUMBER_OF_DEALS);
    printf ("PAIRS:           %6d\t %5.2f%%\n", pair_num,      100.0*pair_num/NUMBER_OF_DEALS);
    printf ("NOTHING:         %6d\t %5.2f%%\n", nothing_num,   100.0*nothing_num/NUMBER_OF_DEALS);
}
