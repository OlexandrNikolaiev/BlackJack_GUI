#ifndef BETTINGPANEL_H
#define BETTINGPANEL_H

#include <QWidget>

namespace Ui {
class BettingPanel;
}

class BettingPanel : public QWidget
{
    Q_OBJECT

public:
    explicit BettingPanel(QWidget *parent = nullptr);
    ~BettingPanel();

private:
    Ui::BettingPanel *ui;
};

#endif // BETTINGPANEL_H
