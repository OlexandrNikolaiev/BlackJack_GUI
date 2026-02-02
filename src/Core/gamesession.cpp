#include "gamesession.h"
#include <cmath>

#include <QDebug>

GameSession::GameSession()
    : m_currentBet(0)
{
}

void GameSession::addBet(int amount)
{
    if (amount > 0) {
        m_currentBet += amount;
    }
}

void GameSession::clearBet()
{
    m_currentBet = 0;
}

int GameSession::getBet() const
{
    qDebug()<<"returning "<< m_currentBet;
    return m_currentBet;
}

int GameSession::calculatePayout(double ratio) const
{
    if (ratio == 0.0) return 0;

    double winPart = m_currentBet * ratio;
    return m_currentBet + static_cast<int>(winPart);
}
