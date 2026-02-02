#include "blackjackgame.h"
#include "../Infrastructure/Service/balancemanager.h"
#include <QTimer>
#include <QTimer>
#include <qdebug.h>
#include <qeventloop.h>

BlackjackGame::BlackjackGame(QObject *parent) : QObject(parent)
{

}

void BlackjackGame::startRound(int betAmount)
{
    if (betAmount <= 0) {
        emit gameError("Invalid bet amount");
        return;
    }
    if (!BalanceManager::instance().hasEnough(betAmount)) {
        emit gameError("Not enough funds");
        return;
    }

    BalanceManager::instance().decrease(betAmount);
    m_session.clearBet();
    m_session.addBet(betAmount);

    m_playerHand.clear();
    m_dealerHand.clear();

    if (m_deck.cardsLeft() < 10) {
        m_deck.fill();
        m_deck.shuffle();
    }

    QEventLoop loop;

    CardInfo c1 = m_deck.draw();
    m_playerHand.addCard(c1.first, c1.second, true);
    emit cardDealtToPlayer(c1.first, c1.second);

    QTimer::singleShot(600, &loop, &QEventLoop::quit);
    loop.exec();

    CardInfo c2 = m_deck.draw();
    m_dealerHand.addCard(c2.first, c2.second, false);
    emit cardDealtToDealer(c2.first, c2.second, false);

    QTimer::singleShot(600, &loop, &QEventLoop::quit);
    loop.exec();

    CardInfo c3 = m_deck.draw();
    m_playerHand.addCard(c3.first, c3.second, true);
    emit cardDealtToPlayer(c3.first, c3.second);

    QTimer::singleShot(600, &loop, &QEventLoop::quit);
    loop.exec();

    CardInfo c4 = m_deck.draw();
    m_dealerHand.addCard(c4.first, c4.second, true);
    emit cardDealtToDealer(c4.first, c4.second, true);

    QTimer::singleShot(600, &loop, &QEventLoop::quit);
    loop.exec();

    if (m_playerHand.isBlackjack()) {
        int payout = m_session.calculatePayout(1.5);
        BalanceManager::instance().increase(payout);
        emit roundFinished(PlayerBlackjack, payout);
    }
    else {
        emit initialDealFinished();
    }
}

void BlackjackGame::playerHit()
{
    CardInfo c = m_deck.draw();
    m_playerHand.addCard(c.first, c.second, true);
    emit cardDealtToPlayer(c.first, c.second);

    if (m_playerHand.isBusted()) {
        emit roundFinished(PlayerBust, 0);
    } else if (m_playerHand.getTotalScore() == 21) {
        QTimer::singleShot(800, this, &BlackjackGame::playerStand);
    }
}

void BlackjackGame::playerStand()
{
    emit dealerTurnStarted();
    runDealerTurn();
}

void BlackjackGame::runDealerTurn()
{
    QEventLoop loop;

    emit dealerTurnStarted(); // block ui (buttons)

    int hiddenCardIndex = 0;

    m_dealerHand.setCardFaceUp(hiddenCardIndex, true);
    qDebug()<<"dealer turn, score = "<<m_dealerHand.getTotalScore();

    emit dealerCardRevealed(hiddenCardIndex);

    QTimer::singleShot(800, &loop, &QEventLoop::quit); // temporary
    loop.exec();

    while (m_dealerHand.getVisibleScore() < 17) {
        CardInfo c = m_deck.draw();
        m_dealerHand.addCard(c.first, c.second, true);
        emit cardDealtToDealer(c.first, c.second, true);

        QTimer::singleShot(1000, &loop, &QEventLoop::quit);
        loop.exec();
    }

    resolveRound();
}

void BlackjackGame::resolveRound()
{
    int pScore = m_playerHand.getTotalScore();
    int dScore = m_dealerHand.getTotalScore();

    GameResult result;
    double payoutRatio = 0.0;

    if (m_dealerHand.isBusted()) {
        result = DealerBust;
        payoutRatio = 1.0;
    } else if (dScore > pScore) {
        result = DealerWin;
        payoutRatio = 0.0;
    } else if (pScore > dScore) {
        result = PlayerWin;
        payoutRatio = 1.0;
    } else {
        result = Push;
        payoutRatio = 0.0;
    }

    int finalPayout = 0;

    if (result == DealerWin) {
        finalPayout = 0;
    }
    else if (result == Push) {
        finalPayout = m_session.getBet();
    }
    else {
        finalPayout = m_session.calculatePayout(payoutRatio);
    }

    if (finalPayout > 0) {
        BalanceManager::instance().increase(finalPayout);
    }

    emit roundFinished(result, finalPayout);
}
