#ifndef DECK_H
#define DECK_H

#include <QVector>
#include <QPair>
#include "CardData.h"

using CardInfo = QPair<Card::Suit, Card::Rank>;

class Deck
{
public:
    Deck();

    void fill();

    void shuffle();

    CardInfo draw();

    bool isEmpty() const;
    int cardsLeft() const;

private:
    QVector<CardInfo> m_cards;
};

#endif // DECK_H
