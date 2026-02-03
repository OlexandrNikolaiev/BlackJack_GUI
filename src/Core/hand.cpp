#include "hand.h"

#include <QDebug>

Hand::Hand()
{
}

void Hand::addCard(Card::Suit suit, Card::Rank rank, bool isFaceUp)
{
    HandCardEntry entry;
    entry.info = {suit, rank};
    entry.isFaceUp = isFaceUp;
    m_cards.append(entry);
}

void Hand::clear()
{
    m_cards.clear();
}

void Hand::setCardFaceUp(int index, bool isFaceUp)
{
    if (index >= 0 && index < m_cards.size()) {
        m_cards[index].isFaceUp = isFaceUp;
    }
}

int Hand::getTotalScore() const
{
    return calculateScoreInternal(false);
}

int Hand::getVisibleScore() const
{
    return calculateScoreInternal(true);
}

bool Hand::isBlackjack() const
{
   return m_cards.size() == 2 && getTotalScore() == 21;
}

bool Hand::isBusted() const
{
    return getTotalScore() > 21;
}

int Hand::size() const
{
    return m_cards.size();
}

int Hand::calculateScoreInternal(bool onlyVisible) const
{
    int score = 0;
    int aceCount = 0;

    for (const HandCardEntry& entry : m_cards) {
        if (onlyVisible && !entry.isFaceUp) {
            continue;
        }

        Card::Rank rank = entry.info.second;
        int val = Card::getValue(rank);

        score += val;

        if (rank == Card::Ace) {
            aceCount++;
        }
    }

    while (score > 21 && aceCount > 0) {
        score -= 10;
        aceCount--;
    }

    qDebug()<<"returning score = "<<score << " "<<this;
    return score;
}
