#ifndef OUTCOME_H
#define OUTCOME_H

#include <QWidget>
#include "../../Core/blackjackgame.h"

namespace Ui {
class Outcome;
}

class Outcome : public QWidget
{
    Q_OBJECT

public:
    explicit Outcome(QWidget *parent = nullptr);
    ~Outcome();

    void setGameResult(BlackjackGame::GameResult result, int payout);

private:
    Ui::Outcome *ui;
};

#endif // OUTCOME_H
