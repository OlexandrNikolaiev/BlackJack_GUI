#include "outcome.h"

#include <QGraphicsDropShadowEffect>

#include "ui_outcome.h"
#include "../Styles/styles.h"

Outcome::Outcome(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Outcome)
{
    ui->setupUi(this);

    Styles::Effects::applyShadowOutcome(this);

    hide();
}

Outcome::~Outcome()
{
    delete ui;
}

void Outcome::setGameResult(BlackjackGame::GameResult result, int payout)
{
    QString text;
    QString style;

    QString bgPath = ":/images/res/images/outcomes/";

    switch(result) {
    case BlackjackGame::PlayerWin:
    case BlackjackGame::PlayerBlackjack:
    case BlackjackGame::DealerBust:
        if (result == BlackjackGame::PlayerBlackjack)
            text = tr("BLACKJACK!");
        else if (result == BlackjackGame::DealerBust)
            text = tr("DEALER BUST! YOU WON!");
        else
            text = tr("YOU WON!");

        style = QString("#frame { border-image: url(%1player_won.png) 10 10 10 10 stretch; }").arg(bgPath);
        break;

    case BlackjackGame::DealerWin:
        text = tr("DEALER WINS");
        style = QString("#frame { border-image: url(%1dealer_won.png) 10 10 10 10 stretch; }").arg(bgPath);
        break;

    case BlackjackGame::PlayerBust:
        text = tr("BUSTED");
        style = QString("#frame { border-image: url(%1bust.png) 10 10 10 10 stretch; }").arg(bgPath);
        break;

    case BlackjackGame::Push:
        text = tr("PUSH (TIE)");
        style = QString("#frame { border-image: url(%1bust.png) 10 10 10 10 stretch; }").arg(bgPath);
        break;
    }

    ui->label->setText(text);
    ui->frame->setStyleSheet(style);
}
