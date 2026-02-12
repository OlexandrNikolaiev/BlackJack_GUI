#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QRandomGenerator>

#include "../../../Core/blackjackgame.h"
#include "../../Outcomes/outcome.h"
#include "../../Dimmer/overlaywidget.h"
#include "../BettingPanel/bettingpanel.h"

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
#ifdef Q_OS_WINDOWS
    bool nativeEvent(const QByteArray &eventType, void *message, qint64 *result) override;
#endif
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

    void onCardDealtToPlayer(Card::Suit suit, Card::Rank rank, int newScore);
    void onCardDealtToDealer(Card::Suit suit, Card::Rank rank, bool isFaceUp, int newScore);
    void onDealerCardRevealed(int index, int newScore);

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
    void updateStyles();

    BlackjackGame* m_game;

    QWidget *m_panelContainer;
    BettingPanel *m_bettingPanel;
    QPropertyAnimation *m_panelAnimation;
    bool m_isPanelVisible;

    Outcome* m_outcomeWidget = nullptr;
    OverlayWidget* m_overlay = nullptr;

    int mBorderSize;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
