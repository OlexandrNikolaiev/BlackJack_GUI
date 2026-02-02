#ifndef BLACKJACKGAME_H
#define BLACKJACKGAME_H

#include <QObject>
#include "deck.h"
#include "hand.h"
#include "gamesession.h"

class BlackjackGame : public QObject
{
    Q_OBJECT
public:
    enum GameResult {
        PlayerWin,
        DealerWin,
        Push,
        PlayerBlackjack,
        PlayerBust,
        DealerBust
    };

    explicit BlackjackGame(QObject *parent = nullptr);

    void startRound(int betAmount);
    void playerHit();
    void playerStand();

    const Hand& getPlayerHand() const { return m_playerHand; }
    const Hand& getDealerHand() const { return m_dealerHand; }
    int getCurrentBet() const { return m_session.getBet(); }

signals:

    // for animations
    void cardDealtToPlayer(Card::Suit suit, Card::Rank rank);
    void cardDealtToDealer(Card::Suit suit, Card::Rank rank, bool isFaceUp);
    void dealerTurnStarted();

    void roundFinished(GameResult result, int payoutAmount);

    void gameError(const QString& message);

    void dealerCardRevealed(int index);

    void initialDealFinished();

private:
    void runDealerTurn();

    void resolveRound();

    Deck m_deck;
    Hand m_playerHand;
    Hand m_dealerHand;
    GameSession m_session;
};

#endif // BLACKJACKGAME_H
