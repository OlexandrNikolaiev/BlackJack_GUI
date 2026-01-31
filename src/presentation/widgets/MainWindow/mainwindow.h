#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QRandomGenerator>

#include "../BettingPanel/bettingpanel.h"
#include "../../../Core/carddata.h"
#include "../../../Core/deck.h"


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

    void onDebugDealPlayer();
    void onDebugDealDealer();

    void onDealClicked();
    void checkChipsAvailability(int currentBetOnTable);

    void onRoundFinished();

    void onAllInClicked();

private:
    void ConnectSignals();
    void applyShadowEffect();

    void CloseWindow();
    void CollapseWindow();

    void setupBettingPanel();

    Deck m_deck;

    QWidget *m_panelContainer;
    BettingPanel *m_bettingPanel;
    QPropertyAnimation *m_panelAnimation;
    bool m_isPanelVisible;

    int mBorderSize;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
