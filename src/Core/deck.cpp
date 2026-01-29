#include "deck.h"
#include <QRandomGenerator>
#include <algorithm>
#include <qdebug.h>
#include <random>

Deck::Deck()
{
    fill();
    shuffle();
}

void Deck::fill()
{
    m_cards.clear();

    for (int s = 0; s <= 3; ++s) {
        for (int r = 2; r <= 14; ++r) {
            Card::Suit suit = static_cast<Card::Suit>(s);
            Card::Rank rank = static_cast<Card::Rank>(r);
            m_cards.append({suit, rank});
        }
    }
}

void Deck::shuffle()
{
    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    std::shuffle(m_cards.begin(), m_cards.end(), rng);
}

CardInfo Deck::draw()
{
    if (m_cards.isEmpty()) {
        fill();
        shuffle();
    }

    CardInfo card = m_cards.last();

    qDebug()<<"taking "<<m_cards.last();
    m_cards.removeLast();

    return card;
}

bool Deck::isEmpty() const { return m_cards.isEmpty(); }

int Deck::cardsLeft() const { return m_cards.size(); }
