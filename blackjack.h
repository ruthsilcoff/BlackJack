//Ruth Silcoff, 260773460

#ifndef COMP322_BLACKJACK_H
#define COMP322_BLACKJACK_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm> //random shuffle

using namespace std;

class Card {
public:
    enum rank {
        ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
    };
    enum suit {
        CLUBS, DIAMONDS, HEARTS, SPADES
    };

    Card(rank r = ACE, suit s = CLUBS);
    int getValue() const;
    void displayCard() const;
private:
    rank m_rank;
    suit m_suit;
};

class Hand {
public:
    Hand();
    virtual ~Hand();
    void add(Card* pCard);
    void clear();
    int getTotal() const;
protected:
    vector<Card*> m_cards;
};

class Deck : public Hand {
public:
    Deck();
    virtual ~Deck();
    void Populate();
    void shuffle();
    void deal(Hand& aHand);
};

class AbstractPlayer : public Hand{
public:
    AbstractPlayer(const string& name = "");
    virtual ~AbstractPlayer();
    virtual bool isDrawing() const = 0;
    bool isBusted() const;
protected:
    string m_name;
};

class HumanPlayer : public AbstractPlayer {
public:
    HumanPlayer(const string& name = "");
    virtual ~HumanPlayer();
    bool isDrawing() const;
    void announce(int cond) const;
};

class ComputerPlayer : public AbstractPlayer
{
public:
    ComputerPlayer(const string& name = "Casino");
    virtual ~ComputerPlayer();
    virtual bool isDrawing() const;
};

class BlackJackGame {
public:
    BlackJackGame();
    ~BlackJackGame();
    void play();
private:
    Deck m_deck;
    ComputerPlayer m_casino;
    HumanPlayer m_player;
};

#endif //COMP322_BLACKJACK_H