#include <string>
#include <sstream>
#include <iostream>

#include "Card.h"

using namespace std;

int rand221(void) {
    return rand();
}

// You may want to write the following three functions, but they are not required

std::istream& operator>>(std::istream& in, Suit& suit) {
    string s;
    in >> s;
    if (s == "spades") {
        suit = Suit::SPADES;
    }
    else if (s == "hearts") {
        suit = Suit::HEARTS;
    }
    else if (s == "diamonds") {
        suit = Suit::DIAMONDS;
    }
    else if (s == "clubs") {
        suit = Suit::CLUBS;
    }
}

Rank to_rank(const std::string& string) {
   if (string == "ace") {
       return ACE;
   }
   else if (string == "jack") {
       return JACK;
   }
   else if (string == "queen") {
       return QUEEN;
   }
   else if (string == "king") {
       return KING;
   }
   else {
       int num = stoi(string);
       return num;
   }
}

std::istream& operator>>(std::istream& in, Card& card) {
   string r;
   in >> card.suit;
   in >> r; 
   card.rank = to_rank(r);
}

List<Card> buildDeck(std::istream& file) {
    List<Card> deck;
    while (!file.eof()) {
        Card c;
        string cardline;
        getline(file, cardline);
        if (cardline == "") {
            break;
        }
        istringstream card(cardline);
        card >> c;

        deck.push_back(c);
    }
    return deck;
}

List<Card> shuffle(const List<Card>& deck) {
    int rand = 0;
    List<Card> new_deck;
    for (auto c = deck.begin(); c != deck.end(); c++) {
        rand = rand221(); //assign rand to random value
        if (rand % 2 == 0) {
            new_deck.push_front(*c);
        }
        else {
            new_deck.push_back(*c);
        }
    }
    return new_deck;
}