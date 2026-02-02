#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QRandomGenerator>

#include "../BettingPanel/bettingpanel.h"
//#include "../../../Core/deck.h"
//#include "../../../Core/gamesession.h"
#include "../../../Core/blackjackgame.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qint64 *result) override;
    void changeEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void showBettingPanel();
    void hideBettingPanel();
    void toggleBettingPanel();
    void onChipClicked(int value);
    void updateBetLabel(int amount);
    void onStackClicked();
    void onAllInClicked();
    void openSettings();

    void onDealClicked();
    void onHitClicked();
    void onStandClicked();

    void onCardDealtToPlayer(Card::Suit suit, Card::Rank rank);
    void onCardDealtToDealer(Card::Suit suit, Card::Rank rank, bool isFaceUp);
    void onDealerTurnStarted();
    void onRoundFinished(BlackjackGame::GameResult result, int payout);
    void onGameError(const QString& message);

    void resetGameAndShowBetting();

    //void onDebugDealPlayer();
    //void onDebugDealDealer();



private:
    void ConnectSignals();
    void applyShadowEffect();
    void CloseWindow();
    void CollapseWindow();
    void setupBettingPanel();
    void checkChipsAvailability(int currentBetOnTable);

    //Deck m_deck;
    //GameSession m_gameSession;
    BlackjackGame* m_game;
    QWidget *m_panelContainer;
    BettingPanel *m_bettingPanel;
    QPropertyAnimation *m_panelAnimation;
    bool m_isPanelVisible;
    int mBorderSize;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
