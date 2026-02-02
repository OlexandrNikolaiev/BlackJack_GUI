#ifndef HAND_H
#define HAND_H

#include <QVector>
#include "carddata.h"

struct HandCardEntry {
    CardInfo info;
    bool isFaceUp;
};

class Hand
{
public:
    Hand();

    void addCard(Card::Suit suit, Card::Rank rank, bool isFaceUp);

    void clear();

    void setCardFaceUp(int index, bool isFaceUp);

    int getTotalScore() const;

    int getVisibleScore() const;

    bool isBlackjack() const;
    bool isBusted() const;

    int size() const;

private:
    int calculateScoreInternal(bool onlyVisible) const;

    QVector<HandCardEntry> m_cards;
};

#endif // HAND_H
