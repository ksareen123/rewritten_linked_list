#pragma once

#include <fstream>

#include "List.h"

enum class Suit { SPADES, DIAMONDS, CLUBS, HEARTS };

using Rank = int;

constexpr Rank ACE   = 1;
constexpr Rank JACK  = 11;
constexpr Rank QUEEN = 12; 
constexpr Rank KING  = 13;

struct Card {
    Suit suit;
    Rank rank;
};

List<Card> buildDeck(std::istream& file);

List<Card> shuffle(const List<Card>& deck);

/*
    Use the "rand221" function to generate random numbers 
    for your shuffling algorithm. It is effectivly the same as 
    the C function "rand" but allows us to test 
    the correctness of your shuffle method.

    Example:

    bool insert_front = rand221() % 2 == 0; 
*/
#if defined(__GNUC__) || defined(__clang__) 
#define WEAK_LINKAGE __attribute__((weak))
#else
#define WEAK_LINKAGE
#endif

int rand221(void) WEAK_LINKAGE;