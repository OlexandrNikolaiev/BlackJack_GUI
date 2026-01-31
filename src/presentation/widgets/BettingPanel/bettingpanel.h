#ifndef BETTINGPANEL_H
#define BETTINGPANEL_H

#include <QWidget>
#include <QMap>

class ClickableChipStack;

namespace Ui {
class BettingPanel;
}

class BettingPanel : public QWidget
{
    Q_OBJECT

public:
    explicit BettingPanel(QWidget *parent = nullptr);
    ~BettingPanel();

    void updateChipsAvailability(int currentAvailableFunds);
    void setDisplayedBalance(int amount);
    void setAllInEnabled(bool enabled);

signals:
    void allInRequested();

private slots:
    void onBalanceChanged(int newBalance);

private:
    void applyShadowEffect();
    Ui::BettingPanel *ui;

    QMap<int, ClickableChipStack*> m_chipButtons;
};

#endif // BETTINGPANEL_H
